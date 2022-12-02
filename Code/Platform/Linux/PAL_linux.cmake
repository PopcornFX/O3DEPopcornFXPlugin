#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux TARGETS PopcornFX PACKAGE_HASH f1da9177f4f13a8424bffd9f985537be664bfcf340f7b1a02d9a665c3c73c1bc)
set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)