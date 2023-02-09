//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "PopcornFX_precompiled.h"

#include "PopcornFXPreviewersSystemComponent.h"

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/StringFunc/StringFunc.h>

namespace PopcornFX {

	void	PopcornFXPreviewersSystemComponent::Reflect(AZ::ReflectContext *context)
	{
		if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context); serialize)
		{
			serialize->Class<PopcornFXPreviewersSystemComponent, AZ::Component>()->Version(0);
		}
	}

	void	PopcornFXPreviewersSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType &provided)
	{
		provided.push_back(AZ_CRC("PopcornFXPreviewersService"));
	}

	void	PopcornFXPreviewersSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType &incompatible)
	{
		incompatible.push_back(AZ_CRC("PopcornFXPreviewersService"));
	}

	void	PopcornFXPreviewersSystemComponent::Init()
	{
		// Curently, only one previewer factory for effect assets, we could have others added in a list.
		m_PreviewerFactory.reset(new PopcornFXEffectPreviewerFactory);
	}

	void PopcornFXPreviewersSystemComponent::Activate()
	{
		AzToolsFramework::AssetBrowser::AssetBrowserInteractionNotificationBus::Handler::BusConnect();
		AzToolsFramework::AssetBrowser::PreviewerRequestBus::Handler::BusConnect();
	}

	void PopcornFXPreviewersSystemComponent::Deactivate()
	{
		AzToolsFramework::AssetBrowser::PreviewerRequestBus::Handler::BusDisconnect();
		AzToolsFramework::AssetBrowser::AssetBrowserInteractionNotificationBus::Handler::BusDisconnect();
	}

	AzToolsFramework::AssetBrowser::SourceFileDetails PopcornFXPreviewersSystemComponent::GetSourceFileDetails(const char *fullSourceFileName)
	{
		const char	*iconPath = "Icons/PopcornFX_Icon.png";
		// Add any PopcornFX asset icons in here
		if (AZ::StringFunc::EndsWith(fullSourceFileName, "pkfx"))
			return AzToolsFramework::AssetBrowser::SourceFileDetails(iconPath);
		if (AZ::StringFunc::EndsWith(fullSourceFileName, "pkat"))
			return AzToolsFramework::AssetBrowser::SourceFileDetails(iconPath);

		// Additional assets we might want to add an icon for later ?
#if 0
		if (AZ::StringFunc::EndsWith(fullSourceFileName, "pkvf"))
			return AzToolsFramework::AssetBrowser::SourceFileDetails(iconPath);
		if (AZ::StringFunc::EndsWith(fullSourceFileName, "pkcf"))
			return AzToolsFramework::AssetBrowser::SourceFileDetails(iconPath);
		if (AZ::StringFunc::EndsWith(fullSourceFileName, "pkfm"))
			return AzToolsFramework::AssetBrowser::SourceFileDetails(iconPath);
		if (AZ::StringFunc::EndsWith(fullSourceFileName, "pksc"))
			return AzToolsFramework::AssetBrowser::SourceFileDetails(iconPath);
#endif
		return AzToolsFramework::AssetBrowser::SourceFileDetails();
	}

	const AzToolsFramework::AssetBrowser::PreviewerFactory	*PopcornFXPreviewersSystemComponent::GetPreviewerFactory(const AzToolsFramework::AssetBrowser::AssetBrowserEntry *entry) const
	{
		return m_PreviewerFactory->IsEntrySupported(entry) ? m_PreviewerFactory.get() : null;
	}

}
