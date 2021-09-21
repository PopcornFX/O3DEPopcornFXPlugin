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
#include <AzToolsFramework/AssetBrowser/AssetBrowserBus.h>
#include <AzToolsFramework/AssetBrowser/Previewer/PreviewerBus.h>

#if defined(LMBR_USE_PK)
#include "Previewer/PopcornFXEffectPreviewerFactory.h"
#endif

__LMBRPK_BEGIN

	class PopcornFXPreviewersSystemComponent
		: public AZ::Component
		, public AzToolsFramework::AssetBrowser::AssetBrowserInteractionNotificationBus::Handler
		, private AzToolsFramework::AssetBrowser::PreviewerRequestBus::Handler
	{
	public:
		AZ_COMPONENT(PopcornFXPreviewersSystemComponent, "{BD8742C8-E9EF-4184-B5BB-8A33D7D71D72}");

		PopcornFXPreviewersSystemComponent() = default;
		~PopcornFXPreviewersSystemComponent() override = default;

		static void Reflect(AZ::ReflectContext* context) { PK_UNUSED(context); }
		static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided) { PK_UNUSED(provided); }
		static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible) { PK_UNUSED(incompatible); }
		static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required) { PK_UNUSED(required); }
		static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent) { PK_UNUSED(dependent); }

	private:
		AZStd::unique_ptr<const PopcornFXEffectPreviewerFactory>	m_PreviewerFactory;

		PopcornFXPreviewersSystemComponent(const PopcornFXPreviewersSystemComponent&) = delete;

	protected:
		////////////////////////////////////////////////////////////////////////
		// AZ::Component interface implementation
		void	Init() override;
		void	Activate() override;
		void	Deactivate() override;
		////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////
		// AssetBrowserInteractionNotificationBus::Handler
		AzToolsFramework::AssetBrowser::SourceFileDetails GetSourceFileDetails(const char* fullSourceFileName) override;
		////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////
		// AzToolsFramework::AssetBrowser::PreviewerRequestBus::Handler
		const AzToolsFramework::AssetBrowser::PreviewerFactory* GetPreviewerFactory(const AzToolsFramework::AssetBrowser::AssetBrowserEntry* entry) const override;
		////////////////////////////////////////////////////////////////////////
	};

__LMBRPK_END

