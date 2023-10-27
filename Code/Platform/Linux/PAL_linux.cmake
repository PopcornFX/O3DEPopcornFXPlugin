#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux)
	set(pk_package_hash bdae4a9e32971a96471ebc8bb6b97b588970a0dd07fa6d0fff6d0d1f8a7a41c7)
	set(pk_package_id Ng3RaXuqyAUWAgHy)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
	set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux-aarch64)
	set(pk_package_hash 305a071c6161ca83f2d95d44785cecf5c95852f881d49c74f35dfbd13653ae1b)
	set(pk_package_id ZDEps23sNQA5p7q5)
else()
	message(FATAL_ERROR "Unsupported linux architecture ${CMAKE_SYSTEM_PROCESSOR}")
endif()

ly_associate_package(PACKAGE_NAME ${package_name} TARGETS PopcornFX PACKAGE_HASH ${pk_package_hash})
pk_download_package_ifn(${package_name} ${pk_package_id})

set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)