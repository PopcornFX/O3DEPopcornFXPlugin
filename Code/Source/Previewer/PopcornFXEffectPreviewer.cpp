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

#include <AzCore/PlatformDef.h>

AZ_PUSH_DISABLE_WARNING(4251 4800, "-Wunknown-warning-option") // disable warnings spawned by QT

#include "Source/Previewer/PopcornFXEffectPreviewer.h"
#include "Source/Previewer/ui_PopcornFXEffectPreviewer.h"
#include "Source/Integration/Editor/PackLoader.h"

#include <AzToolsFramework/AssetBrowser/AssetBrowserEntry.h>
#include <Atom/ImageProcessing/ImageProcessingBus.h>

#include <QString>

AZ_POP_DISABLE_WARNING

// Helpers copied over from ImagePreviewer.cpp
namespace
{
    // Get the preview of a sub-image specified by mip
    QImage GetSubImagePreview(ImageProcessingAtom::IImageObjectPtr image, uint32_t mip)
    {
        AZ::u8* imageBuf;
        AZ::u32 pitch;
        image->GetImagePointer(mip, imageBuf, pitch);
        const AZ::u32 width = image->GetWidth(mip);
        const AZ::u32 height = image->GetHeight(mip);
        return QImage(imageBuf, width, height, pitch, QImage::Format_RGBA8888);
    }
}

PopcornFXEffectPreviewer::PopcornFXEffectPreviewer(QWidget* parent)
    : Previewer(parent)
    , m_ui(new Ui::PopcornFXEffectPreviewerClass())
{
    m_ui->setupUi(this);
    Clear();
}

PopcornFXEffectPreviewer::~PopcornFXEffectPreviewer()
{
    AZ::SystemTickBus::Handler::BusDisconnect();

    if (m_createDisplayTextureResult.isRunning())
    {
        m_createDisplayTextureResult.waitForFinished();
    }
}

void PopcornFXEffectPreviewer::Clear() const
{
    m_ui->m_texturePreviewWidget->hide();
}

void PopcornFXEffectPreviewer::Display(const AzToolsFramework::AssetBrowser::AssetBrowserEntry* entry)
{
    using namespace AzToolsFramework::AssetBrowser;
        
    m_previewImageObject = nullptr;

    Clear();
    switch (entry->GetEntryType())
    {
    case AssetBrowserEntry::AssetEntryType::Source:
        DisplaySource(static_cast<const SourceAssetBrowserEntry*>(entry));
        break;
    case AssetBrowserEntry::AssetEntryType::Product:
        DisplayProduct(static_cast<const ProductAssetBrowserEntry*>(entry));
        break;
    }
}

void PopcornFXEffectPreviewer::DisplayProduct(const AzToolsFramework::AssetBrowser::ProductAssetBrowserEntry* product)
{
    AZStd::string   thumbnailPath;
    LmbrPk::GetThumbnailPathForAsset(product->GetFullPath(), thumbnailPath);

    CreateAndDisplayTextureItemAsync(
    [thumbnailPath]
    () -> CreateDisplayTextureResult
    {
        ImageProcessingAtom::IImageObjectPtr imageObject;
        ImageProcessingAtom::ImageProcessingRequestBus::BroadcastResult(imageObject, &ImageProcessingAtom::ImageProcessingRequests::LoadImagePreview, thumbnailPath);
        return { imageObject, "" };
    });

    DisplayTextureItem();
}

void PopcornFXEffectPreviewer::DisplaySource(const AzToolsFramework::AssetBrowser::SourceAssetBrowserEntry* source)
{
    AZStd::string   thumbnailPath;
    LmbrPk::GetThumbnailPathForAsset(source->GetFullPath(), thumbnailPath);

    CreateAndDisplayTextureItemAsync(
    [thumbnailPath]
    () -> CreateDisplayTextureResult
    {
        ImageProcessingAtom::IImageObjectPtr imageObject;
        ImageProcessingAtom::ImageProcessingRequestBus::BroadcastResult(imageObject, &ImageProcessingAtom::ImageProcessingRequests::LoadImagePreview, thumbnailPath);
        return { imageObject, "" };
    });

    DisplayTextureItem();
}

void PopcornFXEffectPreviewer::DisplayTextureItem()
{
    m_ui->m_texturePreviewWidget->show();

    if (m_previewImageObject)
    {
        // Display mip 0 by default
        PreviewSubImage(0);
    }
    else
    {
        m_ui->m_previewImageCtrl->setPixmap(QPixmap());
    }

    updateGeometry();
}

template<class CreateFn>
void PopcornFXEffectPreviewer::CreateAndDisplayTextureItemAsync(CreateFn create)
{
    AZ::SystemTickBus::Handler::BusConnect();
    m_createDisplayTextureResult = QtConcurrent::run(AZStd::move(create));
}

void PopcornFXEffectPreviewer::OnSystemTick()
{
    if (m_createDisplayTextureResult.isFinished())
    {
        CreateDisplayTextureResult result = m_createDisplayTextureResult.result();
        m_previewImageObject = AZStd::move(result.first);

        AZ::SystemTickBus::Handler::BusDisconnect();

        DisplayTextureItem();
    }
}

void PopcornFXEffectPreviewer::PreviewSubImage(uint32_t mip)
{
    QImage previewImage = GetSubImagePreview(m_previewImageObject, mip);

    QPixmap pix = QPixmap::fromImage(previewImage);
    m_ui->m_previewImageCtrl->setPixmap(pix);
    m_ui->m_previewImageCtrl->updateGeometry();
}

#include <Source/Previewer/moc_PopcornFXEffectPreviewer.cpp>
