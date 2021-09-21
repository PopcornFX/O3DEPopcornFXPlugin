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

#include <AssetBuilderSDK/AssetBuilderSDK.h>

__LMBRPK_BEGIN

class PopcornFXBuilderWorker
	: public AssetBuilderSDK::AssetBuilderCommandBus::Handler
{
public:
	AZ_RTTI(PopcornFXBuilderWorker, "{F42BB484-7EC1-483E-A117-B45603929B6F}");

	PopcornFXBuilderWorker() = default;
	~PopcornFXBuilderWorker() = default;

	void							CreateJobs(const AssetBuilderSDK::CreateJobsRequest& request, AssetBuilderSDK::CreateJobsResponse& response);
	void							ProcessJob(const AssetBuilderSDK::ProcessJobRequest& request, AssetBuilderSDK::ProcessJobResponse& response);
	void							ShutDown() override;
private:
	bool							m_IsShuttingDown = false;
};

__LMBRPK_END
