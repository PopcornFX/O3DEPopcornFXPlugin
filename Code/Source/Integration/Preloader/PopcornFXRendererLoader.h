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

#if defined(LMBR_USE_PK)

#include <Atom/RPI.Public/Shader/Shader.h>
#include <Atom/RPI.Public/Shader/ShaderReloadNotificationBus.h>
#include <AzCore/Asset/AssetCommon.h>
#include <pk_kernel/include/kr_threads_basics.h>
#include "Integration/Render/LmbrRendererCache.h"

__LMBRPK_BEGIN

class	CRenderManager;

class	PopcornFXRendererLoader
:	public AZ::Data::AssetBus::MultiHandler
,	public AZ::RPI::ShaderVariantFinderNotificationBus::MultiHandler
,	public AZ::RPI::ShaderReloadNotificationBus::MultiHandler
{
public:
	AZ_CLASS_ALLOCATOR(PopcornFXRendererLoader, AZ::SystemAllocator, 0);

	PopcornFXRendererLoader();
	~PopcornFXRendererLoader();

	bool	AddMaterialToCreate(const SMaterialCacheKey &materialCacheKey, const SPipelineStateCacheKey &pipelineCacheKey);
	bool	AddGeometryToLoad(const CString &path, const PLmbrRendererCache &rendererCache);

	void	OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
	void	OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
	void	OnAssetUnloaded(const AZ::Data::AssetId, const AZ::Data::AssetType) override;

	void	OnShaderVariantTreeAssetReady(AZ::Data::Asset<AZ::RPI::ShaderVariantTreeAsset> shaderVariantTreeAsset, bool isError) override;
	void	OnShaderVariantAssetReady(AZ::Data::Asset<AZ::RPI::ShaderVariantAsset> shaderVariantAsset, bool isError) override { AZ_UNUSED(shaderVariantAsset); AZ_UNUSED(isError); }

	void	OnShaderAssetReinitialized(const AZ::Data::Asset<AZ::RPI::ShaderAsset> &shaderAsset) override;
	void	OnShaderReinitialized(const AZ::RPI::Shader &shader) override;
	void	OnShaderVariantReinitialized(const AZ::RPI::ShaderVariant& shaderVariant) override;

	PMaterialCache		FindMaterial(const SMaterialCacheKey& key) const;
	PPipelineStateCache	FindPipelineState(const SPipelineStateCacheKey& key) const;
	PGeometryCache		FindGeometryCache(const CString& path) const;

	void	SetRenderManager(CRenderManager *renderManager) { m_RenderManager = renderManager; }

private:

	enum EAssetType
    {
        AssetType_DiffuseMap,
        AssetType_DiffuseRampMap,
        AssetType_EmissiveMap,
        AssetType_EmissiveRampMap,
        AssetType_NormalMap,
        AssetType_MotionVectorsMap,
        AssetType_AlphaMap,
        AssetType_DistortionMap,
		AssetType_MaterialShader,
		AssetType_OpaqueDepthShader,
		AssetType_TransparentDepthMinShader,
		AssetType_TransparentDepthMaxShader,
		AssetType_Geometry,
	};

    AZ::Data::Instance<AZ::RPI::Shader>         &_GetShaderSlot(EAssetType type, const PPipelineStateCache &pipelineState) const;
    AZ::RHI::ConstPtr<AZ::RHI::PipelineState>   &_GetPipelineStateSlot(EAssetType type, const PPipelineStateCache &pipelineState) const;
    AZ::Data::Instance<AZ::RPI::StreamingImage> &_GetStreamingImageSlot(EAssetType type, const PMaterialCache &material) const;

	struct	SAssetDependencies
	{
		struct	SCaches
		{
			SMaterialCacheKey			m_MaterialKey;
			SPipelineStateCacheKey		m_PipelineStateKey;
			CString						m_GeometryPath;
			EAssetType					m_Type;

			bool	operator == (const SCaches &oth) const;
		};

		PopcornFX::TArray<SCaches>				m_Caches;
		AZ::Data::Asset<AZ::Data::AssetData>	m_AssetRef;
	};
	CRenderManager											*m_RenderManager;

	// Mutex:
	Threads::CCriticalSection								m_Lock;
	// Assets:
	THashMap<SAssetDependencies, AZ::Data::AssetId>			m_Assets;
	// Materials:
	THashMap<PMaterialCache, SMaterialCacheKey>				m_Materials;
	// Pipeline state cache created:
	THashMap<PPipelineStateCache, SPipelineStateCacheKey>	m_PipelineStates;
	// Geometry
	THashMap<PGeometryCache, CString>						m_GeometryCaches;
	// RendererCache dependencies:
	THashMap<TArray<PLmbrRendererCache>, AZ::Data::AssetId>	m_RendererCacheDependencies;

	void										_OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset);
	void										_OnShaderVariantsReloaded(	const AZ::Data::AssetId &shaderId,
																			const AZ::RPI::ShaderVariantId &shaderVariantId,
																			bool searchVariant = true);
	AZ::RHI::ConstPtr<AZ::RHI::PipelineState>	_CreatePipelineStateCache(	AZ::RPI::Shader &shader,
                                                                            EAssetType assetType,
                                                                            const AZ::RPI::ShaderVariantStableId &shaderVariantStableId,
																			const SPipelineStateCacheKey &key);
    bool                                        _AddTextureToLoad(const char *texturePath, const SAssetDependencies::SCaches &currentCaches);
    bool                                        _AddShaderToLoad(const char *shaderPath, const SAssetDependencies::SCaches &currentCaches);
	AZ::Data::AssetId	                        _LoadTexture(const CString &path);
	SAssetDependencies	                        *_InsertAssetIFN(const AZ::Data::AssetId &assetId, const SAssetDependencies::SCaches currentCaches);
	bool										_InsertRendererCacheDependency(const AZ::Data::AssetId& assetId, const PLmbrRendererCache &rendererCache);
	PPipelineStateCache	                        _GetOrCreatePipelineCache(const SPipelineStateCacheKey &key, bool &exist);
	PMaterialCache		                        _GetOrCreateMaterialCache(const SMaterialCacheKey &key, bool &exist);
};

__LMBRPK_END

#endif //LMBR_USE_PK
