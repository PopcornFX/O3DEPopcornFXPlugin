#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

get_property(scriptcanvas_gem_root GLOBAL PROPERTY "@GEMROOT:ScriptCanvas@")

set(PK_OPTIONAL_REGISTER_NODEABLE_FILES 
    ${scriptcanvas_gem_root}/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNodeableRegistry_Header.jinja
    ${scriptcanvas_gem_root}/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNodeableRegistry_Source.jinja
    )

if(DEFINED O3DE_VERSION_MAJOR)
    # O3DE 3.x.x and higher no longer need the registry source files
    # Also include special case to handle bug where SDK 23.05.0 engine version was set to the display version
    if(O3DE_VERSION_MAJOR GREATER_EQUAL 3 AND 
        NOT (O3DE_VERSION_MAJOR EQUAL 23 AND O3DE_VERSION_MINOR EQUAL 05 AND O3DE_VERSION_PATCH EQUAL 0))
        unset(PK_OPTIONAL_REGISTER_NODEABLE_FILES)
    endif()
endif()

set(FILES
    ${scriptcanvas_gem_root}/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNodeable_Header.jinja
    ${scriptcanvas_gem_root}/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNodeable_Source.jinja
    ${PK_OPTIONAL_REGISTER_NODEABLE_FILES}
)

