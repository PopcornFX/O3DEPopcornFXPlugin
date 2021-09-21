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

#include <pk_particles/include/ps_mediums.h>
#include "Integration/Render/CameraInfo.h"

__LMBRPK_BEGIN
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
__LMBRPK_END

#endif //LMBR_USE_PK
