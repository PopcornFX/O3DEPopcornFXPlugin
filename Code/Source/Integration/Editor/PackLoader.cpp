//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include "PopcornFX_precompiled.h"

#include "PackLoader.h"

#if defined(LMBR_USE_PK)

#include <AzFramework/StringFunc/StringFunc.h>
#include <AzFramework/IO/LocalFileIO.h>
#include <AzCore/std/function/function_template.h>

#include <pk_kernel/include/kr_file.h>
#include <pk_kernel/include/kr_resources.h>
#include <pk_base_object/include/hbo_context.h>
#include <pk_particles/include/ps_project_settings.h>

__LMBRPK_BEGIN

bool			GetProjectSettings(const AZStd::string &projectFilePath, AZStd::string &outRootPath, AZStd::string &outLibraryPath);
AZStd::string	FindFileInHierarchy(const AZStd::string &folder, const AZStd::string &file);
AZStd::string	FindPkprojWithRootDirOnDev(const AZStd::string &devFolderPath);

bool	GetProjectSettings(const AZStd::string &projectFilePath, AZStd::string &outRootPath, AZStd::string &outLibraryPath)
{
	// Create a local context for loading the .pkproj
	IFileSystem			*localFileSystem = File::NewInternalFileSystem();
	if (!PK_VERIFY(localFileSystem != null))
		return false;
	CResourceManager	*localResourceManager = PK_NEW(CResourceManager(localFileSystem)); // No resource handler bound
	if (!PK_VERIFY(localResourceManager != null))
	{
		PK_DELETE(localFileSystem);
		return false;
	}
	PopcornFX::HBO::CContext	*localBoContext = PK_NEW(PopcornFX::HBO::CContext(localResourceManager));
	if (PK_VERIFY(localBoContext != null))
	{
		CString	pkprojFile = projectFilePath.c_str();
		CFilePath::Purify(pkprojFile);

		const CString					projectSettingsContent = localFileSystem->BufferizeToString(pkprojFile, true);
		PopcornFX::CConstMemoryStream	stream(projectSettingsContent.Data(), projectSettingsContent.Length());
		PopcornFX::PProjectSettings		projectSettings = PopcornFX::CProjectSettings::LoadFromStream(stream, localBoContext);
		CString							folderPath = CFilePath::StripFilename(pkprojFile);

		if (projectSettings != null)
		{
			//Apply ApplyGlobalSettings
			projectSettings->ApplyGlobalSettings(); // Applies constants

			// CProjectSettings::ApplyGlobalSettings also applies the .pkproj's scene settings (distance units and axis system)
			// We don't need the .pkproj's axis system informations: override it (allows effects coming from incorrectly setup packs to be imported without issues)
			CCoordinateFrame::SetGlobalFrame(Frame_RightHand_Z_Up);

			if (!projectSettings->General()->RootDir().Empty()) // We need RootDir
			{
				folderPath /= projectSettings->General()->RootDir();
				CFilePath::Purify(folderPath);
			}
		}

		outLibraryPath = projectSettings->General()->LibraryDir().Data();

		localBoContext->UnloadAllFiles();
		PK_DELETE(localBoContext);
		PK_DELETE(localResourceManager);
		PK_DELETE(localFileSystem);
		AZStd::string	ret = folderPath.Data();
		AzFramework::StringFunc::Path::Normalize(ret);
		outRootPath = ret;
		return true;
	}
	PK_DELETE(localResourceManager);
	PK_DELETE(localFileSystem);
	return false;
}

bool	GetProjectSettingsThumbnailsPath(const AZStd::string &projectFilePath, AZStd::string &outRootDir, AZStd::string &outThumbnailsPath)
{
	// Create a local context for loading the .pkproj
	IFileSystem			*localFileSystem = File::NewInternalFileSystem();
	if (!PK_VERIFY(localFileSystem != null))
		return false;
	CResourceManager	*localResourceManager = PK_NEW(CResourceManager(localFileSystem)); // No resource handler bound
	if (!PK_VERIFY(localResourceManager != null))
	{
		PK_DELETE(localFileSystem);
		return false;
	}
	PopcornFX::HBO::CContext	*localBoContext = PK_NEW(PopcornFX::HBO::CContext(localResourceManager));
	if (PK_VERIFY(localBoContext != null))
	{
		CString	pkprojFile = projectFilePath.c_str();
		CFilePath::Purify(pkprojFile);

		const CString					projectSettingsContent = localFileSystem->BufferizeToString(pkprojFile, true);
		PopcornFX::CConstMemoryStream	stream(projectSettingsContent.Data(), projectSettingsContent.Length());
		PopcornFX::PProjectSettings		projectSettings = PopcornFX::CProjectSettings::LoadFromStream(stream, localBoContext);

		bool	found = false;
		if (projectSettings != null &&
			projectSettings->General() != null)
		{
			if (!projectSettings->General()->RootDir().Empty())
				outRootDir = projectSettings->General()->RootDir().Data();
			outThumbnailsPath = projectSettings->General()->ThumbnailsDir().Data();
			found = true;
		}

		localBoContext->UnloadAllFiles();
		PK_DELETE(localBoContext);
		PK_DELETE(localResourceManager);
		PK_DELETE(localFileSystem);
		return found;
	}
	PK_DELETE(localResourceManager);
	PK_DELETE(localFileSystem);
	return false;
}

