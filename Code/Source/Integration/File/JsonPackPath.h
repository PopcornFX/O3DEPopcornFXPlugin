//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------

#pragma once

#include <AzCore/std/string/string.h>

#if defined(O3DE_USE_PK)

namespace PopcornFX {
	bool	LoadPackPathFromJson(AZStd::string &outPackPath, AZStd::string &outLibraryPath);
	bool	SavePackPathToJson(const AZStd::string &packPath, const AZStd::string &libraryPath);
}

#endif //O3DE_USE_PK
