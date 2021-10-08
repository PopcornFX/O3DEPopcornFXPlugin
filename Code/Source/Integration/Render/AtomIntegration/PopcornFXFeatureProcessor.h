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

#include <Atom/RPI.Public/FeatureProcessor.h>
#include <Integration/Managers/RenderManager.h>
#include <Integration/Render/CameraInfo.h>

__LMBRPK_BEGIN
//----------------------------------------------------------------------------

class CPopcornFXFeatureProcessorInterface : public AZ::RPI::FeatureProcessor
{
public:
	AZ_RTTI(CPopcornFXFeatureProcessorInterface, "{D86216E4-92A8-43BE-A5E4-8853489C7BA1}", AZ::RPI::FeatureProcessor);
};


class CPopcornFXFeatureProcessor : public CPopcornFXFeatureProcessorInterface
{
public:
	AZ_RTTI(CPopcornFXFeatureProcessor, "{D86216E4-92A8-43BE-123F-883489C75BA1}", CPopcornFXFeatureProcessorInterface);

	static void Reflect(AZ::ReflectContext* context);

	CPopcornFXFeatureProcessor();
	virtual ~CPopcornFXFeatureProcessor() = default;

	// FeatureProcessor overrides ...
	void Activate() override;
	void Deactivate() override;
	void Simulate(const SimulatePacket & packet) override;
	void Render(const RenderPacket & packet) override;

	void	Init(CParticleMediumCollection *medCol, const SSceneViews *views);

	void	AppendLightParticles();

	CRenderManager		&GetRenderManager() { return m_RenderManager; }

private:
	const AZ::RHI::DrawPacket	*BuildDrawPacket(	const SLmbrAtomDrawOutputs::SDrawCall& pkfxDrawCall,
													const AZ::RHI::ShaderResourceGroup *viewSrg,
													AZ::RHI::DrawItemSortKey sortKey);

	AZStd::vector<AZStd::unique_ptr<const AZ::RHI::DrawPacket>>	m_drawPackets;
	CRenderManager												m_RenderManager;
	const SSceneViews											*m_SceneViews = null;
	CParticleMediumCollection									*m_MediumCollection = null;
};

//----------------------------------------------------------------------------
__LMBRPK_END

#endif
