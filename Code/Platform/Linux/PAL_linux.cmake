#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux)
set(pk_package_hash 0624bf8ff38bce909fbedede6647990d527c8f11ada08000cdd535fc8c2ee121)
set(pk_package_id 2pEhBPX228JQwwAQ)

ly_associate_package(PACKAGE_NAME ${package_name} TARGETS PopcornFX PACKAGE_HASH ${pk_package_hash})
pk_download_package_ifn(${package_name} ${pk_package_id})

set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)