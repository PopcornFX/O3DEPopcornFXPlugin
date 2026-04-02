//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------

#pragma once

#if defined(O3DE_USE_PK)

namespace PopcornFX
{
	PK_FORWARD_DECLARE(ParticleMediumCollection);
}

namespace PopcornFX {
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
}

#endif //O3DE_USE_PK
