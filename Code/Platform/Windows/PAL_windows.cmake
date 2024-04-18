#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
set(package_name PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-windows)
set(pk_package_hash 147aff703bd6e80316eeacb751b24568d87cc4a6eae85cb3652ca6d5de4d97bd)
set(pk_package_id bM0v9F8oWacWPWF6)

ly_associate_package(PACKAGE_NAME ${package_name} TARGETS PopcornFX PACKAGE_HASH ${pk_package_hash})
pk_download_package_ifn(${package_name} ${pk_package_id})

set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)