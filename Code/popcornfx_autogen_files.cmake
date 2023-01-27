#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

get_property(scriptcanvas_gem_root GLOBAL PROPERTY "@GEMROOT:ScriptCanvas@")

set(FILES
    ${scriptcanvas_gem_root}/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNodeable_Header.jinja
    ${scriptcanvas_gem_root}/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNodeable_Source.jinja
    ${scriptcanvas_gem_root}/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNodeableRegistry_Header.jinja
    ${scriptcanvas_gem_root}/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNodeableRegistry_Source.jinja
)

