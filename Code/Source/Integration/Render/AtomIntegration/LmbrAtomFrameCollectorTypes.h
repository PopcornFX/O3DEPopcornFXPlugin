//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#pragma once

#if defined(LMBR_USE_PK)

#include <pk_render_helpers/include/frame_collector/rh_frame_data.h>
#include <Atom/RHI/Buffer.h>
#include <Atom/RHI/DrawItem.h>
#include <Atom/RHI/DrawList.h>
#include <Atom/RHI/PipelineState.h>
#include <Atom/RPI.Public/Scene.h>
#include <Atom/RPI.Public/RPISystemInterface.h>
#include <Atom/RPI.Public/Shader/ShaderResourceGroup.h>
#include <Atom/Feature/CoreLights/SimplePointLightFeatureProcessorInterface.h>

__LMBRPK_BEGIN
//----------------------------------------------------------------------------

class CRenderManager;

struct	SLmbrAtomRenderContext
{
	CRenderManager	*m_RenderManager = nullptr;
};

struct	SLmbrAtomDrawOutputs
{
	typedef AZ::Render::SimplePointLightFeatureProcessorInterface				ParticleLightProcessor;
	typedef AZ::Render::SimplePointLightFeatureProcessorInterface::LightHandle	ParticleLightHandle;

	struct	SLight
	{
		CFloat3		m_Position = CFloat3::ZERO;
		CFloat3		m_Color = CFloat3::ZERO;
		float		m_AttenuationRadius = 0.0f;
	};

	struct	SDrawCall
	{
        ERendererClass                                          m_RendererType;

		CAABB													m_BoundingBox;
		AZ::RHI::DrawIndexed									m_DrawIndexed;
		AZ::RHI::IndexBufferView								m_Indices;
		TSemiDynamicArray<AZ::RHI::StreamBufferView, 0x10>		m_VertexInputs;
		AZ::Data::Instance<AZ::RPI::ShaderResourceGroup>		m_RendererSrg;
		AZ::Data::Instance<AZ::RPI::ShaderResourceGroup>		m_MaterialSrg;
		AZ::Data::Instance<AZ::RPI::ShaderResourceGroup>		m_ObjectSrg;

		AZ::RHI::ConstPtr<AZ::RHI::PipelineState>				m_MaterialPipelineState;
		AZ::RHI::DrawListTag									m_MaterialDrawList;

		AZ::RHI::ConstPtr<AZ::RHI::PipelineState>				m_OpaqueDepthPipelineState;
		AZ::RHI::DrawListTag									m_OpaqueDepthDrawList;
		AZ::RHI::ConstPtr<AZ::RHI::PipelineState>				m_TransparentDepthMinPipelineState;
		AZ::RHI::DrawListTag									m_TransparentDepthMinDrawList;
		AZ::RHI::ConstPtr<AZ::RHI::PipelineState>				m_TransparentDepthMaxPipelineState;
		AZ::RHI::DrawListTag									m_TransparentDepthMaxDrawList;

	};

	void	Clear(AZ::RPI::Scene *scene)
	{
		PK_SCOPEDPROFILE();
		m_DrawCalls.Clear();
		m_Lights.Clear();

		// Release the previous frame's light handles
		ParticleLightProcessor	*processor = scene->GetFeatureProcessor<ParticleLightProcessor>();
		for (ParticleLightHandle& lightHandle : m_LightHandles)
			processor->ReleaseLight(lightHandle);

		m_LightHandles.Clear();
	}

	PopcornFX::TArray<SDrawCall>			m_DrawCalls;
	PopcornFX::TArray<SLight>				m_Lights;
	PopcornFX::TArray<ParticleLightHandle>	m_LightHandles;
};

struct	SLmbrAtomViewUserData { };

class	CLmbrAtomParticleBatchTypes
{
public:
	typedef SLmbrAtomRenderContext		CRenderContext;
	typedef SLmbrAtomDrawOutputs		CFrameOutputData;
	typedef SLmbrAtomViewUserData		CViewUserData;

	enum { kMaxQueuedCollectedFrame = 2U };
};

// Billboarding views are templated with user data:
typedef TSceneView<SLmbrAtomViewUserData>				SLmbrAtomSceneView;

//----------------------------------------------------------------------------
__LMBRPK_END

#endif //LMBR_USE_PK