AZStd::string	FindFileInHierarchy(const AZStd::string &folder, const AZStd::string &file)
{
	AZStd::string		filePath;
	AZStd::string		folderPath = folder;
	AZ::IO::LocalFileIO	local;

	while (filePath.empty())
	{
		AZStd::string	tryProjPath;
		AzFramework::StringFunc::Path::Join(folderPath.c_str(), file.c_str(), tryProjPath);
		if (local.Exists(tryProjPath.c_str()))
		{
			filePath = tryProjPath;
		}
		else
		{
			if (!AzFramework::StringFunc::Path::StripComponent(folderPath, true) ||
				folderPath.size() < 3) // matches drive paths and too small to be valid
			{
				break;
			}
		}
	}
	AzFramework::StringFunc::Path::Normalize(filePath);
	return filePath;
}

AZStd::string	FindPkprojWithRootDirOnDev(const AZStd::string &devFolderPath)
{
	AZ::IO::LocalFileIO				local;
	AZStd::vector<AZStd::string>	rootFiles;

	//search all files at root
	bool foundOK = local.FindFiles(devFolderPath.c_str(), "*.*",
		[&](const char* filePath) -> bool
	{
		rootFiles.push_back(filePath);
		return true;
	});

	if (!foundOK)
	{
		//AZ_TracePrintf(AssetBuilderSDK::ErrorWindow, "[PopcornFX] - Find PK Project file failed, unable to explore root");
		return "";
	}

	//search .pkproj in found directories
	AZStd::vector<AZStd::string>	pkprojFiles;
	for (size_t i = 0; i < rootFiles.size(); ++i)
	{
		const AZStd::string	&file = rootFiles[i];
		if (local.IsDirectory(file.c_str()))
		{
			local.FindFiles(file.c_str(), "PopcornProject.pkproj",
				[&](const char* filePath) -> bool
			{
				pkprojFiles.push_back(filePath);
				return true;
			});
		}
	}

	if (pkprojFiles.empty())
	{
		//AZ_TracePrintf(AssetBuilderSDK::ErrorWindow, "[PopcornFX] - Find PK Project file failed, PopcornProject.pkproj not found");
		return "";
	}

	for (size_t i = 0; i < pkprojFiles.size(); ++i)
	{
		AZStd::string	rootPath;
		AZStd::string	dummy;
		if (!GetProjectSettings(pkprojFiles[i], rootPath, dummy))
			return "";

		if (rootPath == devFolderPath)
		{
			AzFramework::StringFunc::Path::Normalize(pkprojFiles[i]);
			return pkprojFiles[i];
		}
	}
	
	return "";
}

void	GetThumbnailPathForAsset(const AZStd::string &assetPath, AZStd::string &outThumbnailPath)
{
	const AZStd::string	devassets = AZ::IO::FileIOBase::GetInstance()->GetAlias("@devassets@");
	AZStd::string		folderPath = assetPath;
	AzFramework::StringFunc::Path::StripFullName(folderPath);
	AzFramework::StringFunc::Path::Normalize(folderPath);

	//Find pkproj path
	AZStd::string	pkProjPath = FindFileInHierarchy(folderPath, "PopcornProject.pkproj");

	if (pkProjPath.empty())
	{
		//we are outside of a popcorn folder
		//searching a PopcornProject.pkproj with a RootDir set to ..
		AZStd::string	devFolderPath = devassets;
		if (devFolderPath.empty())
		{
			devFolderPath = FindFileInHierarchy(folderPath, "project.json");
			AzFramework::StringFunc::Path::StripFullName(devFolderPath);
			AzFramework::StringFunc::Path::Normalize(devFolderPath);
			AzFramework::StringFunc::Strip(devFolderPath, AZ_CORRECT_FILESYSTEM_SEPARATOR, false, false, true);
		}
		if (!devFolderPath.empty())
			pkProjPath = FindPkprojWithRootDirOnDev(devFolderPath);
	}
	if (!pkProjPath.empty())
	{
		AZStd::string	rootDir;
		if (GetProjectSettingsThumbnailsPath(pkProjPath, rootDir, outThumbnailPath))
		{
			const CString	pkProjFolder = CFilePath::StripFilename(pkProjPath.c_str());
			CString			filePath = assetPath.c_str();
			CString			projectPath = CString::Format("%s/%s/", pkProjFolder.Data(), rootDir.c_str());

			CFilePath::Purify(filePath);
			CFilePath::Purify(projectPath);

			if (PK_VERIFY(filePath.StartsWith(projectPath)))
			{
				const CString	virtualFilePath = filePath.Extract(projectPath.Length() + 1, filePath.Length());
				CString			thumbnailPath = CString::Format("%s/%s/%s.png", projectPath.Data(), outThumbnailPath.c_str(), virtualFilePath.Data());

				CFilePath::Purify(thumbnailPath);
				outThumbnailPath = thumbnailPath.Data();
			}
		}
	}
}

