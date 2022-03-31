#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(POPCORNFX_VERSION 2.11.3)
set(POPCORNFX_LICENSE O3DE)

# Uncomment this for O3DE development branch
#add_compile_definitions(O3DE_DEV)

ly_get_list_relative_pal_filename(pal_dir ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME})

include(${pal_dir}/PAL_${PAL_PLATFORM_NAME_LOWERCASE}.cmake) #for PAL_TRAIT_POPCORNFX_SUPPORTED Traits

if(NOT PAL_TRAIT_POPCORNFX_SUPPORTED)
    return()
endif()

ly_add_target(
    NAME PopcornFX-2.11.Static STATIC
    NAMESPACE Gem
    FILES_CMAKE
        popcornfx_autogen_files.cmake
        popcornfx_shared_files.cmake
        ../Assets/popcornfx_asset_files.cmake
    COMPILE_DEFINITIONS
        PUBLIC
            O3DE_USE_PK
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
            AZ::AzFramework
            AZ::AzCore
            AZ::AtomCore
            3rdParty::PopcornFX
            Gem::Atom_Utils.Static
            Gem::Atom_RPI.Public
            Gem::Atom_RHI.Public
            Gem::Atom_RHI.Reflect
            Gem::AtomLyIntegration_CommonFeatures.Static
            Gem::LyShine.Static
            Gem::LmbrCentral
            Gem::ScriptCanvas
            Gem::StartingPointInput.Static
            Gem::EMotionFXStaticLib
            Gem::PhysX.Static
    AUTOGEN_RULES
        *.ScriptCanvasGrammar.xml,ScriptCanvasGrammar_Header.jinja,$path/$fileprefix.generated.h
        *.ScriptCanvasGrammar.xml,ScriptCanvasGrammar_Source.jinja,$path/$fileprefix.generated.cpp
        *.ScriptCanvasNodeable.xml,ScriptCanvasNodeable_Header.jinja,$path/$fileprefix.generated.h
        *.ScriptCanvasNodeable.xml,ScriptCanvasNodeable_Source.jinja,$path/$fileprefix.generated.cpp
)

ly_add_target(
    NAME PopcornFX-2.11 ${PAL_TRAIT_MONOLITHIC_DRIVEN_MODULE_TYPE}
    NAMESPACE Gem
    FILES_CMAKE
        popcornfx_files.cmake
    INCLUDE_DIRECTORIES
        PRIVATE
            Source
            .
        PUBLIC
            Include
    BUILD_DEPENDENCIES
        PRIVATE
            Gem::PopcornFX-2.11.Static
    RUNTIME_DEPENDENCIES
        Gem::LmbrCentral
)

ly_create_alias(NAME PopcornFX-2.11.Clients NAMESPACE Gem TARGETS Gem::PopcornFX-2.11)
#ly_create_alias(NAME PopcornFX-2.11.Servers NAMESPACE Gem TARGETS Gem::PopcornFX-2.11)

if(PAL_TRAIT_BUILD_HOST_TOOLS)
    ly_add_target(
        NAME PopcornFX-2.11.Editor MODULE
        NAMESPACE Gem
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
        BUILD_DEPENDENCIES
            PRIVATE
                AZ::AssetBuilderSDK
                AZ::AzToolsFramework
                3rdParty::PopcornFXEditor
                Gem::PopcornFX-2.11.Static
        RUNTIME_DEPENDENCIES
            Gem::LmbrCentral.Editor
    )

    ly_create_alias(NAME PopcornFX-2.11.Builders NAMESPACE Gem TARGETS Gem::PopcornFX-2.11.Editor)
    ly_create_alias(NAME PopcornFX-2.11.Tools NAMESPACE Gem TARGETS Gem::PopcornFX-2.11.Editor)
endif()
