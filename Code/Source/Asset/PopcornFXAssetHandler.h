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

#include <AzCore/Asset/AssetManager.h>
#include <AzCore/Asset/AssetTypeInfoBus.h>

__LMBRPK_BEGIN

	class PopcornFXAssetHandler
		: public AZ::Data::AssetHandler
		, public AZ::AssetTypeInfoBus::Handler
	{
	public:

		AZ_CLASS_ALLOCATOR(PopcornFXAssetHandler, AZ::SystemAllocator, 0);

		~PopcornFXAssetHandler() override;

		//////////////////////////////////////////////////////////////////////////////////////////////
		// AZ::Data::AssetHandler
		virtual AZ::Data::AssetPtr	CreateAsset(const AZ::Data::AssetId& id, const AZ::Data::AssetType& type) override;
		virtual void				DestroyAsset(AZ::Data::AssetPtr ptr) override;
		virtual void				GetHandledAssetTypes(AZStd::vector<AZ::Data::AssetType>& assetTypes) override;
		virtual bool				CanHandleAsset(const AZ::Data::AssetId& id) const override;
		//////////////////////////////////////////////////////////////////////////////////////////////

		static AZ::Data::AssetType	GetAssetTypeStatic();

		//////////////////////////////////////////////////////////////////////////////////////////////
		// AZ::AssetTypeInfoBus::Handler
		virtual AZ::Data::AssetType	GetAssetType() const override;
		virtual const char			*GetAssetTypeDisplayName() const override;
		virtual const char			*GetGroup() const override;
		virtual const char			*GetBrowserIcon() const override;
		virtual AZ::Uuid			GetComponentTypeId() const override;
		virtual void				GetAssetTypeExtensions(AZStd::vector<AZStd::string>& extensions) override;
		//////////////////////////////////////////////////////////////////////////////////////////////

		void Register();
		void Unregister();

	protected:
		virtual LoadResult LoadAssetData(	const AZ::Data::Asset<AZ::Data::AssetData>& asset,
											AZStd::shared_ptr<AZ::Data::AssetDataStream> stream,
											const AZ::Data::AssetFilterCB& assetLoadFilterCB);
	};

__LMBRPK_END
