#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL.
# https://popcornfx.com/popcornfx-community-license/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-windows)
set(pk_package_hash f1414fd1ef7c3c5520638dc11a73793b779a0b3e79821cfe68675f56cdc99b07)
set(pk_package_id EIaMpXXPAbjHqFmy)

ly_associate_package(PACKAGE_NAME ${package_name} TARGETS PopcornFX PACKAGE_HASH ${pk_package_hash})
pk_download_package_ifn(${package_name} ${pk_package_id})

set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)