#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL.
# https://popcornfx.com/popcornfx-community-license/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-mac)
set(pk_package_hash f74eded7dad4f65bde51ef3d008b458ced3001a34f9d2dfd4d74f1036287bd6c)
set(pk_package_id 00uvBvtO7BX4fEda)

ly_associate_package(PACKAGE_NAME ${package_name} TARGETS PopcornFX PACKAGE_HASH ${pk_package_hash})
pk_download_package_ifn(${package_name} ${pk_package_id})

set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)