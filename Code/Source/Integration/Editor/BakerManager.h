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
#include <AzCore/std/containers/vector.h>

#if defined(LMBR_USE_PK)

namespace PopcornFX
{
	class CResourceHandlerMesh;
	class CResourceHandlerImage;
	class CResourceHandlerRectangleList;
	class CResourceHandlerFontMetrics;
	class CResourceHandlerVectorField;
	class CResourceManager;
	class CCookery;
	class CMessageStream;
	namespace HBO {
		class CContext;
	}
	PK_FORWARD_DECLARE(OvenBakeConfig_HBO);
}

__LMBRPK_BEGIN

class CWindManager;

//----------------------------------------------------------------------------

class CBakerManager
{
public:
	bool	Activate();
	void	Deactivate();

	AZStd::string	BakeSingleAsset(const AZStd::string &assetPath, const AZStd::string &outDir, const AZStd::string &platform, CWindManager &windManager);
	bool			GatherDependencies(const AZStd::string &assetPath, AZStd::vector<AZStd::string> &dependencies, CWindManager &windManager);

private:
	class		SBakeContext
	{
	public:
		SBakeContext();
		~SBakeContext();
		CResourceHandlerMesh			*m_BakeResourceMeshHandler;
		CResourceHandlerImage			*m_BakeResourceImageHandler;
		CResourceHandlerRectangleList	*m_BakeResourceRectangleListHandler;
		CResourceHandlerFontMetrics		*m_BakeResourceFontMetricsHandler;
		CResourceHandlerVectorField		*m_BakeResourceVectorFieldHandler;
		IFileSystem						*m_BakeFSController;
		CResourceManager				*m_BakeResourceManager;
		HBO::CContext					*m_BakeContext;

		bool	Initialize();
	};

	void	LogBakerMessages(const CMessageStream &messages);
	bool	_SetPackIFN(const AZStd::string &assetPath, CWindManager &windManager);
	void	_SetBuildVersion(const AZStd::string &platform);

	CCookery		*m_Cookery = null;
	SBakeContext	*m_BakeContext = null;
};

//----------------------------------------------------------------------------
__LMBRPK_END

#endif //LMBR_USE_PK
