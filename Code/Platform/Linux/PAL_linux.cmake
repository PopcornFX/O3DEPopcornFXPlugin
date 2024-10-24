#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux)
	set(pk_package_hash 77420e60a2f83e2965cd0ff0955141e4626a4bf0ee20b7d2ed09e1c4a12bf600)
	set(pk_package_id V1cOibNwsQpr1zPe)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
	set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux-aarch64)
	set(pk_package_hash 44e9f8e443424f748a02c930119ee2c38d69b2f22d989c6ab68ee470850db2bc)
	set(pk_package_id KGX0lcH92TLXi1sE)
else()
	message(FATAL_ERROR "Unsupported linux architecture ${CMAKE_SYSTEM_PROCESSOR}")
endif()

ly_associate_package(PACKAGE_NAME ${package_name} TARGETS PopcornFX PACKAGE_HASH ${pk_package_hash})
pk_download_package_ifn(${package_name} ${pk_package_id})

set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)