//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "PopcornFX_precompiled.h"
#include "BakerManager.h"

#if defined(O3DE_USE_PK)

#include <AzFramework/Platform/PlatformDefaults.h>

#include "../Tools/PK-AssetBakerLib/AssetBaker_Cookery.h"
#include "../Tools/PK-AssetBakerLib/AssetBaker_Startup.h"
#include "../Tools/PK-AssetBakerLib/AssetBaker_Oven.h"
#include "../Tools/PK-AssetBakerLib/AssetBaker_Oven_HBO.h"
#include "../Tools/PK-AssetBakerLib/AssetBaker_Oven_VectorField.h"

#include <pk_particles/include/ps_effect.h>
#include <pk_particles/include/ps_resources.h>
#include <pk_imaging/include/im_resource.h>
#include <pk_geometrics/include/ge_mesh_resource_handler.h>
#include <pk_geometrics/include/ge_rectangle_list.h>
#include <pk_particles/include/ps_font_metrics_resource.h>
#include <pk_particles/include/ps_vectorfield_resource.h>

#include "Integration/Editor/PackLoader.h"
#include "Integration/Managers/WindManager.h"

namespace PopcornFX {

//----------------------------------------------------------------------------------------------------------//
//																											//
//											CBakerManager::SBakeContext										//
//																											//
//----------------------------------------------------------------------------------------------------------//


CBakerManager::SBakeContext::SBakeContext()
	: m_BakeResourceMeshHandler(null)
	, m_BakeResourceImageHandler(null)
	, m_BakeResourceRectangleListHandler(null)
	, m_BakeResourceFontMetricsHandler(null)
	, m_BakeResourceVectorFieldHandler(null)
	, m_BakeFSController(null)
	, m_BakeResourceManager(null)
{

}

CBakerManager::SBakeContext::~SBakeContext()
{
	if (m_BakeResourceManager != null)
	{
		//PK_ASSERT(m_BakeResourceMeshHandler != null);
		//PK_ASSERT(m_BakeResourceImageHandler != null);
		PK_ASSERT(m_BakeResourceVectorFieldHandler != null);
		PK_ASSERT(m_BakeResourceFontMetricsHandler != null);
		PK_ASSERT(m_BakeResourceRectangleListHandler != null);

		//m_BakeResourceManager->UnregisterHandler<PopcornFX::CResourceMesh>(m_BakeResourceMeshHandler);
		//m_BakeResourceManager->UnregisterHandler<PopcornFX::CImage>(m_BakeResourceImageHandler);
		m_BakeResourceManager->UnregisterHandler<PopcornFX::CRectangleList>(m_BakeResourceRectangleListHandler);
		m_BakeResourceManager->UnregisterHandler<PopcornFX::CFontMetrics>(m_BakeResourceFontMetricsHandler);
		m_BakeResourceManager->UnregisterHandler<PopcornFX::CVectorField>(m_BakeResourceVectorFieldHandler);
	}
	//PK_SAFE_DELETE(m_BakeResourceMeshHandler);
	//PK_SAFE_DELETE(m_BakeResourceImageHandler);
	PK_SAFE_DELETE(m_BakeResourceVectorFieldHandler);
	PK_SAFE_DELETE(m_BakeResourceFontMetricsHandler);
	PK_SAFE_DELETE(m_BakeResourceRectangleListHandler);
	PK_SAFE_DELETE(m_BakeContext);
	PK_SAFE_DELETE(m_BakeFSController);
	PK_SAFE_DELETE(m_BakeResourceManager);
}

bool	CBakerManager::SBakeContext::Initialize()
{
	//PK_ASSERT(m_BakeResourceMeshHandler == null);
	PK_ASSERT(m_BakeResourceImageHandler == null);
	PK_ASSERT(m_BakeResourceVectorFieldHandler == null);
	PK_ASSERT(m_BakeResourceFontMetricsHandler == null);
	PK_ASSERT(m_BakeResourceRectangleListHandler == null);
	PK_ASSERT(m_BakeFSController == null);
	PK_ASSERT(m_BakeResourceManager == null);

	// Keep this updated with all PopcornFX resource types
	//m_BakeResourceMeshHandler = PK_NEW(PopcornFX::CResourceHandlerMesh);
	//m_BakeResourceImageHandler = PK_NEW(PopcornFX::CResourceHandlerImage);
	m_BakeResourceRectangleListHandler = PK_NEW(PopcornFX::CResourceHandlerRectangleList);
	m_BakeResourceFontMetricsHandler = PK_NEW(PopcornFX::CResourceHandlerFontMetrics);
	m_BakeResourceVectorFieldHandler = PK_NEW(PopcornFX::CResourceHandlerVectorField);
	if (//!PK_VERIFY(m_BakeResourceMeshHandler != null) ||
		//!PK_VERIFY(m_BakeResourceImageHandler != null) ||
		!PK_VERIFY(m_BakeResourceRectangleListHandler != null) ||
		!PK_VERIFY(m_BakeResourceFontMetricsHandler != null) ||
		!PK_VERIFY(m_BakeResourceVectorFieldHandler != null))
		return false;

	m_BakeFSController = File::NewInternalFileSystem();
	if (!PK_VERIFY(m_BakeFSController != null))
		return false;

	PFilePack	tempPack = m_BakeFSController->MountPack(".");
	if (!PK_VERIFY(tempPack != null))
		return false;

	m_BakeResourceManager = PK_NEW(PopcornFX::CResourceManager(m_BakeFSController));
	if (!PK_VERIFY(m_BakeResourceManager != null))
		return false;
	//m_BakeResourceManager->RegisterHandler<PopcornFX::CResourceMesh>(m_BakeResourceMeshHandler);
	//m_BakeResourceManager->RegisterHandler<PopcornFX::CImage>(m_BakeResourceImageHandler);
	m_BakeResourceManager->RegisterHandler<PopcornFX::CRectangleList>(m_BakeResourceRectangleListHandler);
	m_BakeResourceManager->RegisterHandler<PopcornFX::CFontMetrics>(m_BakeResourceFontMetricsHandler);
	m_BakeResourceManager->RegisterHandler<PopcornFX::CVectorField>(m_BakeResourceVectorFieldHandler);

	m_BakeContext = PK_NEW(PopcornFX::HBO::CContext(m_BakeResourceManager));
	if (!PK_VERIFY(m_BakeContext != null))
		return false;
	return true;
}

//----------------------------------------------------------------------------------------------------------//
//																											//
//											CBakerManager													//
//																											//
//----------------------------------------------------------------------------------------------------------//

bool	CBakerManager::Activate()
{
	if (!AssetBaker::Startup())
		return false;

	m_BakeContext = PK_NEW(SBakeContext);
	if (!PK_VERIFY(m_BakeContext != null))
		return false;
	if (!PK_VERIFY(m_BakeContext->Initialize()))
		return false;

	m_Cookery = PK_NEW(CCookery);
	if (!PK_VERIFY(m_Cookery != null))
		return false;

	m_Cookery->SetHBOContext(m_BakeContext->m_BakeContext);
	if (!m_Cookery->TurnOn())
		return false;

	// Force set the coordinate frame
	CCoordinateFrame::SetGlobalFrame(Frame_RightHand_Z_Up);

	const CGuid	ovenIdParticle = m_Cookery->RegisterOven(PK_NEW(COven_Particle));
	const CGuid	ovenIdVectorField = m_Cookery->RegisterOven(PK_NEW(COven_VectorField));
	if (!ovenIdParticle.Valid() ||
		!ovenIdVectorField.Valid())
		return false;

	m_Cookery->MapOven("fga", ovenIdVectorField);		// FGA vector-field
	m_Cookery->MapOven("pkfx", ovenIdParticle);		// PopcornFX Effect
	m_Cookery->AddOvenFlags(PopcornFX::COven::Flags_BakeMemoryVersion);

	// enable multithreaded baking
	PK_VERIFY(m_Cookery->EnableMultithreadedBake(false));

	PK_FOREACH(it, m_Cookery->m_BaseConfigFile->ObjectList())
	{
		PopcornFX::COvenBakeConfig_Particle	*config = PopcornFX::HBO::Cast<PopcornFX::COvenBakeConfig_Particle>(*it);
		if (config != null)
		{
			config->SetRemoveEditorNodes(true);
			config->SetBakeMode(PopcornFX::COvenBakeConfig_HBO::Bake_SaveAsBinary);
			// We know O3DE has processed textures as .streamingimage files and meshes as .azmodel files
			PopcornFX::COvenBakeConfig_HBO::_TypeOfExtensionsRemap	extensionsRemap = config->ExtensionsRemap();
			if (PK_VERIFY(extensionsRemap.PushBack(CString("png=png.streamingimage")).Valid()) &&
				PK_VERIFY(extensionsRemap.PushBack(CString("jpg=jpg.streamingimage")).Valid()) &&
				PK_VERIFY(extensionsRemap.PushBack(CString("jpeg=jpeg.streamingimage")).Valid()) &&
				PK_VERIFY(extensionsRemap.PushBack(CString("tga=tga.streamingimage")).Valid()) &&
				PK_VERIFY(extensionsRemap.PushBack(CString("tif=tif.streamingimage")).Valid()) &&
				PK_VERIFY(extensionsRemap.PushBack(CString("tiff=tiff.streamingimage")).Valid()) &&
				PK_VERIFY(extensionsRemap.PushBack(CString("dds=dds.streamingimage")).Valid()) &&
				PK_VERIFY(extensionsRemap.PushBack(CString("fbx=azmodel")).Valid()))
			{
				config->SetExtensionsRemap(extensionsRemap);
			}
		}
	}
	return true;
}

void	CBakerManager::Deactivate()
{
	PK_SAFE_DELETE(m_BakeContext);
	PK_SAFE_DELETE(m_Cookery);
	AssetBaker::Shutdown();
}

AZStd::string	CBakerManager::BakeSingleAsset(const AZStd::string &assetPath, const AZStd::string &outDir, const AZStd::string &platform, CWindManager &windManager)
{
	if (!_SetPackIFN(assetPath, windManager))
		return "";

	const CString	virtualPath = m_Cookery->FileController()->PhysicalToVirtual(assetPath.c_str());

	CMessageStream							bakerMessages;
	PopcornFX::TArray<SBakeTarget>			dstPackPaths;

	CThreadID	tid = CCurrentThread::ThreadID();
	if (!tid.Valid())	// if we're not seen as a thread known by the PopcornFX SDK
		tid = CCurrentThread::RegisterUserThread();	// register ourself

	dstPackPaths.PushBack(PopcornFX::SBakeTarget("O3DE_Generic", outDir.c_str()));

	_SetBuildVersion(platform);

	CLog::Log(PK_INFO, "Baking effect '%s'", virtualPath.Data());

	const bool	bakeOk = m_Cookery->BakeAsset(virtualPath, m_Cookery->m_BaseConfigFile, bakerMessages, dstPackPaths);
	LogBakerMessages(bakerMessages);
	if (!bakeOk)
	{
		CLog::Log(PK_ERROR, "Failed baking effect '%s'", virtualPath.Data());
		return "";
	}

	CLog::Log(PK_INFO, "Successfully baked effect '%s'", virtualPath.Data());
	return virtualPath.Data();
}

bool	CBakerManager::GatherDependencies(const AZStd::string &assetPath, AZStd::vector<AZStd::string> &dependencies, CWindManager &windManager)
{
	AZStd::string	rootPath;
	AZStd::string	libraryPath;

	if (ChangePackIFN(assetPath, m_Cookery->FileController(), rootPath, libraryPath))
		windManager.Reset(libraryPath);
	else if (rootPath.empty())
		return false;

	const CString	virtualPath = m_Cookery->FileController()->PhysicalToVirtual(assetPath.c_str());

	PopcornFX::HBO::CContext	*context = m_Cookery->HBOContext();
	PBaseObjectFile				srcFile = context->FindFile(virtualPath);

	bool	ownsFile = (srcFile == null);
	if (ownsFile)
		srcFile = context->LoadFile(virtualPath, true);

	if (srcFile == null)
	{
		CLog::Log(PK_INFO, "Couldn't find asset '%s'", virtualPath.Data());
		return false;
	}

	PCParticleEffect	effect = srcFile->FindFirstOf<CParticleEffect>();
	if (effect == null)
		return false; // _GatherDependencies is not supported on anything else than a CParticleEffect

	PopcornFX::TArray<PopcornFX::SResourceDependency>	effectDepends;
	if (!effect->GatherRuntimeDependencies(effectDepends) || !effect->GatherStaticDependencies(effectDepends))
	{
		CLog::Log(PK_INFO, "Couldn't gather effect dependencies for '%s'", effect->File()->Path().Data());
		return false;
	}
	const u32	dependsCount = effectDepends.Count();
	for (u32 iDepend = 0; iDepend < dependsCount; ++iDepend)
	{
		const SResourceDependency	&depend = effectDepends[iDepend];
		if (depend.Empty())
			continue;

		const AZStd::string	dependFullPath = rootPath + "/" + depend.m_Path.Data();

		if (depend.m_Usage & PopcornFX::SResourceDependency::UsageFlags_UsedInSimulation)
		{
			// We will only append runtime related dependencies
			// Except .pkat files which are handled using a copy command in .ini files, there are no PopcornFX specific resources that are used by render code.
			// This gather dependencies code is only here to output runtime, PopcornFX specific files
			dependencies.push_back(dependFullPath);
		}
		else
		{
			const CStringView	ext = CFilePath::ExtractExtension(CStringView(depend.m_Path));

			if (ext == "pkfx")
			{
				dependencies.push_back(dependFullPath);
			}
		}

	}
	return true;
}

void	CBakerManager::LogBakerMessages(const CMessageStream &messages)
{
	PopcornFX::TArray<CString>	messageLines;
	for (const CMessageStream::SMessage &msg : messages.Messages())
	{
		msg.m_Message.Split('\n', messageLines);
		for (u32 j = 0; j < messageLines.Count(); j++)
		{
			switch (msg.m_Level)
			{
			case	CMessageStream::Info:
				CLog::Log(PK_INFO, "%s", messageLines[j].Data());
				break;
			case	CMessageStream::Warning:
				CLog::Log(PK_WARN, "%s", messageLines[j].Data());
				break;
			case	CMessageStream::Error:
				CLog::Log(PK_ERROR, "%s", messageLines[j].Data());
				break;
			default:
				PK_ASSERT_NOT_REACHED();
			}
		}
	}
}

bool	CBakerManager::_SetPackIFN(const AZStd::string &assetPath, CWindManager &windManager)
{
	AZStd::string	rootPath;
	AZStd::string	libraryPath;

	if (ChangePackIFN(assetPath, m_Cookery->FileController(), rootPath, libraryPath))
		windManager.Reset(libraryPath);
	else if (rootPath.empty())
		return false;
	return true;
}

void	CBakerManager::_SetBuildVersion(const AZStd::string &platform)
{
	PK_FOREACH(it, m_Cookery->m_BaseConfigFile->ObjectList())
	{
		PopcornFX::COvenBakeConfig_Particle	*config = PopcornFX::HBO::Cast<PopcornFX::COvenBakeConfig_Particle>(*it);
		if (config != null)
		{
			COvenBakeConfig_Particle::_TypeOfBuildVersions	buildVersions;

			if (platform != AzFramework::PlatformServer)
			{
				CString	platformTags;
				if (platform == AzFramework::PlatformPC)
					platformTags = "windows, desktop";
				else if (platform == AzFramework::PlatformLinux)
					platformTags = "linux, desktop";
				else if (platform == AzFramework::PlatformMac)
					platformTags = "macos, desktop";
				else if (platform == AzFramework::PlatformAndroid)
					platformTags = "android, mobile";
				else if (platform == AzFramework::PlatformIOS)
					platformTags = "ios, mobile";
				else if (platform == AzFramework::PlatformProvo)
					platformTags = "provo, console";

				if (!platformTags.Empty())
					platformTags = CString(", ") + platformTags;

				buildVersions.PushBack(CString("Low: low") + platformTags);
				buildVersions.PushBack(CString("Medium: medium") + platformTags);
				buildVersions.PushBack(CString("High: high") + platformTags);
				buildVersions.PushBack(CString("VeryHigh: veryhigh") + platformTags);
			}

			config->SetBuildVersions(buildVersions);
		}
	}
}

//----------------------------------------------------------------------------
}

#endif //O3DE_USE_PK
