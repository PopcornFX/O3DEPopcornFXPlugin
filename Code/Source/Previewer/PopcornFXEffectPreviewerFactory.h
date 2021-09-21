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

#include <AzCore/Memory/SystemAllocator.h>
#include <AzToolsFramework/AssetBrowser/Previewer/PreviewerFactory.h>

#include <QString>

__LMBRPK_BEGIN

    class PopcornFXEffectPreviewerFactory final
        : public AzToolsFramework::AssetBrowser::PreviewerFactory
    {
    public:
        AZ_CLASS_ALLOCATOR(PopcornFXEffectPreviewerFactory, AZ::SystemAllocator, 0);

        PopcornFXEffectPreviewerFactory() = default;
        ~PopcornFXEffectPreviewerFactory() = default;

        //! AzToolsFramework::AssetBrowser::PreviewerFactory overrides
        AzToolsFramework::AssetBrowser::Previewer* CreatePreviewer(QWidget* parent = nullptr) const override;
        bool IsEntrySupported(const AzToolsFramework::AssetBrowser::AssetBrowserEntry* entry) const override;
        const QString& GetName() const override { return m_name; }

    private:
        QString m_name = "PopcornFXEffectPreviewer";
    };

__LMBRPK_END
