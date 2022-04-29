#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-windows TARGETS PopcornFX PACKAGE_HASH 52d36ddddf77a5eb9c288c0a51d0af4059cc6935b7bf3aeb0f8d38527a3993e5)
set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)
