//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------

#pragma once

#if defined(O3DE_USE_PK)

namespace PopcornFX {

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

}

#endif //O3DE_USE_PK

