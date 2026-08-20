#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL.
# https://popcornfx.com/popcornfx-community-license/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-mac)
set(pk_package_hash 6f39f5448c7a9bb048621df9ff8c71b624890218cc88f04b64588180c900e92a)
set(pk_package_id TjszB8AZRapAHoM1)

ly_associate_package(PACKAGE_NAME ${package_name} TARGETS PopcornFX PACKAGE_HASH ${pk_package_hash})
pk_download_package_ifn(${package_name} ${pk_package_id})

set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)