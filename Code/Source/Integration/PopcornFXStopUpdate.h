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

#include <AzCore/Component/TickBus.h>

__LMBRPK_BEGIN

class PopcornFXIntegration;

	class PopcornFXStopUpdate
		: protected AZ::TickBus::Handler
	{
	public:
		void	Activate(PopcornFXIntegration *integration);
		void	Deactivate();

	protected:
		virtual int					GetTickOrder() override;
		virtual void				OnTick(float deltaTime, AZ::ScriptTimePoint time) override;

		PopcornFXIntegration		*m_Integration = nullptr;
	};

__LMBRPK_END

#endif //LMBR_USE_PK

