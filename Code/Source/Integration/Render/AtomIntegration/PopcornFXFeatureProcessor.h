//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once


#include <Atom/RPI.Public/FeatureProcessor.h>

#if defined(O3DE_USE_PK)
#include <Integration/Managers/RenderManager.h>
#include <Integration/Render/CameraInfo.h>
#endif //O3DE_USE_PK

namespace PopcornFX {
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
	AZ_CLASS_ALLOCATOR(CPopcornFXFeatureProcessor, AZ::SystemAllocator, 0);

	CPopcornFXFeatureProcessor();
	virtual ~CPopcornFXFeatureProcessor() = default;

	static void	Reflect(AZ::ReflectContext *context);

#if defined(O3DE_USE_PK)
	// FeatureProcessor overrides ...
	void		Activate() override;
	void		Deactivate() override;
	void		Simulate(const SimulatePacket &packet) override;
	void		Render(const RenderPacket &packet) override;
	void		OnRenderPipelineChanged(AZ::RPI::RenderPipeline *renderPipeline, AZ::RPI::SceneNotification::RenderPipelineChangeType changeType) override;
	void		AddRenderPasses(AZ::RPI::RenderPipeline *renderPipeline) override;

	void	AddDistortionRenderPass(AZ::RPI::RenderPipeline* renderPipeline);
	void	UpdateDistortionRenderPassBindings(AZ::RPI::RenderPipeline* renderPipeline);

	void	Init(CParticleMediumCollection *medCol, const SSceneViews *views);

	void	AppendLightParticles();

	CRenderManager		&GetRenderManager() { return m_RenderManager; }

private:

#if O3DE_VERSION_MAJOR >= 4 && O3DE_VERSION_MINOR >= 2
	using DrawPacketPtr = AZ::RHI::ConstPtr<AZ::RHI::DrawPacket>;
	using DrawPackets = AZStd::vector<AZ::RHI::ConstPtr<AZ::RHI::DrawPacket>>;
#else
	using DrawPacketPtr = const AZ::RHI::DrawPacket*;
	using DrawPackets = AZStd::vector<AZStd::unique_ptr<const AZ::RHI::DrawPacket>>;
#endif

	DrawPacketPtr			BuildDrawPacket(SAtomRenderContext::SDrawCall &pkfxDrawCall,
											const AZ::RHI::ShaderResourceGroup *viewSrg,
											AZ::RHI::DrawItemSortKey sortKey);

	DrawPackets													m_drawPackets;
	CRenderManager												m_RenderManager;
	const SSceneViews											*m_SceneViews = null;
	CParticleMediumCollection									*m_MediumCollection = null;
	AZ::Data::Asset<AZ::RPI::AnyAsset>							m_passRequestAsset;
#endif //O3DE_USE_PK
};

//----------------------------------------------------------------------------
}
