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

// script canvas
#include <ScriptCanvas/Core/Nodeable.h>
#include <ScriptCanvas/Core/NodeableNode.h>
#include <ScriptCanvas/CodeGen/NodeableCodegen.h>
#include <PopcornFX/PopcornFXBus.h>

#include <Source/ScriptCanvas/PopcornFXBroadcastNodeable.generated.h>

//__LMBRPK_BEGIN
namespace LmbrPk
{

    //////////////////////////////////////////////////////////////////////////
    class PopcornFXBroadcastNodeable
        : public ScriptCanvas::Nodeable
        , protected PopcornFX::PopcornFXEmitterComponentEventsBus::Handler
    {
        SCRIPTCANVAS_NODE(PopcornFXBroadcastNodeable)

    public:
        PopcornFXBroadcastNodeable() = default;
        virtual ~PopcornFXBroadcastNodeable();
        PopcornFXBroadcastNodeable(const PopcornFXBroadcastNodeable&) = default;
        PopcornFXBroadcastNodeable& operator=(const PopcornFXBroadcastNodeable&) = default;

    protected:
        void OnDeactivate() override;

        //////////////////////////////////////////////////////////////////////////
        /// PopcornFX::PopcornFXEventsBus::Handler
        void OnFxCreated(const AZ::EntityId&) override;
        void OnFxBroadcastEvent(const AZ::EntityId&, PopcornFX::SBroadcastParams* params) override;

        AZStd::string   m_EventName;
    };

}
//__LMBRPK_END
