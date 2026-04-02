//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------

#pragma once

#include <AzCore/std/string/string.h>

#if defined(O3DE_USE_PK)

namespace PopcornFX {
	void	GetThumbnailPathForAsset(const AZStd::string &assetPath, AZStd::string &outThumbnailPath, AZStd::string &inOutPkProjPath);
	bool	ChangePackIFN(	const AZStd::string &assetPath, IFileSystem *fileSystem,
							AZStd::string &rootPath, AZStd::string &outLibraryPath, bool assetProcessor = true);
}

#endif //O3DE_USE_PK
