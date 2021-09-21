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

#include "PopcornFXPreloader.h"

#if defined(LMBR_USE_PK)

#include "Asset/PopcornFXAsset.h"
#include "Integration/PopcornFXIntegrationBus.h"

#include <AzCore/Asset/AssetManagerBus.h>
#include <AzCore/Asset/AssetManager.h>
#include <LmbrCentral/Rendering/MeshAsset.h>

#include <pk_particles/include/ps_effect.h>
#include <pk_particles/include/ps_mediums.h>

__LMBRPK_BEGIN

void	PopcornFXPreloader::Clear()
{
	{
		AZStd::lock_guard<AZStd::recursive_mutex>	lock(m_AssetsToPreloadMutex);
		for (auto elem = m_AssetsToPreload.begin(); elem != m_AssetsToPreload.end(); ++elem)
		{
			AZ::Data::AssetBus::MultiHandler::BusDisconnect(elem->first);
		}
		m_AssetsToPreload.clear();
	}
	{
		AZStd::lock_guard<AZStd::recursive_mutex>	lock(m_PreloadedAssetsMutex);
		for (auto elem = m_PreloadedAssets.begin(); elem != m_PreloadedAssets.end(); ++elem)
		{
			AZ::Data::AssetBus::MultiHandler::BusDisconnect(elem->first);
		}
		m_PreloadedAssets.clear();
	}
}

bool	PopcornFXPreloader::PreloadEffect(const AZ::Data::AssetId &assetId)
{
	return PreloadAsset(assetId, azrtti_typeid<PopcornFXAsset>());
}

bool	PopcornFXPreloader::PreloadMesh(const AZStd::string &meshPath)
{
	AZ::Data::AssetId	assetId;
	AZ::Data::AssetCatalogRequestBus::BroadcastResult(assetId, &AZ::Data::AssetCatalogRequestBus::Handler::GetAssetIdByPath, meshPath.c_str(), azrtti_typeid<LmbrCentral::MeshAsset>(), false);

	return PreloadAsset(assetId, azrtti_typeid<LmbrCentral::MeshAsset>());
}

bool	PopcornFXPreloader::IsPreloadingDone()
{
	AZStd::lock_guard<AZStd::recursive_mutex>	lock(m_AssetsToPreloadMutex);
	return m_AssetsToPreload.empty();
}

void	PopcornFXPreloader::OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset)
{
	OnAssetReady(asset);
}

void	PopcornFXPreloader::OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset)
{
	// If the preloaded asset is an fx
	PopcornFXAsset				*pkAsset = asset.GetAs<PopcornFXAsset>();
	if (pkAsset != null)
	{
		CParticleMediumCollection	*mediumCollection;

		PopcornFXIntegrationBus::BroadcastResult(mediumCollection, &LmbrPk::PopcornFXIntegrationBus::Handler::GetMediumCollection);
		if (!PK_VERIFY(mediumCollection != null &&
			pkAsset->m_Effect != null &&
			pkAsset->m_Effect->Install(mediumCollection)))
		{
			AZ_Error("PopcornFX", false, "Unable to preload the asset %s", pkAsset->m_Path.c_str());
			return;
		}
	}
	
	{
		AZStd::lock_guard<AZStd::recursive_mutex>	lock(m_PreloadedAssetsMutex);
		m_PreloadedAssets.insert(AZStd::make_pair(asset.GetId(), asset));
	}
	{
		AZStd::lock_guard<AZStd::recursive_mutex>	lock(m_AssetsToPreloadMutex);
		m_AssetsToPreload.erase(asset.GetId());
	}
}

bool	PopcornFXPreloader::PreloadAsset(const AZ::Data::AssetId &assetId, const AZ::Data::AssetType& assetType)
{
	if (!assetId.IsValid() || AssetToPreloadPending(assetId) || AssetPreloaded(assetId))
		return false;

	AssetType	asset = AZ::Data::AssetManager::Instance().GetAsset(assetId, assetType, AZ::Data::AssetLoadBehavior::QueueLoad);

	{
		AZStd::lock_guard<AZStd::recursive_mutex>	lock(m_AssetsToPreloadMutex);
		m_AssetsToPreload.insert(AZStd::make_pair(assetId, asset));
	}

	if (!AZ::Data::AssetBus::MultiHandler::BusIsConnectedId(assetId))
		AZ::Data::AssetBus::MultiHandler::BusConnect(assetId);

	return true;
}

bool	PopcornFXPreloader::AssetToPreloadPending(const AZ::Data::AssetId &assetId)
{
	AZStd::lock_guard<AZStd::recursive_mutex>	lock(m_AssetsToPreloadMutex);

	AssetMap::iterator	it = m_AssetsToPreload.find(assetId);
	return it != m_AssetsToPreload.end();
}

bool	PopcornFXPreloader::AssetPreloaded(const AZ::Data::AssetId &assetId)
{
	AZStd::lock_guard<AZStd::recursive_mutex>	lock(m_PreloadedAssetsMutex);

	AssetMap::iterator	it = m_PreloadedAssets.find(assetId);
	return it != m_PreloadedAssets.end();
}

__LMBRPK_END

#endif //LMBR_USE_PK
