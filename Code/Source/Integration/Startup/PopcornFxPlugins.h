//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#pragma once

#if defined(LMBR_USE_PK)

__LMBRPK_BEGIN

    enum    ERuntimePlugin
    {
        EPlugin_CompilerBackendVM = 1 << 0,
        EPlugin_ImageCodecDDS = 1 << 1,
        EPlugin_ImageCodecPNG = 1 << 2,
        EPlugin_ImageCodecJPG = 1 << 3,
        EPlugin_ImageCodecTGA = 1 << 4,
        EPlugin_ImageCodecTIFF = 1 << 5,
        EPlugin_ImageCodecPKM = 1 << 6,
        EPlugin_ImageCodecPVR = 1 << 7,

        EPlugin_MeshCodecFBX = 1 << 10, // ! not in EPlugin_All

        EPlugin_All = EPlugin_CompilerBackendVM |
        EPlugin_ImageCodecDDS |
        EPlugin_ImageCodecPNG |
        EPlugin_ImageCodecJPG |
        EPlugin_ImageCodecTGA

    };

    // loads selected plugins. set 'selected' to a bitwise combinations of the above plugin flags. must be matched by a call to 'UnregisterPlugins'
    bool    PopcornRegisterPlugins(u32 selected = 0);

    // unloads all previously loaded plugins
    void    PopcornUnregisterPlugins();

__LMBRPK_END

#endif //LMBR_USE_PK
