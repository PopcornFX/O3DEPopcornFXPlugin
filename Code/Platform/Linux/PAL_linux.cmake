#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux TARGETS PopcornFX PACKAGE_HASH 8f92717d63eea6a7a25dfc68d6a40beb794e9d01a14e5f0dbeac783b3d9e43b2)
set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)