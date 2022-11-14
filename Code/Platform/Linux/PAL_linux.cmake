#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux TARGETS PopcornFX PACKAGE_HASH 7d68c9e8116e37e2fcb2c4895ce6069c510ffb0bef9e265c63a8acae1051bd54)
set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)