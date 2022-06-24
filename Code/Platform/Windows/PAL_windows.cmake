#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-windows TARGETS PopcornFX PACKAGE_HASH d77eee7f46751ba5285ff7e96f041c453db79caccca0f7a68d57a7a74a7d14fd)
set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)
