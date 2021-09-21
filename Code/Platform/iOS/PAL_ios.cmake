#----------------------------------------------------------------------------
# This program is the property of Persistant Studios SARL.
#
# You may not redistribute it and/or modify it under any conditions
# without written permission from Persistant Studios SARL, unless
# otherwise stated in the latest Persistant Studios Code License.
#
# See the Persistant Studios Code License for further details.
#----------------------------------------------------------------------------

set(LY_PACKAGE_SERVER_URLS ${LY_PACKAGE_SERVER_URLS} "https://downloads.popcornfx.com/o3de-packages")
ly_associate_package(PACKAGE_NAME PopcornFX-${POPCORNFX_VERSION}-${POPCORNFX_LICENSE}-ios TARGETS PopcornFX PACKAGE_HASH POPCORNFXPACKAGEHASHTOREPLACE)
