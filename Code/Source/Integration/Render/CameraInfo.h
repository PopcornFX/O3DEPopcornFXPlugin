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

__LMBRPK_BEGIN

struct SSceneViews
{
	struct SView
	{
		CFloat4x4		m_ViewMatrix;
		CFloat4x4		m_InvViewMatrix;
		CFloat4x4		m_ViewProjMatrix;
	};

	PopcornFX::TArray<SView>	m_Views;
};

__LMBRPK_END

#endif //LMBR_USE_PK

