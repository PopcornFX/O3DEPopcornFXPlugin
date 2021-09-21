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
#include <Atom/RPI.Public/Shader/ShaderResourceGroup.h>

__LMBRPK_BEGIN
//----------------------------------------------------------------------------

class CRenderManager;

struct	SLmbrAtomRenderContext
{
	CRenderManager	*m_RenderManager = nullptr;
};

struct	SLmbrAtomDrawOutputs
{
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

	PopcornFX::TArray<SDrawCall>	m_DrawCalls;
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
