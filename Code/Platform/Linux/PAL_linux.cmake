#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-linux TARGETS PopcornFX PACKAGE_HASH ed1477588501fe383e52b6a7bd866e44060afbfaac3e9e3e7f7d7d8956039740)
set(PAL_TRAIT_POPCORNFX_SUPPORTED TRUE)