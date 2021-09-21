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

#include <AzCore/Component/Component.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Asset/AssetManagerBus.h>
#include <AzCore/Asset/AssetManager.h>

#if defined(LMBR_USE_PK)
#include "Integration/PopcornFXIntegration.h"
#endif

__LMBRPK_BEGIN

	class PopcornFXSystemComponent
		: public AZ::Component
		, private AZ::Data::AssetManagerNotificationBus::Handler
	{
	public:
		AZ_COMPONENT(PopcornFXSystemComponent, "{684942CB-7746-44EF-BB4E-6D045915A624}");

		PopcornFXSystemComponent() = default;
		~PopcornFXSystemComponent() override = default;

		static void	Reflect(AZ::ReflectContext* context);

		static void	GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
		static void	GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
		static void	GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
		static void	GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

	private:
		AZStd::vector<AZStd::unique_ptr<AZ::Data::AssetHandler> >	m_AssetHandlers;

		PopcornFXSystemComponent(const PopcornFXSystemComponent&) = delete;

	protected:
		////////////////////////////////////////////////////////////////////////
		// AZ::Component interface implementation
		void	Init() override;
		void	Activate() override;
		void	Deactivate() override;
		////////////////////////////////////////////////////////////////////////

#if defined(LMBR_USE_PK)
		PopcornFXIntegration	m_Integration;
#endif
	};

__LMBRPK_END

