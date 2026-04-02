//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------

#include "PopcornFX_precompiled.h"

#include "PopcornFXStartUpdate.h"
#include "PopcornFXIntegration.h"

#if defined(O3DE_USE_PK)

namespace PopcornFX {

void	PopcornFXStartUpdate::Activate(PopcornFXIntegration *integration)
{
	AZ::TickBus::Handler::BusConnect();
	m_Integration = integration;
}

void	PopcornFXStartUpdate::Deactivate()
{
	AZ::TickBus::Handler::BusDisconnect();
	m_Integration = null;
}

int		PopcornFXStartUpdate::GetTickOrder()
{
	return AZ::TICK_DEFAULT + 1; // For now we start the update at default + 1 to avoid getting some behavior changes
}

void	PopcornFXStartUpdate::OnTick(float deltaTime, AZ::ScriptTimePoint time)
{
	(void)time;
	m_Integration->StartUpdate(deltaTime);
}

}

#endif //O3DE_USE_PK
