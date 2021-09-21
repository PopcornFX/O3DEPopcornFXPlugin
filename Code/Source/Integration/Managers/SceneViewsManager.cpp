//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFX_precompiled.h"
#include "SceneViewsManager.h"

#if defined(LMBR_USE_PK)

#include <Atom/RPI.Public/ViewportContextBus.h>
#include <Atom/RPI.Public/ViewportContext.h>
#include <Atom/RPI.Public/View.h>
#include <pk_geometrics/include/ge_coordinate_frame.h>
#include "Integration/PopcornFXUtils.h"

__LMBRPK_BEGIN
//----------------------------------------------------------------------------

void	CSceneViewsManager::Activate()
{
	m_SceneViews.m_Views.Clear();
}

void	CSceneViewsManager::Deactivate()
{
	m_SceneViews.m_Views.Clear();
}

void	CSceneViewsManager::Update(CParticleMediumCollection *mediumCollection)
{
	if (m_SceneViews.m_Views.Empty())
		m_SceneViews.m_Views.PushBack();

	auto	atomViewportRequests = AZ::Interface<AZ::RPI::ViewportContextRequestsInterface>::Get();
	auto	context = atomViewportRequests->GetDefaultViewportContext();
	auto	currentView = context->GetDefaultView();
	auto	currentViewSize = context->GetViewportSize();

	const CUint2		viewportDim(currentViewSize.m_width, currentViewSize.m_height);
	const AZ::Matrix4x4	mProj = currentView->GetWorldToClipMatrix();
	const AZ::Matrix4x4	mView = currentView->GetWorldToViewMatrix();
	const CFloat4x4		camW2V = ToPk(mView);
	const CFloat4x4		camW2P = ToPk(mProj);

	SSceneViews::SView& view = m_SceneViews.m_Views[0];
	view.m_ViewMatrix = camW2V;
	view.m_InvViewMatrix = camW2V.Inverse();
	view.m_ViewProjMatrix = camW2P;

	CFloat4x4	toZUp;
	PopcornFX::CCoordinateFrame::BuildTransitionFrame(PopcornFX::ECoordinateFrame::Frame_RightHand_Y_Up, PopcornFX::ECoordinateFrame::Frame_RightHand_Z_Up, toZUp);
	mediumCollection->UpdateView(0, camW2V * toZUp, camW2P, viewportDim);
}

//----------------------------------------------------------------------------
__LMBRPK_END

#endif //LMBR_USE_PK
