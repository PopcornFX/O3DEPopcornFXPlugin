#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL.
# https://popcornfx.com/popcornfx-community-license/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-mac)
set(pk_package_hash 5fd50d7efaeb5fd96bbef19056c0052e5665ba0034e25a8418586e090537f40f)
set(pk_package_id m02Jv2o3JV9SjtvU)

ly_associate_package(PACKAGE_NAME ${package_name} TARGETS PopcornFX PACKAGE_HASH ${pk_package_hash})
pk_download_package_ifn(${package_name} ${pk_package_id})

set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)