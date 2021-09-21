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

#include "PopcornFXBroadcastNodeable.h"

#include <AzCore/Component/Entity.h>
#include <ScriptCanvas/Libraries/Libraries.h>

#include "Integration/PopcornFXIntegrationBus.h"

__LMBRPK_BEGIN

    PopcornFXBroadcastNodeable::~PopcornFXBroadcastNodeable()
    {
        PopcornFX::PopcornFXEmitterComponentEventsBus::Handler::BusDisconnect();
    }
    
    void PopcornFXBroadcastNodeable::ConnectEvent(AZStd::string eventName, AZ::EntityId entityId)
    {
        PopcornFX::PopcornFXEmitterComponentEventsBus::Handler::BusDisconnect();

        if (entityId.IsValid())
        {
            m_EventName = eventName;
            PopcornFX::PopcornFXEmitterComponentEventsBus::Handler::BusConnect(entityId);
        }
    }
    
    void PopcornFXBroadcastNodeable::OnDeactivate()
    {
        PopcornFX::PopcornFXEmitterComponentEventsBus::Handler::BusDisconnect();
    }

    void PopcornFXBroadcastNodeable::OnFxCreated(const AZ::EntityId& entityId)
    {
        bool ok = false;
        LmbrPk::PopcornFXIntegrationBus::BroadcastResult(ok, &LmbrPk::PopcornFXIntegrationBus::Handler::RegisterToBroadcast, entityId, m_EventName);
        AZ_Error("PopcornFX", ok, "Unable to register event %s to broadcast.", m_EventName.c_str());
    }

    void PopcornFXBroadcastNodeable::OnFxBroadcastEvent(const AZ::EntityId&, PopcornFX::SBroadcastParams* params)
    {
        SCRIPT_CANVAS_PERFORMANCE_SCOPE_LATENT(GetScriptCanvasId(), GetAssetId());

        if (params->m_EventName == m_EventName)
            CallBroadcast();
    }

__LMBRPK_END
