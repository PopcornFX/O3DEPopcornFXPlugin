//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------

#pragma once

#if defined(O3DE_USE_PK)

#include <pk_particles/include/ps_mediums.h>
#include "Integration/Render/CameraInfo.h"

namespace PopcornFX {
//----------------------------------------------------------------------------

class CSceneViewsManager
{
public:
	void	Activate();
	void	Deactivate();
	void	Update(CParticleMediumCollection *mediumCollection);

	const SSceneViews	*SceneViews() { return &m_SceneViews; }

	SSceneViews					m_SceneViews;
};

//----------------------------------------------------------------------------
}

#endif //O3DE_USE_PK
