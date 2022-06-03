#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux TARGETS PopcornFX PACKAGE_HASH 3d1f32a8be5fec922ab7bf7be917e7b3fffcbf84a04a10eccddd5446eaf3f755)
set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)