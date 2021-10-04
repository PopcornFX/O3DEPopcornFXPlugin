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

#include "Integration/PopcornFXIntegrationBus.h"

#include <pk_kernel/include/kr_resources.h>
#include <pk_imaging/include/im_image.h>
#include <Atom/RHI.Reflect/Format.h>

__LMBRPK_BEGIN

class CImageResourceHandler : public IResourceHandler
{
public:
    CImageResourceHandler();
    virtual ~CImageResourceHandler();
 
    virtual void    *Load(  const CResourceManager  *resourceManager,
                            u32                     resourceTypeID,
                            const CString           &resourcePath,
                            bool                    pathNotVirtual,
                            const SResourceLoadCtl  &loadCtl,
                            CMessageStream          &loadReport,
                            SAsyncLoadStatus        *asyncLoadStatus) override;
 
    virtual void    *Load(  const CResourceManager  *resourceManager,
                            u32                     resourceTypeID,
                            const CFilePackPath     &resourcePath,
                            const SResourceLoadCtl  &loadCtl,
                            CMessageStream          &loadReport,
                            SAsyncLoadStatus        *asyncLoadStatus) override;
 
    virtual void    Unload( const CResourceManager  *resourceManager,
                            u32                      resourceTypeID,        // used to check we are called with the correct type
                            void                    *resource) override;

    virtual void    AppendDependencies( const CResourceManager      *resourceManager,
                                        u32                         resourceTypeID,
                                        void                        *resource,
                                        PopcornFX::TArray<CString>  &outResourcePaths) const override;
 
    virtual void    AppendDependencies( const CResourceManager      *resourceManager,
                                        u32                         resourceTypeID,
                                        const CString               &resourcePath,
                                        bool                        pathNotVirtual,
                                         PopcornFX::TArray<CString> &outResourcePaths) const override;
 
    virtual void    AppendDependencies( const CResourceManager      *resourceManager,
                                        u32                         resourceTypeID,
                                        const CFilePackPath         &resourcePath,
                                        PopcornFX::TArray<CString>  &outResourcePaths) const override;
 
    virtual void    BroadcastResourceChanged(const CResourceManager *resourceManager, const CFilePackPath &resourcePath) override;

private:
    Threads::CCriticalSection                  m_Lock;

    struct  SImageResource
    {
        PImage  m_Image;
        u32     m_RefCount;
    };

    THashMap<SImageResource, AZ::Data::AssetId> m_AssetIdToImg;
    THashMap<AZ::Data::AssetId, CImage*>        m_ImgToAssetId;

    struct  SAtomToPkFormat
    {
        AZ::RHI::Format     m_AtFormat;
        CImage::EFormat     m_PkFormat;
        u32                 m_PkFlags;
    };

public:
    static SAtomToPkFormat     m_AtomToPk[];
};

//----------------------------------------------------------------------------

void	ToPkImageFormatAndFlags(const AZ::RHI::Format &imgFormat, PopcornFX::CImage::EFormat &outPkImatFormat, u32 &outPkImageFlags);

//----------------------------------------------------------------------------

__LMBRPK_END
