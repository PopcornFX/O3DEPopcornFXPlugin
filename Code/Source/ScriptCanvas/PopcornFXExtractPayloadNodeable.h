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

#include <Source/ScriptCanvas/PopcornFXExtractPayloadNodeable.generated.h>

//__LMBRPK_BEGIN
namespace LmbrPk
{
    //////////////////////////////////////////////////////////////////////////
    class PopcornFXExtractPayloadNodeable
        : public ScriptCanvas::Nodeable
    {
        SCRIPTCANVAS_NODE(PopcornFXExtractPayloadNodeable)

    public:
        PopcornFXExtractPayloadNodeable() = default;
        virtual ~PopcornFXExtractPayloadNodeable();
        PopcornFXExtractPayloadNodeable(const PopcornFXExtractPayloadNodeable&) = default;
        PopcornFXExtractPayloadNodeable& operator=(const PopcornFXExtractPayloadNodeable&) = default;

    protected:
        void OnDeactivate() override;
    };

}
//__LMBRPK_END
