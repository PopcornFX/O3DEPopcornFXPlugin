//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <AzCore/std/string/string.h>
#include <AzCore/Asset/AssetCommon.h>
#include <PopcornFX/PopcornFXBus.h>

#if defined(O3DE_USE_PK)
#include <pk_particles/include/ps_effect.h>
#else
namespace PopcornFX
{
	class CParticleEffect;
	typedef CParticleEffect*	PParticleEffect;
	class CBaseObjectFile;
	typedef CBaseObjectFile*	PBaseObjectFile;

	namespace HBO
	{
		class CContext;
	}
}
#endif

namespace PopcornFX {

	class PopcornFXAsset
		: public AZ::Data::AssetData
	{
	public:

		AZ_RTTI(PopcornFXAsset, PopcornFX::AssetTypeId, AZ::Data::AssetData);
		AZ_CLASS_ALLOCATOR(PopcornFXAsset, AZ::SystemAllocator, 0);

		PParticleEffect	m_Effect = null;
		PBaseObjectFile	m_File = null;
		HBO::CContext	*m_Context = null;
		AZStd::string	m_Path;
	};

	class PopcornFXAssetPKVF
		: public AZ::Data::AssetData
	{
	public:

		AZ_RTTI(PopcornFXAssetPKVF, "{0F8F0BF4-C87E-430D-AD75-EC5A9A631F06}", AZ::Data::AssetData);
		AZ_CLASS_ALLOCATOR(PopcornFXAssetPKVF, AZ::SystemAllocator, 0);
	};

}
