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

#include "Integration/Render/AtomIntegration/LmbrAtomFrameCollector.h"
#include "Integration/Render/AtomIntegration/LmbrAtomRenderDataFactory.h"

#include <Atom/RHI/BufferPool.h>
#include <Atom/RPI.Public/Pass/Pass.h>

__LMBRPK_BEGIN
//----------------------------------------------------------------------------

struct SSceneViews;

class CRenderManager
{
public:
	void	Activate(CParticleMediumCollection *mediumCollection, const AZStd::string &packPath);
	void	Deactivate();
	void	Reset();
	void	SetPackPath(const AZStd::string &packPath);
	void	StartUpdate(CParticleMediumCollection *mediumCollection, const SSceneViews *sceneViews);
	void	StopUpdate(CParticleMediumCollection *mediumCollection);

	AZ::RHI::Ptr<AZ::RHI::Buffer>	ResizeOrCreateBufferIFN(AZ::RHI::Ptr<AZ::RHI::Buffer> buffer, AZ::u64 bufferSize, AZ::u32 alignBufferSize = 0x1000);
	void							*MapBuffer(AZ::RHI::Ptr<AZ::RHI::Buffer> buffer, AZ::u64 sizeToMap);
	void							UnmapBuffer(AZ::RHI::Ptr<AZ::RHI::Buffer> buffer);

	const SLmbrAtomDrawOutputs		&GetCollectedDrawCalls() const { return m_CollectedDrawCalls; }
	void							UnregisterEffectMaterials(const PParticleEffect& effect);

private:
	void	CollectFrame(CParticleMediumCollection *mediumCollection);

	AZ::RHI::Ptr<AZ::RHI::Buffer>	AllocBuffer(AZ::u64 bufferSize, AZ::RHI::BufferBindFlags binding, AZ::u32 alignSizeOn = 0x10);

	AZ::RHI::Ptr<AZ::RHI::BufferPool>	m_BufferPool;

	CLmbrAtomFrameCollector				m_FrameCollector;
	CLmbrAtomRenderDataFactory			m_RenderBatchFactory;
	SLmbrAtomRenderContext				m_RenderContext;
	SLmbrAtomDrawOutputs				m_CollectedDrawCalls;

	const SSceneViews					*m_SceneViews = null; // Used by CollectFrame
};

//----------------------------------------------------------------------------
__LMBRPK_END

#endif //LMBR_USE_PK
