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

#include "PopcornFXStartUpdate.h"
#include "PopcornFXIntegration.h"

#if defined(LMBR_USE_PK)

__LMBRPK_BEGIN

void	PopcornFXStartUpdate::Activate(PopcornFXIntegration *integration)
{
	AZ::TickBus::Handler::BusConnect();
	m_Integration = integration;
}

void	PopcornFXStartUpdate::Deactivate()
{
	AZ::TickBus::Handler::BusDisconnect();
	m_Integration = nullptr;
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

__LMBRPK_END

#endif //LMBR_USE_PK
