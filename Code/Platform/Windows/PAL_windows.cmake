#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-windows TARGETS PopcornFX PACKAGE_HASH 719e3d6b3f1fe25c53ec80e6b274fd064f6077af232fa4d1baccd587b5f52e0f)
set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)
