//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFX_precompiled.h"
#include "PopcornFXFeatureProcessor.h"
#include "Integration/PopcornFXUtils.h"
#include <pk_kernel/include/kr_profiler.h>
#include <Atom/RPI.Public/View.h>
#include <Atom/RHI/DrawPacketBuilder.h>
#include <Atom/RPI.Public/Scene.h>
#include <Atom/Feature/ReflectionProbe/ReflectionProbeFeatureProcessor.h>

#if defined(LMBR_USE_PK)

__LMBRPK_BEGIN
//----------------------------------------------------------------------------

void CPopcornFXFeatureProcessor::Reflect(AZ::ReflectContext* context)
{
	if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
	{
		serializeContext
			->Class<CPopcornFXFeatureProcessor, AZ::RPI::FeatureProcessor>()
			->Version(0);
	}
}

CPopcornFXFeatureProcessor::CPopcornFXFeatureProcessor()
{
}

void	CPopcornFXFeatureProcessor::Activate()
{
}

void	CPopcornFXFeatureProcessor::Deactivate()
{

}

void	CPopcornFXFeatureProcessor::Simulate(const SimulatePacket& packet)
{
	AZ_UNUSED(packet);
	if (!CCurrentThread::IsRegistered())
		CCurrentThread::RegisterUserThread();
}

void	CPopcornFXFeatureProcessor::Render(const RenderPacket& packet)
{
	const SLmbrAtomDrawOutputs& drawCalls = m_RenderManager.GetCollectedDrawCalls();

	// Delete the DrawPackets that were used last frame
	m_drawPackets.clear();

	for (const SLmbrAtomDrawOutputs::SDrawCall& dc : drawCalls.m_DrawCalls)
	{
    	for (const AZ::RPI::ViewPtr &view : packet.m_views)
	    {
    		const AZ::RHI::DrawPacket	*drawPacket = BuildDrawPacket(dc, view->GetRHIShaderResourceGroup(), 0);
    		m_drawPackets.emplace_back(drawPacket);

            CFloat3         bboxCenter = dc.m_BoundingBox.Center();
            AZ::Matrix4x4   cameraMatrix = view->GetViewToWorldMatrix();
            CFloat3         cameraPosition = ToPk(cameraMatrix.GetTranslation());

			view->AddDrawPacket(drawPacket, (cameraPosition - bboxCenter).Length());
		}
	}
}

