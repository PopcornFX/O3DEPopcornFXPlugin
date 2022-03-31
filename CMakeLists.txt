#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(O3DE_DEVELOPMENT ON)

set(gem_path ${CMAKE_CURRENT_LIST_DIR})
set(gem_json ${gem_path}/gem.json)
o3de_restricted_path(${gem_json} gem_restricted_path gem_parent_relative_path)

if (${O3DE_DEVELOPMENT})
    o3de_pal_dir(pal_dir ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME} "${gem_restricted_path}" "${gem_path}" "${gem_parent_relative_path}")
else ()
    o3de_read_json_key(gem_name ${gem_json} "gem_name")
    ly_get_list_relative_pal_filename(pal_dir ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME} "${gem_restricted_path}" ${gem_path} ${gem_name})
endif ()

# Now that we have the platform abstraction layer (PAL) folder for this folder, thats where we will find the
# project cmake for this platform.
include(${pal_dir}/${PAL_PLATFORM_NAME_LOWERCASE}_gem.cmake)

add_subdirectory(Code)
