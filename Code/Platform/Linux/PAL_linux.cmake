#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux)
	set(pk_package_hash e856127f75f9d8db718aa8361be15646e86061a6d343eb3e42366d338095041b)
	set(pk_package_id DXKEqC0oqzPEkVAl)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
	set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux-aarch64)
	set(pk_package_hash 9daa4c7af8afcfac58b7de3157f10fce1220b8d5bf96680d4259a544354c3cf7)
	set(pk_package_id AtLcf1FV1OsVZVQj)
else()
	message(FATAL_ERROR "Unsupported linux architecture ${CMAKE_SYSTEM_PROCESSOR}")
endif()

ly_associate_package(PACKAGE_NAME ${package_name} TARGETS PopcornFX PACKAGE_HASH ${pk_package_hash})
pk_download_package_ifn(${package_name} ${pk_package_id})

set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)