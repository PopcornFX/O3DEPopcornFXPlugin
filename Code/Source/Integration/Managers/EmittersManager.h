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

#include <PopcornFX/PopcornFXBus.h>

__LMBRPK_BEGIN
//----------------------------------------------------------------------------

class CEmittersManager
{
public:
	void				Reset();

	AZ::EntityId		SpawnEmitterEntityAtLocation(const AZStd::string& path, AZ::Transform transform, bool start);
	StandaloneEmitter	*SpawnEffectById(const AZ::Data::AssetId &assetId, const PopcornFX::SpawnParams &spawnParams);
	bool				IsEffectAlive(StandaloneEmitter *emitter);
	void				DestroyEffect(StandaloneEmitter *emitter);

private:
	AZStd::list<StandaloneEmitter>	m_Emitters;

};

//----------------------------------------------------------------------------
__LMBRPK_END

#endif //LMBR_USE_PK
