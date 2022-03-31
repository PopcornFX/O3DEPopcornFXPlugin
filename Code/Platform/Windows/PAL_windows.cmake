#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-windows TARGETS PopcornFX PACKAGE_HASH 6a8aefd3c46f7d0d60db6ae19ec09570e508873cc0e669b2e40e9de8c1772f88)
set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)
