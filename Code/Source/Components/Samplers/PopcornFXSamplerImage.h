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

#include <LmbrCentral/Rendering/MaterialAsset.h>
#include <AzFramework/Asset/SimpleAsset.h>

#include "Integration/PopcornFXIntegrationBus.h"

#if defined(LMBR_USE_PK)
#include <AzCore/Component/TickBus.h>
#include <pk_particles/include/ps_samplers_image.h>
#include <pk_imaging/include/im_samplers.h>
#endif //LMBR_USE_PK

__LMBRPK_BEGIN

	class PopcornFXSamplerImage
		: public PopcornFXSamplerComponentRequestBus::Handler
#if 0//defined(LMBR_USE_PK)
		, public AZ::TickBus::Handler
#endif //LMBR_USE_PK
	{
	public:
		enum class TexcoordMode
		{
			Clamp,
			Wrap,
		};

		AZ_TYPE_INFO(PopcornFXSamplerImage, "{969B9AC2-3218-4208-AA82-CFCD985CF1A3}")

		static void	Reflect(AZ::ReflectContext* context);

		void		CopyFrom(const PopcornFXSamplerImage &other);

		void		Activate();
		void		Deactivate();

		AZ::EntityId	m_AttachedToEntityId;

	protected:
		AZ::u32		_OnDataChanged();
		void		_OnTexcoordModeChanged();

		AzFramework::SimpleAssetReference<LmbrCentral::TextureAsset>	m_Texture;
		TexcoordMode													m_TexcoordMode = TexcoordMode::Clamp;

	private:
		static bool VersionConverter(	AZ::SerializeContext& context,
										AZ::SerializeContext::DataElementNode& classElement);

#if 0//defined(LMBR_USE_PK)
	public:
		//////////////////////////////////////////////////////////////////////////
		// PopcornFXSamplerComponentRequestBus interface implementation
		PopcornFX::CParticleSamplerDescriptor*	GetDescriptor() override { return m_SamplerDescriptor.Get(); }
		virtual AZ::u32							GetType() override { return CParticleSamplerDescriptor_Image::SamplerTypeID(); };
		//////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////
		// AZ::TickBus::Handler interface implementation
		virtual void	OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
		////////////////////////////////////////////////////////////////////////

	protected:
		void	_Clean();
		void	_LoadTexture();
		bool	_BuildDescriptor(ITexture *texture);

		PParticleSamplerDescriptor	m_SamplerDescriptor = null;
		CImageSampler				*m_ImageSampler = null;
		bool						m_LoadTexturePending = false;

#endif //LMBR_USE_PK
};

__LMBRPK_END
