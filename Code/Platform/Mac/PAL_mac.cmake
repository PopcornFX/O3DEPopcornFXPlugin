#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-mac TARGETS PopcornFX PACKAGE_HASH 2aa016ea1b8a36a9b69cfbaf1d14cc7ac379953fc621a0ebb0d795978caeafe3)
set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)