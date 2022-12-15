//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#if defined(O3DE_USE_PK)

#include <Atom/RPI.Public/Shader/Shader.h>
#include <Atom/RPI.Public/Shader/ShaderReloadNotificationBus.h>
#include <AzCore/Asset/AssetCommon.h>
#include <pk_kernel/include/kr_threads_basics.h>
#include "Integration/Render/AtomIntegration/AtomRendererCache.h"

namespace PopcornFX {

class	CRenderManager;

class	PopcornFXRendererLoader
:	public AZ::Data::AssetBus::MultiHandler
,	public AZ::RPI::ShaderReloadNotificationBus::MultiHandler
{
public:
	AZ_CLASS_ALLOCATOR(PopcornFXRendererLoader, AZ::SystemAllocator, 0);

	PopcornFXRendererLoader();
	~PopcornFXRendererLoader();

	bool	AddMaterialToCreate(const PAtomRendererCache &rendererCache);
	bool	AddGeometryToCreate(const PAtomRendererCache &rendererCache);

	void	OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
	void	OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
	void	OnAssetUnloaded(const AZ::Data::AssetId, const AZ::Data::AssetType) override;

	void	OnShaderAssetReinitialized(const AZ::Data::Asset<AZ::RPI::ShaderAsset> &shaderAsset) override;
	void	OnShaderReinitialized(const AZ::RPI::Shader &shader) override;
	void	OnShaderVariantReinitialized(const AZ::RPI::ShaderVariant &shaderVariant) override;

	void	UpdatePendingRendererCache();
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

	AZ::Data::Instance<AZ::RPI::Shader>			&_GetShaderSlot(EAssetType type, const PPipelineStateCache &pipelineState) const;
	AZ::RHI::ConstPtr<AZ::RHI::PipelineState>	&_GetPipelineStateSlot(EAssetType type, const PPipelineStateCache &pipelineState) const;
	AZ::Data::Instance<AZ::RPI::StreamingImage>	&_GetStreamingImageSlot(EAssetType type, const PMaterialCache &material) const;

	struct	SAssetDependencies
	{
		struct	SCache
		{
			SMaterialCacheKey		m_MaterialKey;
			SPipelineStateCacheKey	m_PipelineStateKey;
			CString					m_GeometryPath;
			EAssetType				m_Type;

			bool	operator == (const SCache &oth) const;
		};

		PopcornFX::TArray<SCache>				m_Caches;
		AZ::Data::Asset<AZ::Data::AssetData>	m_AssetRef;
	};

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

	// Renderer cache waiting for a cache to load:
	TArray<PAtomRendererCache>								m_PendingRendererCaches;
	// Modified caches:
	TArray<PBaseCache>										m_ModifiedCaches;

	THashMap<AZ::Data::Asset<AZ::RPI::ShaderVariantAsset>, AZ::Data::AssetId>	m_LoadedVariants;

	void										_OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset);
	void										_OnShaderVariantsReloaded(	const AZ::Data::AssetId &shaderId,
																			const AZ::RPI::ShaderVariantId &shaderVariantId);
	AZ::RHI::ConstPtr<AZ::RHI::PipelineState>	_CreatePipelineStateCache(	AZ::RPI::Shader &shader,
																			AZ::RPI::ShaderVariant &shaderVariant,
																			EAssetType assetType,
																			const SPipelineStateCacheKey &key);
	bool										_AddTextureToLoad(	const char *texturePath,
																	const SAssetDependencies::SCache &currentCache,
																	const PBaseCache &cache);
	bool										_AddShaderToLoad(	const char *shaderPath,
																	const SAssetDependencies::SCache &currentCache,
																	const PBaseCache &cache);
	bool										_AddGeometryToLoad(	const char *geometryPath,
																	const SAssetDependencies::SCache &currentCache,
																	const PBaseCache &cache);
	AZ::Data::AssetId							_LoadTexture(const CString &path);
	SAssetDependencies							*_InsertAssetIFN(	const AZ::Data::AssetId &assetId,
																	const SAssetDependencies::SCache &currentCache,
																	const PBaseCache &cache);
	bool										_LinkAndUpdateRendererCacheIFP(	const PAtomRendererCache &rendererCache,
																				const PBaseCache &cache,
																				CAtomRendererCache::ECacheType cacheType);
	void										_RemovePendingCacheDependencyIFN(const PBaseCache &cache, AZ::Data::AssetId);


	PPipelineStateCache							_GetOrCreatePipelineCache(const SPipelineStateCacheKey &key, bool &exist);
	PMaterialCache								_GetOrCreateMaterialCache(const SMaterialCacheKey &key, bool &exist);
	PGeometryCache								_GetOrCreateGeometryCache(const CString &key, bool &exist);
};

}

#endif //O3DE_USE_PK