const AZ::RHI::DrawPacket* CPopcornFXFeatureProcessor::BuildDrawPacket(	const SLmbrAtomDrawOutputs::SDrawCall& pkfxDrawCall,
																		const AZ::RHI::ShaderResourceGroup* viewSrg,
																		AZ::RHI::DrawItemSortKey sortKey)
{
	AZ_UNUSED(viewSrg);
	AZ::RHI::DrawPacketBuilder	dpBuilder;

	dpBuilder.Begin(nullptr);
	dpBuilder.SetDrawArguments(pkfxDrawCall.m_DrawIndexed);
	dpBuilder.SetIndexBufferView(pkfxDrawCall.m_Indices);
	dpBuilder.AddShaderResourceGroup(pkfxDrawCall.m_RendererSrg->GetRHIShaderResourceGroup());
	dpBuilder.AddShaderResourceGroup(pkfxDrawCall.m_MaterialSrg->GetRHIShaderResourceGroup());

    // Actual particle rendering:
    {
    	AZ::RHI::DrawPacketBuilder::DrawRequest materialDr;
    	materialDr.m_listTag = pkfxDrawCall.m_MaterialDrawList;
    	materialDr.m_pipelineState = pkfxDrawCall.m_MaterialPipelineState.get();
    	materialDr.m_streamBufferViews = AZStd::array_view<AZ::RHI::StreamBufferView>(	pkfxDrawCall.m_VertexInputs.RawDataPointer(),
    																					pkfxDrawCall.m_VertexInputs.Count());

		// TODO: set this depending on lit state.
		materialDr.m_stencilRef = (AZ::Render::StencilRefs::UseIBLSpecularPass | AZ::Render::StencilRefs::UseDiffuseGIPass);

    	materialDr.m_sortKey = sortKey;
    	dpBuilder.AddDrawItem(materialDr);
    }

    AZStd::array_view<AZ::RHI::StreamBufferView>  depthVtxInput;

    if (pkfxDrawCall.m_RendererType == Renderer_Billboard ||
		pkfxDrawCall.m_RendererType == Renderer_Mesh)
    {
        depthVtxInput = AZStd::array_view<AZ::RHI::StreamBufferView>(   pkfxDrawCall.m_VertexInputs.RawDataPointer(),
                                                                        pkfxDrawCall.m_VertexInputs.Count());
    }
    else
    {
        depthVtxInput = AZStd::array_view<AZ::RHI::StreamBufferView>(   pkfxDrawCall.m_VertexInputs.RawDataPointer(),
                                                                        1);
    }

    if (pkfxDrawCall.m_OpaqueDepthPipelineState != null)
    {
    	AZ::RHI::DrawPacketBuilder::DrawRequest opaqueDepthDr;
    	opaqueDepthDr.m_listTag = pkfxDrawCall.m_OpaqueDepthDrawList;
    	opaqueDepthDr.m_pipelineState = pkfxDrawCall.m_OpaqueDepthPipelineState.get();
        opaqueDepthDr.m_streamBufferViews = depthVtxInput;
    	opaqueDepthDr.m_sortKey = sortKey;
    	dpBuilder.AddDrawItem(opaqueDepthDr);
    }
    else if (   pkfxDrawCall.m_TransparentDepthMinPipelineState != null &&
                pkfxDrawCall.m_TransparentDepthMaxPipelineState != null)
    {
    	AZ::RHI::DrawPacketBuilder::DrawRequest transparentDepthMinDr;
    	transparentDepthMinDr.m_listTag = pkfxDrawCall.m_TransparentDepthMinDrawList;
    	transparentDepthMinDr.m_pipelineState = pkfxDrawCall.m_TransparentDepthMinPipelineState.get();
    	transparentDepthMinDr.m_streamBufferViews = depthVtxInput;
    	transparentDepthMinDr.m_sortKey = sortKey;
    	dpBuilder.AddDrawItem(transparentDepthMinDr);

    	AZ::RHI::DrawPacketBuilder::DrawRequest transparentDepthMaxDr;
    	transparentDepthMaxDr.m_listTag = pkfxDrawCall.m_TransparentDepthMaxDrawList;
    	transparentDepthMaxDr.m_pipelineState = pkfxDrawCall.m_TransparentDepthMaxPipelineState.get();
    	transparentDepthMaxDr.m_streamBufferViews = depthVtxInput;
    	transparentDepthMaxDr.m_sortKey = sortKey;
    	dpBuilder.AddDrawItem(transparentDepthMaxDr);
    }

    AZ::Data::Instance<AZ::RPI::ShaderResourceGroup> objectSrg = pkfxDrawCall.m_ObjectSrg;

	if (objectSrg != nullptr)
    {
#if 1 // for development branch
		// retrieve probe constant indices
		AZ::RHI::ShaderInputConstantIndex modelToWorldConstantIndex = objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_modelToWorld"));
		AZ_Error("MeshDataInstance", modelToWorldConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

		AZ::RHI::ShaderInputConstantIndex modelToWorldInverseConstantIndex = objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_modelToWorldInverse"));
		AZ_Error("MeshDataInstance", modelToWorldInverseConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

		AZ::RHI::ShaderInputConstantIndex outerObbHalfLengthsConstantIndex = objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_outerObbHalfLengths"));
		AZ_Error("MeshDataInstance", outerObbHalfLengthsConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

		AZ::RHI::ShaderInputConstantIndex innerObbHalfLengthsConstantIndex = objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_innerObbHalfLengths"));
		AZ_Error("MeshDataInstance", innerObbHalfLengthsConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

		AZ::RHI::ShaderInputConstantIndex useReflectionProbeConstantIndex = objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_useReflectionProbe"));
		AZ_Error("MeshDataInstance", useReflectionProbeConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

		AZ::RHI::ShaderInputConstantIndex useParallaxCorrectionConstantIndex = objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_useParallaxCorrection"));
		AZ_Error("MeshDataInstance", useParallaxCorrectionConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

		// retrieve probe cubemap index
		AZ::Name reflectionCubeMapImageName = AZ::Name("m_reflectionProbeCubeMap");
		AZ::RHI::ShaderInputImageIndex reflectionCubeMapImageIndex = objectSrg->FindShaderInputImageIndex(reflectionCubeMapImageName);
		AZ_Error("MeshDataInstance", reflectionCubeMapImageIndex.IsValid(), "Failed to find shader image index [%s]", reflectionCubeMapImageName.GetCStr());

		AZ::Render::ReflectionProbeFeatureProcessor* reflectionProbeFeatureProcessor = GetParentScene()->GetFeatureProcessor<AZ::Render::ReflectionProbeFeatureProcessor>();

		AZ::Render::ReflectionProbeFeatureProcessor::ReflectionProbeVector reflectionProbes;
		reflectionProbeFeatureProcessor->FindReflectionProbes(AZ::Vector3(pkfxDrawCall.m_BoundingBox.Center().x(), pkfxDrawCall.m_BoundingBox.Center().y(), pkfxDrawCall.m_BoundingBox.Center().z()), reflectionProbes);

		if (!reflectionProbes.empty() && reflectionProbes[0])
		{
			objectSrg->SetConstant(modelToWorldConstantIndex, reflectionProbes[0]->GetTransform());
			objectSrg->SetConstant(modelToWorldInverseConstantIndex, AZ::Matrix3x4::CreateFromTransform(reflectionProbes[0]->GetTransform()).GetInverseFull());
			objectSrg->SetConstant(outerObbHalfLengthsConstantIndex, reflectionProbes[0]->GetOuterObbWs().GetHalfLengths());
			objectSrg->SetConstant(innerObbHalfLengthsConstantIndex, reflectionProbes[0]->GetInnerObbWs().GetHalfLengths());
			objectSrg->SetConstant(useReflectionProbeConstantIndex, true);
			objectSrg->SetConstant(useParallaxCorrectionConstantIndex, reflectionProbes[0]->GetUseParallaxCorrection());

			objectSrg->SetImage(reflectionCubeMapImageIndex, reflectionProbes[0]->GetCubeMapImage());
		}
		else
		{
			objectSrg->SetConstant(useReflectionProbeConstantIndex, false);
		}
#else
        // retrieve probe constant indices
        AZ::RHI::ShaderInputConstantIndex posConstantIndex =
            objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_aabbPos"));
        AZ_Error("MeshDataInstance", posConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

        AZ::RHI::ShaderInputConstantIndex outerAabbMinConstantIndex =
            objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_outerAabbMin"));
        AZ_Error("MeshDataInstance", outerAabbMinConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

        AZ::RHI::ShaderInputConstantIndex outerAabbMaxConstantIndex =
            objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_outerAabbMax"));
        AZ_Error("MeshDataInstance", outerAabbMaxConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

        AZ::RHI::ShaderInputConstantIndex innerAabbMinConstantIndex =
            objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_innerAabbMin"));
        AZ_Error("MeshDataInstance", innerAabbMinConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

        AZ::RHI::ShaderInputConstantIndex innerAabbMaxConstantIndex =
            objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_innerAabbMax"));
        AZ_Error("MeshDataInstance", innerAabbMaxConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

        AZ::RHI::ShaderInputConstantIndex useReflectionProbeConstantIndex =
            objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_useReflectionProbe"));
        AZ_Error("MeshDataInstance", useReflectionProbeConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

        AZ::RHI::ShaderInputConstantIndex useParallaxCorrectionConstantIndex =
            objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_useParallaxCorrection"));
        AZ_Error("MeshDataInstance", useParallaxCorrectionConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

        // retrieve probe cubemap index
        AZ::Name reflectionCubeMapImageName = AZ::Name("m_reflectionProbeCubeMap");
        AZ::RHI::ShaderInputImageIndex reflectionCubeMapImageIndex = objectSrg->FindShaderInputImageIndex(reflectionCubeMapImageName);
        AZ_Error(
            "MeshDataInstance", reflectionCubeMapImageIndex.IsValid(), "Failed to find shader image index [%s]",
            reflectionCubeMapImageName.GetCStr());

        // retrieve the list of probes that contain the centerpoint of the mesh
        /*/
        AZ::Render::TransformServiceFeatureProcessor* transformServiceFeatureProcessor = GetParentScene()->GetFeatureProcessor<TransformServiceFeatureProcessor>();
        Transform transform = transformServiceFeatureProcessor->GetTransformForId(m_objectId);*/

        AZ::Render::ReflectionProbeFeatureProcessor* reflectionProbeFeatureProcessor = GetParentScene()->GetFeatureProcessor<AZ::Render::ReflectionProbeFeatureProcessor>();
   
        AZ::Render::ReflectionProbeFeatureProcessor::ReflectionProbeVector reflectionProbes;
        reflectionProbeFeatureProcessor->FindReflectionProbes(AZ::Vector3(pkfxDrawCall.m_BoundingBox.Center().x(), pkfxDrawCall.m_BoundingBox.Center().y(), pkfxDrawCall.m_BoundingBox.Center().z()), reflectionProbes);

        if (!reflectionProbes.empty() && reflectionProbes[0])
        { 
            objectSrg->SetConstant(posConstantIndex, reflectionProbes[0]->GetPosition());
            objectSrg->SetConstant(outerAabbMinConstantIndex, reflectionProbes[0]->GetOuterAabbWs().GetMin());
            objectSrg->SetConstant(outerAabbMaxConstantIndex, reflectionProbes[0]->GetOuterAabbWs().GetMax());
            objectSrg->SetConstant(innerAabbMinConstantIndex, reflectionProbes[0]->GetInnerAabbWs().GetMin());
            objectSrg->SetConstant(innerAabbMaxConstantIndex, reflectionProbes[0]->GetInnerAabbWs().GetMax());
            objectSrg->SetConstant(useReflectionProbeConstantIndex, true);
            objectSrg->SetConstant(useParallaxCorrectionConstantIndex, reflectionProbes[0]->GetUseParallaxCorrection());

            objectSrg->SetImage(reflectionCubeMapImageIndex, reflectionProbes[0]->GetCubeMapImage());
        }
        else
        {
            objectSrg->SetConstant(useReflectionProbeConstantIndex, false);
        }
        if (!objectSrg->IsQueuedForCompile())
            objectSrg->Compile();
        dpBuilder.AddShaderResourceGroup(objectSrg->GetRHIShaderResourceGroup());
#endif
    }

	return dpBuilder.End();
}

void	CPopcornFXFeatureProcessor::Init(CParticleMediumCollection* medCol, const SSceneViews* views)
{
	m_MediumCollection = medCol;
	m_SceneViews = views;
}

//----------------------------------------------------------------------------
__LMBRPK_END

#endif
