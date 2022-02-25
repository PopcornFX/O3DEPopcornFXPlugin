//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <AzCore/std/string/string.h>

#if defined(O3DE_USE_PK)

namespace PopcornFX {
	void	GetThumbnailPathForAsset(const AZStd::string &assetPath, AZStd::string &outThumbnailPath);
	bool	ChangePackIFN(	const AZStd::string &assetPath, IFileSystem *fileSystem,
							AZStd::string &outRootPath, AZStd::string &outLibraryPath,
							const AZStd::string &devFolder = "");
	bool	ChangePackWithPathIFN(	const AZStd::string &pkProjPath, IFileSystem *fileSystem,
									AZStd::string &outRootPath, AZStd::string &outLibraryPath,
									const AZStd::string &devFolder = "");
}

#endif //O3DE_USE_PK
