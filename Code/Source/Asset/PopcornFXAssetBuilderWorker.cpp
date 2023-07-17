//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "PopcornFX_precompiled.h"

#include "PopcornFXAssetBuilderWorker.h"

#include <AzFramework/IO/LocalFileIO.h>
#include <AzCore/StringFunc/StringFunc.h>

#include "PopcornFXAsset.h"
#include "Integration/PopcornFXIntegrationBus.h"


namespace PopcornFX {

//----------------------------------------------------------------------------------------------------------//
//																											//
//											PopcornFXBuilderWorker											//
//																											//
//----------------------------------------------------------------------------------------------------------//

// Note - Shutdown will be called on a different thread than your process job thread
void	PopcornFXBuilderWorker::ShutDown()
{
	m_IsShuttingDown = true;
}

// CreateJobs will be called early on in the file scanning pass from the Asset Processor.
// You should create the same jobs, and avoid checking whether the job is up to date or not. The Asset Processor will manage this for you
void	PopcornFXBuilderWorker::CreateJobs(const AssetBuilderSDK::CreateJobsRequest &request, AssetBuilderSDK::CreateJobsResponse &response)
{
	AZ_Info("PopcornFX", "Begin jobs creation for '%s'.", request.m_sourceFile.c_str());
	if (m_IsShuttingDown)
	{
		response.m_result = AssetBuilderSDK::CreateJobsResultCode::ShuttingDown;
		return;
	}
	AZStd::string	ext;
	AZStd::string	relPath = request.m_sourceFile;

	response.m_result = AssetBuilderSDK::CreateJobsResultCode::Failed;

	// TODO: Fix that, if the user has some Cache/ folder somewhere and it's legit, this won't work.
	// We need to grab the project's editor cache folder from PopcornProject.pkproj
	if (AZ::StringFunc::Find(relPath.c_str(), "Cache/") != AZStd::string::npos)
	{
		response.m_result = AssetBuilderSDK::CreateJobsResultCode::Success; // We do not want to process files from project's cache folder. This is not an error
		return;
	}

	AZ::StringFunc::Path::GetExtension(request.m_sourceFile.c_str(), ext, false);

	if (AZ::StringFunc::Equal(ext.c_str(), "pkfx"))
	{
		AZStd::string	fullPath;
		AZ::StringFunc::Path::ConstructFull(request.m_watchFolder.c_str(), request.m_sourceFile.c_str(), fullPath, false);

		bool							ok = false;
		AZStd::vector<AZStd::string>	dependencies;
		PopcornFX::PopcornFXIntegrationBus::BroadcastResult(ok, &PopcornFX::PopcornFXIntegrationBus::Handler::GatherDependencies, fullPath, dependencies);

		if (!ok)
		{
			AZ_Error("PopcornFX", false, "_GatherDependencies failed on '%s'.", fullPath.c_str());
			return;
		}
		for (int i = 0; i < dependencies.size(); ++i)
		{
			AssetBuilderSDK::SourceFileDependency	dependencyInfo;
			dependencyInfo.m_sourceFileDependencyPath = dependencies[i];
			response.m_sourceFileDependencyList.push_back(dependencyInfo);

			AZ_Info("PopcornFX", "'%s': New dependency added '%s'", relPath.data(), dependencies[i].c_str());
		}
		for (const AssetBuilderSDK::PlatformInfo &platformInfo : request.m_enabledPlatforms)
		{
			AssetBuilderSDK::JobDescriptor	descriptor;
			descriptor.m_jobKey = "PopcornFX Effect";
			descriptor.SetPlatformIdentifier(platformInfo.m_identifier.c_str());

			response.m_createJobOutputs.push_back(descriptor);
		}
		response.m_result = AssetBuilderSDK::CreateJobsResultCode::Success;
		return;
	}
	else if (AZ::StringFunc::Equal(ext.c_str(), "fga"))
	{
		for (const AssetBuilderSDK::PlatformInfo &platformInfo : request.m_enabledPlatforms)
		{
			AssetBuilderSDK::JobDescriptor	descriptor;
			descriptor.m_jobKey = "PopcornFX VectorField";
			descriptor.SetPlatformIdentifier(platformInfo.m_identifier.c_str());
			response.m_createJobOutputs.push_back(descriptor);
		}
		response.m_result = AssetBuilderSDK::CreateJobsResultCode::Success;
		return;
	}
	AZ_Error("PopcornFX", false, "Unhandled extension type in PopcornFXBuilderWorker for '%s'", request.m_sourceFile.c_str());
}


void	PopcornFXBuilderWorker::ProcessJob(const AssetBuilderSDK::ProcessJobRequest &request, AssetBuilderSDK::ProcessJobResponse &response)
{
	response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Cancelled;

	AssetBuilderSDK::JobCancelListener	jobCancelListener(request.m_jobId);
	AZStd::string						ext;

	AZ::StringFunc::Path::GetExtension(request.m_sourceFile.c_str(), ext, false);

	if (!AZ::StringFunc::Equal(ext.c_str(), "pkfx") &&
		!AZ::StringFunc::Equal(ext.c_str(), "fga"))
	{
		AZ_Warning("PopcornFX", false, "Wrong file type for job '%s', ignoring.", request.m_fullPath.c_str());
		return;
	}
	if (!(AZ::StringFunc::Equal(request.m_jobDescription.m_jobKey.c_str(), "PopcornFX Effect")
		|| AZ::StringFunc::Equal(request.m_jobDescription.m_jobKey.c_str(), "PopcornFX VectorField")))
	{
		AZ_Warning("PopcornFX", false, "Wrong job description for job '%s', ignoring.", request.m_fullPath.c_str());
		return;
	}
	if (jobCancelListener.IsCancelled() || m_IsShuttingDown)
	{
		AZ_Warning("PopcornFX", false, "Cancel was requested for job '%s'.", request.m_fullPath.c_str());
		return;
	}
	response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Failed;

	AZStd::string	fullSrcPath = request.m_fullPath;

	AZ_Info("PopcornFX", "Processing asset '%s'", fullSrcPath.c_str());
	AZ_Info("PopcornFX", "Asset temp bake directory: '%s'", request.m_tempDirPath.c_str());

	AZStd::string	virtualPath;
	PopcornFX::PopcornFXIntegrationBus::BroadcastResult(virtualPath, &PopcornFX::PopcornFXIntegrationBus::Handler::BakeSingleAsset, fullSrcPath, request.m_tempDirPath, request.m_platformInfo.m_identifier);

	if (virtualPath.empty())
		return;

	AssetBuilderSDK::JobProduct		jobProduct{ virtualPath };
	if (AZ::StringFunc::Equal(ext.c_str(), "pkfx"))
	{
		jobProduct.m_productAssetType = AZ::AzTypeInfo<PopcornFXAsset>::Uuid();
		jobProduct.m_dependenciesHandled = true;
	}
	else
	{
		AZ::IO::LocalFileIO	fileIO;
		AZStd::string		buildPath = request.m_tempDirPath + "/" + virtualPath;

		AZ::StringFunc::Path::ReplaceExtension(buildPath, "pkvf");
		if (fileIO.Exists(buildPath.c_str()))
		{
			AZ::StringFunc::Path::ReplaceExtension(jobProduct.m_productFileName, "pkvf");
			jobProduct.m_productAssetType = AZ::AzTypeInfo<PopcornFXAssetPKVF>::Uuid();
			jobProduct.m_dependenciesHandled = true;
		}
	}
	response.m_outputProducts.push_back(jobProduct);
	response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Success;
}

}
