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

#include "PopcornFXSamplerAnimTrack.h"

#include <AzCore/Component/Component.h>

__LMBRPK_BEGIN

	class PopcornFXSamplerAnimTrackGameComponent
		: public AZ::Component
	{
	public:
		AZ_COMPONENT(PopcornFXSamplerAnimTrackGameComponent, "{96B23921-8763-4617-90A0-AD5F0F688A52}");

		virtual void Activate() override;
		virtual void Deactivate() override;

		static void Reflect(AZ::ReflectContext* reflection);
		static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
		static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
		static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

		PopcornFXSamplerAnimTrack	m_Sampler;
	};

__LMBRPK_END
