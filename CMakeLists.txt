#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

# O3DE version 1.2.0 (23.05.0) and higher provide version major/minor/patch global properties
get_property(O3DE_VERSION_MAJOR GLOBAL PROPERTY O3DE_VERSION_MAJOR)
get_property(O3DE_VERSION_MINOR GLOBAL PROPERTY O3DE_VERSION_MINOR)
get_property(O3DE_VERSION_PATCH GLOBAL PROPERTY O3DE_VERSION_PATCH)

set(gem_path ${CMAKE_CURRENT_LIST_DIR})
set(gem_json ${gem_path}/gem.json)
o3de_restricted_path(${gem_json} gem_restricted_path gem_parent_relative_path)

o3de_pal_dir(pal_dir ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME} "${gem_restricted_path}" "${gem_path}" "${gem_parent_relative_path}")

# Now that we have the platform abstraction layer (PAL) folder for this folder, thats where we will find the
# project cmake for this platform.
include(${pal_dir}/${PAL_PLATFORM_NAME_LOWERCASE}_gem.cmake)

function(pk_patch_file old_string new_string path)
    unset(file_content)
    ly_file_read(${path} file_content)
    string(REPLACE ${old_string} ${new_string} file_content "${file_content}")
    file(WRITE ${path} "${file_content}")
endfunction()

if(DEFINED O3DE_VERSION_MAJOR)
    # Between engine version 1.2.0 -> 2.2.0 there is an issue that prevents
    # PopcornFX from being added to the .setreg files for loading .dlls at runtime
    # In addition there is a bug where the engine version in the SDK is the display version
    # instead of matching the version field in the source code
    if((O3DE_VERSION_MAJOR EQUAL 23 AND O3DE_VERSION_MINOR EQUAL 05 AND O3DE_VERSION_PATCH EQUAL 0) OR 
       (O3DE_VERSION_MAJOR EQUAL 1 AND O3DE_VERSION_MINOR GREATER_EQUAL 2) OR 
       (O3DE_VERSION_MAJOR EQUAL 2 AND O3DE_VERSION_MINOR LESS_EQUAL 1))
        message(VERBOSE "Activating PopcornFX gem for O3DE ${O3DE_VERSION_MAJOR}.${O3DE_VERSION_MINOR}.${O3DE_VERSION_PATCH}")
        ly_enable_gems(GEMS PopcornFX)
    endif()

    # Special case to handle bug where SDK 23.05.0 engine version was set to the display version
    if(O3DE_VERSION_MAJOR EQUAL 23 AND O3DE_VERSION_MINOR EQUAL 05)
        set(O3DE_VERSION_MAJOR 1)
        set(O3DE_VERSION_MINOR 2)
    endif()

    # Apply patches here IFN
    if(O3DE_VERSION_MAJOR GREATER_EQUAL 4 OR (O3DE_VERSION_MAJOR EQUAL 2 AND O3DE_VERSION_MINOR GREATER_EQUAL 5)) # Development or 25.10+
        pk_patch_file("PK_O3DE_LIT_MULTI_VIEW 0" "PK_O3DE_LIT_MULTI_VIEW 1" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
        pk_patch_file("PK_O3DE_SRG_ALL 0" "PK_O3DE_SRG_ALL 1" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
        pk_patch_file("PK_O3DE_MULTI_VIEW_SPECULAR 0" "PK_O3DE_MULTI_VIEW_SPECULAR 1" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
        pk_patch_file("PK_O3DE_SEPARATE_LIGHTING 0" "PK_O3DE_SEPARATE_LIGHTING 1" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
    elseif(O3DE_VERSION_MAJOR EQUAL 2 AND O3DE_VERSION_MINOR EQUAL 4) # 25.05
        pk_patch_file("PK_O3DE_LIT_MULTI_VIEW 0" "PK_O3DE_LIT_MULTI_VIEW 1" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
        pk_patch_file("PK_O3DE_SRG_ALL 0" "PK_O3DE_SRG_ALL 1" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
        pk_patch_file("PK_O3DE_MULTI_VIEW_SPECULAR 1" "PK_O3DE_MULTI_VIEW_SPECULAR 0" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
        pk_patch_file("PK_O3DE_SEPARATE_LIGHTING 1" "PK_O3DE_SEPARATE_LIGHTING 0" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
    elseif (O3DE_VERSION_MAJOR EQUAL 2 AND O3DE_VERSION_MINOR EQUAL 3) # 24.09
        pk_patch_file("PK_O3DE_LIT_MULTI_VIEW 0" "PK_O3DE_LIT_MULTI_VIEW 1" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
        pk_patch_file("PK_O3DE_SRG_ALL 1" "PK_O3DE_SRG_ALL 0" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
        pk_patch_file("PK_O3DE_MULTI_VIEW_SPECULAR 1" "PK_O3DE_MULTI_VIEW_SPECULAR 0" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
        pk_patch_file("PK_O3DE_SEPARATE_LIGHTING 1" "PK_O3DE_SEPARATE_LIGHTING 0" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
    else()
        pk_patch_file("PK_O3DE_LIT_MULTI_VIEW 1" "PK_O3DE_LIT_MULTI_VIEW 0" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
        pk_patch_file("PK_O3DE_SRG_ALL 1" "PK_O3DE_SRG_ALL 0" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
        pk_patch_file("PK_O3DE_MULTI_VIEW_SPECULAR 1" "PK_O3DE_MULTI_VIEW_SPECULAR 0" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
        pk_patch_file("PK_O3DE_SEPARATE_LIGHTING 1" "PK_O3DE_SEPARATE_LIGHTING 0" ${gem_path}/Assets/shaders/Common/PopcornOptions.azsli)
    endif()
endif()

add_subdirectory(Code)
