#----------------------------------------------------------------------------
# This program is the property of Persistant Studios SARL.
#
# You may not redistribute it and/or modify it under any conditions
# without written permission from Persistant Studios SARL, unless
# otherwise stated in the latest Persistant Studios Code License.
#
# See the Persistant Studios Code License for further details.
#----------------------------------------------------------------------------

if (${POPCORNFX_AUTOGEN_SCRIPTCANVAS})
    set(FILES
        ${LY_ROOT_FOLDER}/Gems/ScriptCanvas/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasGrammar_Header.jinja
        ${LY_ROOT_FOLDER}/Gems/ScriptCanvas/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasGrammar_Source.jinja
        ${LY_ROOT_FOLDER}/Gems/ScriptCanvas/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNodeable_Header.jinja
        ${LY_ROOT_FOLDER}/Gems/ScriptCanvas/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNodeable_Source.jinja
)
else()
    set(FILES
        Source/ScriptCanvas/PopcornFXBroadcastNodeable.generated.h
        Source/ScriptCanvas/PopcornFXBroadcastNodeable.generated.cpp
        Source/ScriptCanvas/PopcornFXExtractPayloadNodeable.generated.h
        Source/ScriptCanvas/PopcornFXExtractPayloadNodeable.generated.cpp
)
endif()
