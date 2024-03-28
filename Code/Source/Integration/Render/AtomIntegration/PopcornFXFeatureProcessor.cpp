//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "PopcornFX_precompiled.h"
#include "PopcornFXFeatureProcessor.h"

#if defined(O3DE_USE_PK)

#include "Integration/PopcornFXUtils.h"

#include <pk_kernel/include/kr_profiler.h>

#include <Atom/RPI.Public/View.h>
#include <Atom/RHI/DrawPacketBuilder.h>
#include <Atom/RPI.Public/Scene.h>

#include <Atom/RPI.Reflect/Asset/AssetUtils.h>
#include <Atom/RPI.Public/Pass/PassFilter.h>
#include <Atom/RPI.Public/RenderPipeline.h>

#include <Atom/Feature/RenderCommon.h>

#endif //O3DE_USE_PK

namespace PopcornFX {
//----------------------------------------------------------------------------

void	CPopcornFXFeatureProcessor::Reflect(AZ::ReflectContext *context)
{
	if (auto *serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
	{
		serializeContext
			->Class<CPopcornFXFeatureProcessor, AZ::RPI::FeatureProcessor>()
			->Version(0);
	}
}

CPopcornFXFeatureProcessor::CPopcornFXFeatureProcessor()
{
}

#if defined(O3DE_USE_PK)
void	CPopcornFXFeatureProcessor::Activate()
{
	m_RenderManager.SetFeatureProcessor(this);

	EnableSceneNotification();

}

void	CPopcornFXFeatureProcessor::Deactivate()
{
	DisableSceneNotification();
}

void	CPopcornFXFeatureProcessor::Simulate(const SimulatePacket &packet)
{
	AZ_UNUSED(packet);
	// To enable once O3DE moves the simple point light processor GPU buffer update into their Render() function instead of Simulate()
#if 0
	AppendLightParticles();
#endif
}

void	CPopcornFXFeatureProcessor::Render(const RenderPacket &packet)
{
	const SAtomRenderContext	&drawCalls = m_RenderManager.GetRenderContext();

	// Delete the DrawPackets that were used last frame
	m_drawPackets.clear();

	{
		PK_NAMEDSCOPEDPROFILE("Append draw calls");

		for (const SAtomRenderContext::SDrawCall &dc : drawCalls.m_DrawCalls)
		{
			const bool	castShadows = dc.m_CastShadows;

			for (const AZ::RPI::ViewPtr &view : packet.m_views)
			{
				if ((view->GetUsageFlags() & AZ::RPI::View::UsageShadow) != 0 && !castShadows)
					continue;

				const AZ::RHI::DrawPacket	*drawPacket = BuildDrawPacket(dc, view->GetRHIShaderResourceGroup(), dc.m_GlobalSortOverride);
				m_drawPackets.emplace_back(drawPacket);

				const CFloat3		&bboxCenter = dc.m_BoundingBox.Center();
				const AZ::Matrix4x4	&cameraMatrix = view->GetViewToWorldMatrix();
				CFloat3				cameraPosition = ToPk(cameraMatrix.GetTranslation());

				// Note: sliced draw calls have their bbox center the center of a slice, but the bbox can't be trusted.
				const float			drawCallDepth = (cameraPosition - bboxCenter).Length();

				view->AddDrawPacket(drawPacket, drawCallDepth);
			}
		}
	}
}

void	CPopcornFXFeatureProcessor::AppendLightParticles()
{
	SAtomRenderContext	&drawCalls = const_cast<SAtomRenderContext&>(m_RenderManager.GetRenderContext());

	if (!drawCalls.m_Lights.Empty())
	{
		PK_NAMEDSCOPEDPROFILE("Append lights");
		if (PK_VERIFY(drawCalls.m_LightHandles.Reserve(drawCalls.m_Lights.Count())))
		{
			SAtomRenderContext::ParticleLightProcessor	*processor = GetParentScene()->GetFeatureProcessor<SAtomRenderContext::ParticleLightProcessor>();

			for (const SAtomRenderContext::SLight &light : drawCalls.m_Lights)
			{
				SAtomRenderContext::ParticleLightHandle	lightHandle = processor->AcquireLight();
				processor->SetAttenuationRadius(lightHandle, light.m_AttenuationRadius);
				processor->SetPosition(lightHandle, ToAZ(light.m_Position));

				// TODO: can this be simplified, and check the color space
				// Here, it's doing a CFloat3->AZ::Vector3->AZ::Color->AZ::Render::PhotometricColor convertion/copy per particle..
				AZ::Render::PhotometricColor<SAtomRenderContext::ParticleLightProcessor::PhotometricUnitType> lightColor(AZ::Color::CreateFromVector3(ToAZ(light.m_Color)));
				processor->SetRgbIntensity(lightHandle, lightColor);

				PK_VERIFY(drawCalls.m_LightHandles.PushBack(lightHandle).Valid());
			}
		}
	}
}

const AZ::RHI::DrawPacket	*CPopcornFXFeatureProcessor::BuildDrawPacket(	const SAtomRenderContext::SDrawCall &pkfxDrawCall,
																			const AZ::RHI::ShaderResourceGroup *viewSrg,
																			AZ::RHI::DrawItemSortKey sortKey)
{
	AZ_UNUSED(viewSrg);
	AZ::RHI::DrawPacketBuilder	dpBuilder;

	dpBuilder.Begin(null);
	dpBuilder.SetDrawArguments(pkfxDrawCall.m_DrawIndexed);
	dpBuilder.SetIndexBufferView(pkfxDrawCall.m_Indices);
	dpBuilder.AddShaderResourceGroup(pkfxDrawCall.m_RendererSrg->GetRHIShaderResourceGroup());
	dpBuilder.AddShaderResourceGroup(pkfxDrawCall.m_MaterialSrg->GetRHIShaderResourceGroup());

	// Actual particle rendering:
	{
		AZ::RHI::DrawPacketBuilder::DrawRequest	materialDr;
		materialDr.m_listTag = pkfxDrawCall.m_MaterialDrawList;
		materialDr.m_pipelineState = pkfxDrawCall.m_MaterialPipelineState.get();
		materialDr.m_streamBufferViews = AZStd::span<const AZ::RHI::StreamBufferView>(	pkfxDrawCall.m_VertexInputs.RawDataPointer(),
																						pkfxDrawCall.m_VertexInputs.Count());

		// TODO: set this depending on lit state.
		materialDr.m_stencilRef = (AZ::Render::StencilRefs::UseIBLSpecularPass | AZ::Render::StencilRefs::UseDiffuseGIPass);

		materialDr.m_sortKey = sortKey;
		dpBuilder.AddDrawItem(materialDr);
	}

	AZStd::span<const AZ::RHI::StreamBufferView>	depthVtxInput;

	if (pkfxDrawCall.m_RendererType == Renderer_Billboard ||
		pkfxDrawCall.m_RendererType == Renderer_Mesh)
	{
		depthVtxInput = AZStd::span<const AZ::RHI::StreamBufferView>(	pkfxDrawCall.m_VertexInputs.RawDataPointer(),
																		pkfxDrawCall.m_VertexInputs.Count());
	}
	else
	{
		depthVtxInput = AZStd::span<const AZ::RHI::StreamBufferView>(	pkfxDrawCall.m_VertexInputs.RawDataPointer(),
																		1);
	}

	if (pkfxDrawCall.m_OpaqueDepthPipelineState != null)
	{
		AZ::RHI::DrawPacketBuilder::DrawRequest	opaqueDepthDr;
		opaqueDepthDr.m_listTag = pkfxDrawCall.m_OpaqueDepthDrawList;
		opaqueDepthDr.m_pipelineState = pkfxDrawCall.m_OpaqueDepthPipelineState.get();
		opaqueDepthDr.m_streamBufferViews = depthVtxInput;
		opaqueDepthDr.m_sortKey = sortKey;
		dpBuilder.AddDrawItem(opaqueDepthDr);
	}
	else if (	pkfxDrawCall.m_TransparentDepthMinPipelineState != null &&
				pkfxDrawCall.m_TransparentDepthMaxPipelineState != null)
	{
		AZ::RHI::DrawPacketBuilder::DrawRequest	transparentDepthMinDr;
		transparentDepthMinDr.m_listTag = pkfxDrawCall.m_TransparentDepthMinDrawList;
		transparentDepthMinDr.m_pipelineState = pkfxDrawCall.m_TransparentDepthMinPipelineState.get();
		transparentDepthMinDr.m_streamBufferViews = depthVtxInput;
		transparentDepthMinDr.m_sortKey = sortKey;
		dpBuilder.AddDrawItem(transparentDepthMinDr);

		AZ::RHI::DrawPacketBuilder::DrawRequest	transparentDepthMaxDr;
		transparentDepthMaxDr.m_listTag = pkfxDrawCall.m_TransparentDepthMaxDrawList;
		transparentDepthMaxDr.m_pipelineState = pkfxDrawCall.m_TransparentDepthMaxPipelineState.get();
		transparentDepthMaxDr.m_streamBufferViews = depthVtxInput;
		transparentDepthMaxDr.m_sortKey = sortKey;
		dpBuilder.AddDrawItem(transparentDepthMaxDr);
	}

	AZ::Data::Instance<AZ::RPI::ShaderResourceGroup>	objectSrg = pkfxDrawCall.m_ObjectSrg;

	if (objectSrg != null)
	{
		// retrieve probe constant indices
		AZ::RHI::ShaderInputConstantIndex	useReflectionProbeConstantIndex = objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_useReflectionProbe"));
		AZ_Error("MeshDataInstance", useReflectionProbeConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

#if 0 //PK_O3DE_MAJOR_VERSION == 2111
//See : https://github.com/o3de/o3de/pull/7189 and https://github.com/o3de/o3de/issues/7434
		AZ::RHI::ShaderInputConstantIndex	modelToWorldConstantIndex = objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_modelToWorld"));
		AZ_Error("MeshDataInstance", modelToWorldConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

		AZ::RHI::ShaderInputConstantIndex	modelToWorldInverseConstantIndex = objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_modelToWorldInverse"));
		AZ_Error("MeshDataInstance", modelToWorldInverseConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

		AZ::RHI::ShaderInputConstantIndex	outerObbHalfLengthsConstantIndex = objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_outerObbHalfLengths"));
		AZ_Error("MeshDataInstance", outerObbHalfLengthsConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

		AZ::RHI::ShaderInputConstantIndex	innerObbHalfLengthsConstantIndex = objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_innerObbHalfLengths"));
		AZ_Error("MeshDataInstance", innerObbHalfLengthsConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

		AZ::RHI::ShaderInputConstantIndex	useParallaxCorrectionConstantIndex = objectSrg->FindShaderInputConstantIndex(AZ::Name("m_reflectionProbeData.m_useParallaxCorrection"));
		AZ_Error("MeshDataInstance", useParallaxCorrectionConstantIndex.IsValid(), "Failed to find ReflectionProbe constant index");

		// retrieve probe cubemap index
		AZ::Name	reflectionCubeMapImageName = AZ::Name("m_reflectionProbeCubeMap");
		AZ::RHI::ShaderInputImageIndex reflectionCubeMapImageIndex = objectSrg->FindShaderInputImageIndex(reflectionCubeMapImageName);
		AZ_Error("MeshDataInstance", reflectionCubeMapImageIndex.IsValid(), "Failed to find shader image index [%s]", reflectionCubeMapImageName.GetCStr());

		AZ::Render::ReflectionProbeFeatureProcessor	*reflectionProbeFeatureProcessor = GetParentScene()->GetFeatureProcessor<AZ::Render::ReflectionProbeFeatureProcessor>();

		AZ::Render::ReflectionProbeFeatureProcessor::ReflectionProbeVector	reflectionProbes;
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
#endif
		{
			objectSrg->SetConstant(useReflectionProbeConstantIndex, false);
		}
		if (!objectSrg->IsQueuedForCompile())
			objectSrg->Compile();
		dpBuilder.AddShaderResourceGroup(objectSrg->GetRHIShaderResourceGroup());
	}

	if (pkfxDrawCall.m_RendererType == Renderer_Billboard)
	{
		// See BillboardSrg.azsli, "m_instanceOffset"
		dpBuilder.SetRootConstants(AZStd::span<const uint8_t>(reinterpret_cast<const uint8_t*>(&pkfxDrawCall.m_InstanceOffset), sizeof(u32)));
	}

	return dpBuilder.End();
}

void CPopcornFXFeatureProcessor::OnRenderPipelineChanged(	AZ::RPI::RenderPipeline *renderPipeline,
															AZ::RPI::SceneNotification::RenderPipelineChangeType changeType)
{
	if (changeType == AZ::RPI::SceneNotification::RenderPipelineChangeType::PassChanged)
		UpdateDistortionRenderPassBindings(renderPipeline);
}

void	CPopcornFXFeatureProcessor::AddRenderPasses(AZ::RPI::RenderPipeline *renderPipeline)
{
	AddDistortionRenderPass(renderPipeline);
}

void	CPopcornFXFeatureProcessor::AddDistortionRenderPass(AZ::RPI::RenderPipeline *renderPipeline)
{
	// Only add DistortionParentPass if TransparentPass exists
	if (!renderPipeline->FindFirstPass(AZ::Name("TransparentPass")))
	{
		AZ_Error("PopcornFX", false, "Failed to find TransparentPass to add the distortion pass");
		return;
	}

	// Get the pass request if it's not loaded
	if (!m_passRequestAsset)
	{
		const char *passRequestAssetFilePath = "Passes/DistortionPassRequest.azasset";
		m_passRequestAsset = AZ::RPI::AssetUtils::LoadAssetByProductPath<AZ::RPI::AnyAsset>(
			passRequestAssetFilePath, AZ::RPI::AssetUtils::TraceLevel::Warning);

	}

	const AZ::RPI::PassRequest *passRequest = nullptr;
	if (m_passRequestAsset->IsReady())
	{
		passRequest = m_passRequestAsset->GetDataAs<AZ::RPI::PassRequest>();
	}

	// Return if the pass to be created already exists
	AZ::RPI::PassFilter	passFilter = AZ::RPI::PassFilter::CreateWithPassName(passRequest->m_passName, renderPipeline);
	AZ::RPI::Pass		*pass = AZ::RPI::PassSystemInterface::Get()->FindFirstPass(passFilter);
	if (pass)
	{
		return;
	}

	// Create the pass
	AZ::RPI::Ptr<AZ::RPI::Pass>	distortionParentPass = AZ::RPI::PassSystemInterface::Get()->CreatePassFromRequest(passRequest);
	if (!distortionParentPass)
	{
		AZ_Error("PopcornFX", false, "Create PopcornFX distortion parent pass from pass request failed");
		return;
	}

	// Insert the DistortionParentPass after TransparentPass
	bool	success = renderPipeline->AddPassAfter(distortionParentPass, AZ::Name("TransparentPass"));
	// only create pass resources if it was success
	if (!success)
	{
		AZ_Error("PopcornFX", false, "Add the PopcornFX distortion parent pass to render pipeline [%s] failed", renderPipeline->GetId().GetCStr());
	}
}

void	CPopcornFXFeatureProcessor::UpdateDistortionRenderPassBindings(AZ::RPI::RenderPipeline* renderPipeline)
{
#if 1
	// same as DiffuseProbeGridFeatureProcessor::OnRenderPipelineChanged
	// change the attachment on the DeferredFogPass pass to use the output of the distortion pass
	AZ::RPI::PassFilter	deferredFogPassFilter = AZ::RPI::PassFilter::CreateWithPassName(AZ::Name("DeferredFogPass"), renderPipeline);
	AZ::RPI::Pass		*deferredFogPass = AZ::RPI::PassSystemInterface::Get()->FindFirstPass(deferredFogPassFilter);
	AZ::RPI::PassFilter	distortionPassFilter = AZ::RPI::PassFilter::CreateWithPassName(AZ::Name("DistortionPass"), renderPipeline);
	AZ::RPI::Pass		*distortionPass = AZ::RPI::PassSystemInterface::Get()->FindFirstPass(distortionPassFilter);

	if (distortionPass != null && distortionPass->GetOutputCount())
	{
		if (deferredFogPass != null)
		{
			AZ::RPI::PassAttachmentBinding	&distortionBinding = distortionPass->GetOutputBinding(0);
			AZ::RPI::PassAttachmentBinding	*deferredFogBinding = deferredFogPass->FindAttachmentBinding(AZ::Name("RenderTargetInputOutput"));
			if (deferredFogBinding)
			{
#if 0
				// Doesn't work, overridden by Pass::UpdateConnectedBindings
				//deferredFogBinding->SetAttachment(distortionBinding.GetAttachment());
#else
				// Update the connection instead
				deferredFogBinding->m_connectedBinding = &distortionBinding;
#endif
				distortionPass->SetEnabled(true);
		}
			else
			{
				AZ_Error("PopcornFX", false, "Failed to find RenderTargetInputOutput in DeferredFogPass to bind the distortion pass");
			}
		}
		else
		{
			AZ_Error("PopcornFX", false, "Failed to find DeferredFogPass to bind the distortion pass");
		}
	}
#else
	//Replace the bindings of all passes after the distortion pass 
	AZ::Name	distortionPassName("DistortionPass");
	AZ::RPI::Ptr<AZ::RPI::Pass>	distortionPass = renderPipeline->FindFirstPass(distortionPassName);
	if (!distortionPass)
		return;

	AZ::RPI::PassAttachmentBinding	*distortionInputBinding = distortionPass->FindAttachmentBinding(AZ::Name("InputColor"));
	if (distortionInputBinding == null || distortionPass->GetOutputCount() == 0)
	{
		AZ_Error("PopcornFX", false, "Failed to input/output bindings for %s.", distortionPassName.GetCStr());
		return;
	}

	PK_ASSERT(distortionPass->GetParent() != null);

	AZ::RPI::ParentPass				*parentPass = distortionPass->GetParent();
	AZ::RPI::Pass::ChildPassIndex	passIndex = parentPass->FindChildPassIndex(distortionPassName);
	auto							children = parentPass->GetChildren();
	AZ::RPI::PassAttachmentBinding	*distortionOutputBinding = &distortionPass->GetOutputBinding(0);
	bool							outputConnected = false;

	for (u32 i = passIndex.GetIndex() + 1; i < children.size(); ++i)
	{
		AZ::RPI::Ptr<AZ::RPI::Pass>	pass = children[i];

		for (uint32_t iBinding = 0; iBinding < pass->GetInputCount(); ++iBinding)
		{
			AZ::RPI::PassAttachmentBinding	&binding = pass->GetInputBinding(iBinding);
			if (binding.m_connectedBinding == distortionInputBinding->m_connectedBinding)
			{
				outputConnected = true;
				binding.m_connectedBinding = distortionOutputBinding;
			}

		}
		for (uint32_t iBinding = 0; iBinding < pass->GetInputOutputCount(); ++iBinding)
		{
			AZ::RPI::PassAttachmentBinding	&binding = pass->GetInputOutputBinding(iBinding);
			if (binding.m_connectedBinding == distortionInputBinding->m_connectedBinding)
			{
				outputConnected = true;
				binding.m_connectedBinding = distortionOutputBinding;
			}
		}
	}

	if (!outputConnected)
	{
		AZ_Error("PopcornFX", false, "Failed to connect %s output.", distortionPassName.GetCStr());
		return;
	}
	distortionPass->SetEnabled(true);
#endif
}

void	CPopcornFXFeatureProcessor::Init(CParticleMediumCollection *medCol, const SSceneViews *views)
{
	m_MediumCollection = medCol;
	m_SceneViews = views;
}
#endif //O3DE_USE_PK

//----------------------------------------------------------------------------
}
