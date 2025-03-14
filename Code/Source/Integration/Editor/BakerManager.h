//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <AzCore/std/string/string.h>
#include <AzCore/std/containers/vector.h>

#if defined(O3DE_USE_PK)

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
	class IResourceHandler;
	PK_FORWARD_DECLARE(OvenBakeConfig_HBO);
}

namespace PopcornFX {

class CWindManager;

class CEffectBrowser
{
public:
	bool	ActivateIFN();
	void	Deactivate();

	bool	GatherRuntimeDependencies(const AZStd::string &packPath, const AZStd::string &effectPath, AZStd::vector<AZStd::string> &outDependencies);

private:
	IResourceHandler			*m_BrowseResourceMeshHandler = null;
	IResourceHandler			*m_BrowseResourceImageHandler = null;
	IResourceHandler			*m_BrowseResourceRectangleListHandler = null;
	IResourceHandler			*m_BrowseResourceFontMetricsHandler = null;
	IResourceHandler			*m_BrowseResourceVectorFieldHandler = null;
	IFileSystem					*m_BrowseFSController = null;
	CResourceManager			*m_BrowseResourceManager = null;
	HBO::CContext				*m_BrowseContext = null;
	bool						m_Activated = false;
};

//----------------------------------------------------------------------------

class CBakerManager
{
public:
	bool	Activate();
	void	Deactivate();

	AZStd::string	BakeSingleAsset(const AZStd::string &assetPath, const AZStd::string &outDir, const AZStd::string &platform, CWindManager &windManager);
	bool			GatherStaticDependencies(const AZStd::string &assetPath, AZStd::vector<AZStd::string> &dependencies, CWindManager &windManager);
	bool			GatherRuntimeDependencies(const AZStd::string &packPath, const AZStd::string &effectPath, AZStd::vector<AZStd::string> &dependencies);

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

#if (PK_MESSAGE_STREAM_NO_MESSAGES == 0)
	void	LogBakerMessages(const CMessageStream &messages);
#endif
	bool	_SetPackIFN(const AZStd::string &assetPath, CWindManager &windManager);
	void	_SetBuildVersion(const AZStd::string &platform);

	CCookery		*m_Cookery = null;
	SBakeContext	*m_BakeContext = null;
	AZStd::string	m_RootPath;
	CEffectBrowser	m_EffectBrowser;
};

//----------------------------------------------------------------------------
}

#endif //O3DE_USE_PK
