#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-mac TARGETS PopcornFX PACKAGE_HASH 31ed133a19117e1aa81a7bd6eb0f76eb9de8c51c45a04c4b571c733ed22a4d58)
set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)