bool	ChangePackIFN(	const AZStd::string &assetPath, IFileSystem *fileSystem,
						AZStd::string &outRootPath, AZStd::string &outLibraryPath,
						const AZStd::string &devFolder /*= ""*/)
{
	AZStd::string	folderPath = assetPath;
	AzFramework::StringFunc::Path::StripFullName(folderPath);
	AzFramework::StringFunc::Path::Normalize(folderPath);

	//Find pkproj path
	AZStd::string	pkProjPath = FindFileInHierarchy(folderPath, "PopcornProject.pkproj");

	if (pkProjPath.empty())
	{
		//we are outside of a popcorn folder
		//searching a PopcornProject.pkproj with a RootDir set to ..
		AZStd::string	devFolderPath = devFolder;
		if (devFolderPath.empty())
		{
			devFolderPath = FindFileInHierarchy(folderPath, "project.json");
			AzFramework::StringFunc::Path::StripFullName(devFolderPath);
			AzFramework::StringFunc::Path::Normalize(devFolderPath);
			AzFramework::StringFunc::Strip(devFolderPath, AZ_CORRECT_FILESYSTEM_SEPARATOR, false, false, true);
		}
		if (devFolderPath.empty())
			return false;
		pkProjPath = FindPkprojWithRootDirOnDev(devFolderPath);
		if (pkProjPath.empty())
			return false;
	}

	return ChangePackWithPathIFN(pkProjPath, fileSystem, outRootPath, outLibraryPath, devFolder);
}

bool	ChangePackWithPathIFN(	const AZStd::string &pkProjPath, IFileSystem *fileSystem,
								AZStd::string &outRootPath, AZStd::string &outLibraryPath,
								const AZStd::string &devFolder /*= ""*/)
{
	AZStd::string	rootPath;
	if (!GetProjectSettings(pkProjPath, rootPath, outLibraryPath))
		return false;

	//Check if the pack is already mounted
	TMemoryView<const PFilePack>	packs = fileSystem->PrioritizedPacks();

	if (packs.Count() > 0)
	{
		AZStd::string	packPath;

		packPath = packs[0]->Path().Data();

		if (!devFolder.empty())
			AzFramework::StringFunc::Path::Join(devFolder.c_str(), packPath.c_str(), packPath);

		AzFramework::StringFunc::Path::Normalize(packPath);

		if (!packPath.empty() && rootPath == packPath)
		{
			//AZ_TracePrintf(AssetBuilderSDK::InfoWindow, "[PopcornFX] Pack %s already mounted", packPath.c_str());
			if (!devFolder.empty())
				packPath = CFilePath::Relativize(devFolder.c_str(), rootPath.c_str()).Data();
			outRootPath = packPath;
			return false;
		}
		else
		{
			//AZ_TracePrintf(AssetBuilderSDK::InfoWindow, "[PopcornFX] Asset not in pack %s, unmounting it.", packPath.c_str());
			fileSystem->UnmountAllPacks();
		}
	}

	//mount the pack
	if (!rootPath.empty())
	{
		AZStd::string	packPath = rootPath;
		PFilePack		filePack = null;
		//AZ_TracePrintf(AssetBuilderSDK::InfoWindow, "[PopcornFX] Mounting RootDir %s", folderPath.Data());

		if (!devFolder.empty())
			packPath = CFilePath::Relativize(devFolder.c_str(), rootPath.c_str()).Data();

		filePack = fileSystem->MountPack(packPath.c_str());
		if (filePack != null)
		{
			outRootPath = packPath.c_str();
			return true;
		}
	}
	return false;
}

__LMBRPK_END
//----------------------------------------------------------------------------

#endif //LMBR_USE_PK

