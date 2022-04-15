#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-mac TARGETS PopcornFX PACKAGE_HASH 074c6de2b1faa7ea27b04bc5966ecba6040117bbf7395a1eda0e67ea8c076bbc)
set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)