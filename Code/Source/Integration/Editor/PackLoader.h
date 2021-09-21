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
	void	GetThumbnailPathForAsset(const AZStd::string &assetPath, AZStd::string &outThumbnailPath);
	bool	ChangePackIFN(	const AZStd::string &assetPath, IFileSystem *fileSystem,
							AZStd::string &outRootPath, AZStd::string &outLibraryPath,
							const AZStd::string &devFolder = "");
	bool	ChangePackWithPathIFN(	const AZStd::string &pkProjPath, IFileSystem *fileSystem,
									AZStd::string &outRootPath, AZStd::string &outLibraryPath,
									const AZStd::string &devFolder = "");
__LMBRPK_END

#endif //LMBR_USE_PK
