#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(PK_O3DE_DEVELOPMENT ON)

if (${INSTALLED_ENGINE})
    o3de_read_json_key(o3de_build_number ${LY_ROOT_FOLDER}/engine.json "O3DEBuildNumber")
    if (o3de_build_number STREQUAL "" OR o3de_build_number STREQUAL "0")
        message(FATAL_ERROR "Invalid O3DEBuildNumber from 'engine.json'")
    endif()
else()
    set(o3de_build_number "2205.0")
    #set(o3de_build_number "2111.2")
    #set(o3de_build_number "2111.1")
endif()

string(REPLACE "." ";" o3de_version_list ${o3de_build_number})
list(GET o3de_version_list 0 PK_O3DE_MAJOR_VERSION)
list(GET o3de_version_list 1 PK_O3DE_PATCH_VERSION)

if (${PK_O3DE_DEVELOPMENT})
    MATH(EXPR PK_O3DE_MAJOR_VERSION "${PK_O3DE_MAJOR_VERSION}+1")
endif()

set(gem_path ${CMAKE_CURRENT_LIST_DIR})
set(gem_json ${gem_path}/gem.json)
o3de_restricted_path(${gem_json} gem_restricted_path gem_parent_relative_path)

if (PK_O3DE_MAJOR_VERSION GREATER_EQUAL 2205)
    o3de_pal_dir(pal_dir ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME} "${gem_restricted_path}" "${gem_path}" "${gem_parent_relative_path}")
else ()
    o3de_read_json_key(gem_name ${gem_json} "gem_name")
    ly_get_list_relative_pal_filename(pal_dir ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME} "${gem_restricted_path}" ${gem_path} ${gem_name})
endif ()

# Now that we have the platform abstraction layer (PAL) folder for this folder, thats where we will find the
# project cmake for this platform.
include(${pal_dir}/${PAL_PLATFORM_NAME_LOWERCASE}_gem.cmake)

add_subdirectory(Code)
