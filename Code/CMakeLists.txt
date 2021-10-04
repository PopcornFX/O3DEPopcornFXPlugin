#----------------------------------------------------------------------------
# This program is the property of Persistant Studios SARL.
#
# You may not redistribute it and/or modify it under any conditions
# without written permission from Persistant Studios SARL, unless
# otherwise stated in the latest Persistant Studios Code License.
#
# See the Persistant Studios Code License for further details.
#----------------------------------------------------------------------------
#

set(POPCORNFX_VERSION 2.9.13)
set(POPCORNFX_LICENSE O3DE)

# Uncomment this for O3DE development branch
add_compile_definitions(O3DE_DEV)

ly_get_list_relative_pal_filename(pal_dir ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME})

include(${pal_dir}/PAL_${PAL_PLATFORM_NAME_LOWERCASE}.cmake)

set(POPCORNFX_AUTOGEN_SCRIPTCANVAS OFF)

if (${POPCORNFX_AUTOGEN_SCRIPTCANVAS})
    set(POCPORNFX_AUTOGEN_SCRIPTCANVAS_FILES
        *.ScriptCanvasGrammar.xml,ScriptCanvasGrammar_Header.jinja,$path/$fileprefix.generated.h
        *.ScriptCanvasGrammar.xml,ScriptCanvasGrammar_Source.jinja,$path/$fileprefix.generated.cpp
        *.ScriptCanvasNodeable.xml,ScriptCanvasNodeable_Header.jinja,$path/$fileprefix.generated.h
        *.ScriptCanvasNodeable.xml,ScriptCanvasNodeable_Source.jinja,$path/$fileprefix.generated.cpp
    )
endif()

ly_add_target(
    NAME PopcornFX.Static STATIC
    NAMESPACE Gem
    FILES_CMAKE
        popcornfx_autogen_files.cmake
        popcornfx_shared_files.cmake
        ../Assets/popcornfx_asset_files.cmake
    COMPILE_DEFINITIONS
        PRIVATE
            LMBR_USE_PK
            PK_USE_PHYSX
            PK_USE_EMOTIONFX
            PK_USE_STARTINGPOINTINPUT
    INCLUDE_DIRECTORIES
        PRIVATE
            Source
            .
        PUBLIC
            Include
    BUILD_DEPENDENCIES
        PUBLIC
            3rdParty::PopcornFX
            Gem::LmbrCentral
            Gem::AtomLyIntegration_CommonFeatures.Static
            Gem::ScriptCanvas
            Gem::StartingPointInput
            Gem::EMotionFXStaticLib
            Gem::PhysX.Static
            3rdParty::PopcornFX
            AZ::AtomCore
            Gem::Atom_Utils.Static
            Gem::Atom_RPI.Public
            Gem::Atom_RHI.Public
            Gem::Atom_RHI.Reflect
    AUTOGEN_RULES
        ${POCPORNFX_AUTOGEN_SCRIPTCANVAS_FILES}
)

ly_add_target(
    NAME PopcornFX ${PAL_TRAIT_MONOLITHIC_DRIVEN_MODULE_TYPE}
    NAMESPACE Gem
    OUTPUT_NAME Gem.PopcornFX.v${POPCORNFX_VERSION}
    FILES_CMAKE
        popcornfx_files.cmake
    INCLUDE_DIRECTORIES
        PRIVATE
            Source
            .
        PUBLIC
            Include
    COMPILE_DEFINITIONS
        PRIVATE
            LMBR_USE_PK
            PK_USE_PHYSX
            PK_USE_EMOTIONFX
            PK_USE_STARTINGPOINTINPUT
    BUILD_DEPENDENCIES
        PRIVATE
            Gem::ScriptCanvas
            Gem::StartingPointInput
            Gem::LmbrCentral
            3rdParty::PopcornFX
            Gem::PopcornFX.Static
            Gem::LyShine.Static
)

ly_create_alias(NAME PopcornFX.Clients NAMESPACE Gem TARGETS Gem::PopcornFX)
ly_create_alias(NAME PopcornFX.Servers NAMESPACE Gem TARGETS Gem::PopcornFX)

if(PAL_TRAIT_BUILD_HOST_TOOLS)
    ly_add_target(
        NAME PopcornFX.Editor MODULE
        NAMESPACE Gem
        OUTPUT_NAME Gem.PopcornFX.Editor.v${POPCORNFX_VERSION}
        AUTOUIC
        AUTOMOC
        AUTORCC
        FILES_CMAKE
            popcornfx_editor_files.cmake
        INCLUDE_DIRECTORIES
            PRIVATE
                Source
                .
            PUBLIC
                Include
        COMPILE_DEFINITIONS
            PRIVATE
                POPCORNFX_EDITOR
                LMBR_USE_PK
                PK_USE_PHYSX
                PK_USE_EMOTIONFX
                PK_USE_STARTINGPOINTINPUT
        BUILD_DEPENDENCIES
            PRIVATE
                Gem::ScriptCanvas
                Gem::StartingPointInput
                Gem::LmbrCentral
                3rdParty::PopcornFX
                3rdParty::PopcornFXEditor
                Gem::PopcornFX.Static
                AZ::AssetBuilderSDK
                AZ::AzFramework
                AZ::AzToolsFramework
                Gem::LyShine.Static
    )

    ly_create_alias(NAME PopcornFX.Builders NAMESPACE Gem TARGETS Gem::PopcornFX.Editor)
    ly_create_alias(NAME PopcornFX.Tools NAMESPACE Gem TARGETS Gem::PopcornFX.Editor)
endif()
