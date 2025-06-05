#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux)
	set(pk_package_hash fd83a54cd41f58c71ed1c70eecdb5257623a28191518b0be671d1e4ac55f989b)
	set(pk_package_id Ut4bTBFqkcXrNKBV)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
	set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux-aarch64)
	set(pk_package_hash fb706aa45c84cd72c02298975c5f969173c6581a9621a334cb85de005eabb4cd)
	set(pk_package_id u1Zfo5OspHzSvgcA)
else()
	message(FATAL_ERROR "Unsupported linux architecture ${CMAKE_SYSTEM_PROCESSOR}")
endif()

ly_associate_package(PACKAGE_NAME ${package_name} TARGETS PopcornFX PACKAGE_HASH ${pk_package_hash})
pk_download_package_ifn(${package_name} ${pk_package_id})

set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)