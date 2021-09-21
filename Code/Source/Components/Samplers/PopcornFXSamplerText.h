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

#include <AzFramework/Asset/SimpleAsset.h>
#include <LmbrCentral/Rendering/MaterialAsset.h>

#include "Integration/PopcornFXIntegrationBus.h"

#if defined(LMBR_USE_PK)
#include <pk_particles/include/ps_samplers_text.h>
#endif //LMBR_USE_PK


namespace PopcornFX
{
	class CParticleAttributeSamplerDeclaration;
}

__LMBRPK_BEGIN

	class PopcornFXSamplerText
		: public PopcornFXSamplerComponentRequestBus::Handler
	{
	public:
		AZ_TYPE_INFO(PopcornFXSamplerText, "{BA964576-2207-4A47-B077-63496F8D0368}")

		static void		Reflect(AZ::ReflectContext* context);

		void			CopyFrom(const PopcornFXSamplerText &other);

		void			Activate();
		void			Deactivate();

		AZ::EntityId	m_AttachedToEntityId;

	protected:
		void			OnDataChanged();
		AZStd::string	m_Text;

	private:
		static bool	VersionConverter(	AZ::SerializeContext& context,
										AZ::SerializeContext::DataElementNode& classElement);

#if defined(LMBR_USE_PK)
	public:
		//////////////////////////////////////////////////////////////////////////
		// PopcornFXSamplerComponentRequestBus interface implementation
		PopcornFX::CParticleSamplerDescriptor*	GetDescriptor() override { return m_SamplerDescriptor.Get(); }
		virtual AZ::u32							GetType() override { return CParticleSamplerDescriptor_Text::SamplerTypeID(); };
		//////////////////////////////////////////////////////////////////////////


	protected:
		void	_Clean();
		bool	_BuildDescriptor();

		PParticleSamplerDescriptor	m_SamplerDescriptor = null;
#endif //LMBR_USE_PK
};

__LMBRPK_END
