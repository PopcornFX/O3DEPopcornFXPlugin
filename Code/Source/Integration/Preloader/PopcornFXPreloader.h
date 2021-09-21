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

#include <AzCore/Asset/AssetTypeInfoBus.h>

__LMBRPK_BEGIN

class PopcornFXPreloader
	: public AZ::Data::AssetBus::MultiHandler
{
public:
	/**
	* Specifies that this class should use AZ::SystemAllocator for memory
	* management by default.
	*/
	AZ_CLASS_ALLOCATOR(PopcornFXPreloader, AZ::SystemAllocator, 0);

	void	Clear();

	bool	PreloadEffect(const AZ::Data::AssetId &assetId);
	bool	PreloadMesh(const AZStd::string &meshPath);
	bool	IsPreloadingDone();

	// AZ::Data::AssetBus::Handler
	void	OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
	void	OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset) override;

private:
	typedef AZ::Data::Asset<AZ::Data::AssetData> AssetType;
	typedef AZStd::unordered_map<AZ::Data::AssetId, AssetType> AssetMap;

	AssetMap				m_AssetsToPreload;
	AZStd::recursive_mutex	m_AssetsToPreloadMutex;
	AssetMap				m_PreloadedAssets;
	AZStd::recursive_mutex	m_PreloadedAssetsMutex;

	bool	PreloadAsset(const AZ::Data::AssetId &assetId, const AZ::Data::AssetType& assetType);
	bool	AssetToPreloadPending(const AZ::Data::AssetId &assetId);
	bool	AssetPreloaded(const AZ::Data::AssetId &assetId);

};

__LMBRPK_END

#endif //LMBR_USE_PK
