//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/EBus/EBus.h>

#if defined(O3DE_USE_PK)
#include "Integration/PopcornFXIntegration.h"
#endif

#if !defined(POPCORNFX_BUILDER)
#include <Atom/RPI.Public/Pass/PassSystemInterface.h>
#endif

namespace PopcornFX {

	class PopcornFXSystemComponent
		: public AZ::Component
	{
	public:
		AZ_COMPONENT(PopcornFXSystemComponent, "{684942CB-7746-44EF-BB4E-6D045915A624}");

		PopcornFXSystemComponent() = default;
		~PopcornFXSystemComponent() override = default;

		static void	Reflect(AZ::ReflectContext *context);

		static void	GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType &provided);
		static void	GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType &incompatible);
		static void	GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType &required);
		static void	GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType &dependent);

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

#if defined(O3DE_USE_PK)
		PopcornFXIntegration	m_Integration;
#endif

#if !defined(POPCORNFX_BUILDER)
		void	LoadPassTemplateMappings();
		AZ::RPI::PassSystemInterface::OnReadyLoadTemplatesEvent::Handler m_LoadTemplatesHandler;
#endif
	};

}

