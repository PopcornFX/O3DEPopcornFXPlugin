#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-windows TARGETS PopcornFX PACKAGE_HASH d89d368ae39f2afa29b490e38de81d39578c56c7e0e86dc63e00f81f23d94a5b)
set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)
