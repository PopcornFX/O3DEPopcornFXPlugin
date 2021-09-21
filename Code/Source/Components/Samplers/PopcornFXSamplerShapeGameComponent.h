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

#include "PopcornFXSamplerShape.h"

#include <AzCore/Component/Component.h>

__LMBRPK_BEGIN

	class PopcornFXSamplerShapeGameComponent
		: public AZ::Component
{
	public:
		AZ_COMPONENT(PopcornFXSamplerShapeGameComponent, "{B89FD122-D785-4196-B023-2EA940EACF01}");

		virtual void Activate() override;
		virtual void Deactivate() override;

		static void Reflect(AZ::ReflectContext* reflection);
		static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
		static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
		static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
		static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

		PopcornFXSamplerShape	m_Sampler;
	};

__LMBRPK_END
