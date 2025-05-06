#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux)
	set(pk_package_hash f20106888d9925a9e779572cda42a3ec7b42b777da11f2526fe14e17fceb8f02)
	set(pk_package_id 0Ugz5FvNWP6dDBhd)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
	set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux-aarch64)
	set(pk_package_hash edb22b6ed0c268da43cc43e5af79c17d49d85f9aa766c076b10afc481478f13b)
	set(pk_package_id ZqkDuypvcE8sBQbQ)
else()
	message(FATAL_ERROR "Unsupported linux architecture ${CMAKE_SYSTEM_PROCESSOR}")
endif()

ly_associate_package(PACKAGE_NAME ${package_name} TARGETS PopcornFX PACKAGE_HASH ${pk_package_hash})
pk_download_package_ifn(${package_name} ${pk_package_id})

set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)