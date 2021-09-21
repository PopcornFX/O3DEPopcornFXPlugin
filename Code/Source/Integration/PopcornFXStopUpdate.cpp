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

#include "PopcornFXStopUpdate.h"
#include "PopcornFXIntegration.h"

#if defined(LMBR_USE_PK)

__LMBRPK_BEGIN

void	PopcornFXStopUpdate::Activate(PopcornFXIntegration *integration)
{
	AZ::TickBus::Handler::BusConnect();
	m_Integration = integration;
}

void	PopcornFXStopUpdate::Deactivate()
{
	AZ::TickBus::Handler::BusDisconnect();
	m_Integration = nullptr;
}

int		PopcornFXStopUpdate::GetTickOrder()
{
	return AZ::TICK_LAST;
}

void	PopcornFXStopUpdate::OnTick(float deltaTime, AZ::ScriptTimePoint time)
{
	(void)deltaTime; (void)time;
	m_Integration->StopUpdate();
}

__LMBRPK_END

#endif //LMBR_USE_PK
