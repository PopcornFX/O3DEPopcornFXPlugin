//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "PopcornFX_precompiled.h"

#include <AzCore/Serialization/EditContext.h>
#include "PopcornFXAssetBuilderComponent.h"
#include "Integration/PopcornFXIntegrationBus.h"

namespace PopcornFX {

void	PopcornFXBuilderComponent::Reflect(AZ::ReflectContext *context)
{
	if (AZ::SerializeContext *serialize = azrtti_cast<AZ::SerializeContext*>(context))
	{
		serialize->Class<PopcornFXBuilderComponent, AZ::Component>()
			->Version(0)
			->Attribute(AZ::Edit::Attributes::SystemComponentTags, AZStd::vector<AZ::Crc32>({ AssetBuilderSDK::ComponentTags::AssetBuilder }))
			;
	}
}

void	PopcornFXBuilderComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType &provided)
{
	provided.push_back(AZ_CRC("PopcornFXBuilderPluginService", 0x1380f480));
}

void PopcornFXBuilderComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType &incompatible)
{
	incompatible.push_back(AZ_CRC("PopcornFXBuilderPluginService", 0x1380f480));
}

void PopcornFXBuilderComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType &required)
{
	required.push_back(AZ_CRC("PopcornFXService"));
}

void PopcornFXBuilderComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType &dependent)
{
	AZ_UNUSED(dependent);
}

PopcornFXBuilderComponent::PopcornFXBuilderComponent()
{
}

PopcornFXBuilderComponent::~PopcornFXBuilderComponent()
{
}

void	PopcornFXBuilderComponent::Init()
{
}

//----------------------------------------------------------------------------------------------------------//
//																											//
//											PopcornFXBuilderComponent										//
//																											//
//----------------------------------------------------------------------------------------------------------//

void	PopcornFXBuilderComponent::Activate()
{
	AZ_Info("PopcornFX", "Asset builder activated.");
	AssetBuilderSDK::AssetBuilderDesc builderDescriptor;
	builderDescriptor.m_name = "PopcornFX Worker Builder";
	builderDescriptor.m_version = 2;
	builderDescriptor.m_patterns.emplace_back(AssetBuilderSDK::AssetBuilderPattern("*.pkfx", AssetBuilderSDK::AssetBuilderPattern::PatternType::Wildcard));
	builderDescriptor.m_patterns.emplace_back(AssetBuilderSDK::AssetBuilderPattern("*.fga", AssetBuilderSDK::AssetBuilderPattern::PatternType::Wildcard));

	builderDescriptor.m_busId = azrtti_typeid<PopcornFXBuilderWorker>();
	builderDescriptor.m_createJobFunction = AZStd::bind(&PopcornFXBuilderWorker::CreateJobs, &m_PKBuilder, AZStd::placeholders::_1, AZStd::placeholders::_2);
	builderDescriptor.m_processJobFunction = AZStd::bind(&PopcornFXBuilderWorker::ProcessJob, &m_PKBuilder, AZStd::placeholders::_1, AZStd::placeholders::_2);

	m_PKBuilder.BusConnect(builderDescriptor.m_busId);

	AssetBuilderSDK::AssetBuilderBus::Broadcast(&AssetBuilderSDK::AssetBuilderBusTraits::RegisterBuilderInformation, builderDescriptor);
	PopcornFX::PopcornFXIntegrationBus::Broadcast(&PopcornFX::PopcornFXIntegrationBus::Handler::SetBakingThreadpool);
}

void	PopcornFXBuilderComponent::Deactivate()
{
	m_PKBuilder.BusDisconnect();
}

//-------------------------------------------------------------------------------------------------------------

}
