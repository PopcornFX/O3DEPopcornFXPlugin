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

#include <AzCore/std/string/string.h>

#if defined(LMBR_USE_PK)

__LMBRPK_BEGIN
	bool	LoadPackPathFromJson(AZStd::string &outPackPath, AZStd::string &outLibraryPath);
	bool	SavePackPathToJson(const AZStd::string &packPath, const AZStd::string &libraryPath);
__LMBRPK_END

#endif //LMBR_USE_PK
