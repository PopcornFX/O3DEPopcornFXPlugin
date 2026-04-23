#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL.
# https://popcornfx.com/popcornfx-community-license/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-windows)
set(pk_package_hash 080cca86580e22213eaf0e220f99b8bbd43d59d39818cd8057c5183428e5ee33)
set(pk_package_id gdv9f131QL5h4sAo)

ly_associate_package(PACKAGE_NAME ${package_name} TARGETS PopcornFX PACKAGE_HASH ${pk_package_hash})
pk_download_package_ifn(${package_name} ${pk_package_id})

set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)