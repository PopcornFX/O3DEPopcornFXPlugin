//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------

#pragma once

#if defined(O3DE_USE_PK)

#include <PopcornFX/PopcornFXBus.h>

namespace PopcornFX {
//----------------------------------------------------------------------------

class CEmittersManager
{
public:
	void				Reset();

	AZ::EntityId		SpawnEmitterEntityAtLocation(const AZStd::string &path, const AZ::Transform &transform, bool start);
	StandaloneEmitter	*SpawnEffectById(const AZ::Data::AssetId &assetId, const PopcornFX::SpawnParams &spawnParams);
	bool				IsEffectAlive(StandaloneEmitter *emitter);
	void				DestroyEffect(StandaloneEmitter *emitter);

private:
	AZStd::list<StandaloneEmitter>	m_Emitters;

};

//----------------------------------------------------------------------------
}

#endif //O3DE_USE_PK
