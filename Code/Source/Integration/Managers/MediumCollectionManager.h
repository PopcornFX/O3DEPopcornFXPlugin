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

namespace PopcornFX
{
	PK_FORWARD_DECLARE(ParticleMediumCollection);
}

__LMBRPK_BEGIN
//----------------------------------------------------------------------------

class CSceneInterface;

class CMediumCollectionManager
{
public:
	bool	Activate(CSceneInterface *sceneInterface);
	void	Deactivate();
	void	Reset();
	void	StartUpdate(float deltaTime);
	void	StopUpdate();

	PopcornFX::CParticleMediumCollection	*MediumCollection() { return m_ParticleMediumCollection; }

private:
	PopcornFX::CParticleMediumCollection	*m_ParticleMediumCollection = null;
};

//----------------------------------------------------------------------------
__LMBRPK_END

#endif //LMBR_USE_PK
