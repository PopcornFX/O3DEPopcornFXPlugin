#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-mac TARGETS PopcornFX PACKAGE_HASH daebc59177c61ac79b076d51f7371d20c2210280fa551629043b9302a2127c9d)
set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)