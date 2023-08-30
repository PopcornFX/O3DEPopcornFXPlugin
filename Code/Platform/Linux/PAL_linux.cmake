#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux)
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	set(pk_package_hash POPCORNFXPACKAGEHASHTOREPLACE)
	set(pk_package_id POPCORNFXPACKAGEIDTOREPLACE)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
	set(pk_package_hash 0017707d58f80cac493048386a6d703ccd498515633430da9c63a4ad5a1ed564)
	set(pk_package_id POPCORNFXPACKAGEIDTOREPLACE_ARM64)
else()
	message(FATAL_ERROR "Unsupported linux architecture ${CMAKE_SYSTEM_PROCESSOR}")
endif()

ly_associate_package(PACKAGE_NAME ${package_name} TARGETS PopcornFX PACKAGE_HASH ${pk_package_hash})
pk_download_package_ifn(${package_name} ${pk_package_id})

set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)