//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFX_precompiled.h"

#include "PopcornFXPreviewersSystemComponent.h"

#include <AzCore/StringFunc/StringFunc.h>

__LMBRPK_BEGIN

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

	AzToolsFramework::AssetBrowser::SourceFileDetails PopcornFXPreviewersSystemComponent::GetSourceFileDetails(const char* fullSourceFileName)
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
		return m_PreviewerFactory->IsEntrySupported(entry) ? m_PreviewerFactory.get() : nullptr;
	}

__LMBRPK_END
