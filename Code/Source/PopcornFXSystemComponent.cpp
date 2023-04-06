//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "PopcornFX_precompiled.h"

#include "PopcornFXSystemComponent.h"

#include <AzFramework/CommandLine/CommandLine.h>
#include <AzFramework/API/ApplicationAPI.h>
#include <AzCore/Console/IConsole.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <Atom/RHI/Factory.h>
#include <PopcornFX/PopcornFXBus.h>

#include "Integration/Render/AtomIntegration/PopcornFXFeatureProcessor.h"
#include "Asset/PopcornFXAsset.h"
#include "Asset/PopcornFXAssetHandler.h"

#include <AutoGenNodeableRegistry.generated.h>
#include <AutoGenGrammarRegistry.generated.h>

REGISTER_SCRIPTCANVAS_AUTOGEN_NODEABLE(PopcornFXStatic);

namespace PopcornFX {

	AZ_CVAR(AZ::CVarFixedString, p_PopcornFXVersion, PK_VERSION_CURRENT_STRING,
			null,
			AZ::ConsoleFunctorFlags::DontReplicate & AZ::ConsoleFunctorFlags::ReadOnly,
			"PopcornFX runtime version.");


	void	PopcornFXSystemComponent::Reflect(AZ::ReflectContext *context)
	{
		CPopcornFXFeatureProcessor::Reflect(context);

		if (AZ::SerializeContext *serialize = azrtti_cast<AZ::SerializeContext*>(context))
		{
			serialize->Class<PopcornFXSystemComponent, AZ::Component>()
				->Version(3)
				->Attribute(AZ::Edit::Attributes::SystemComponentTags, AZStd::vector<AZ::Crc32>({ AZ_CRC("AssetBuilder", 0xc739c7d7) }))
				;

			if (AZ::EditContext *ec = serialize->GetEditContext())
			{
				ec->Class<PopcornFXSystemComponent>("PopcornFX", "PopcornFX particle system")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
						->Attribute(AZ::Edit::Attributes::Category, "PopcornFX")
						->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System", 0xc94d118b))
						->Attribute(AZ::Edit::Attributes::AutoExpand, true)
					;
			}
		}

		if (AZ::BehaviorContext *behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
		{
			behaviorContext->EBus<PopcornFXRequestBus>("PopcornFXRequestBus")
				->Attribute(AZ::Edit::Attributes::Category, "PopcornFX")
				->Event("PreloadEffectByName", &PopcornFXRequestBus::Events::PreloadEffectByName)
				->Event("IsEffectsPreloadingDone", &PopcornFXRequestBus::Events::IsEffectsPreloadingDone)
				->Event("SpawnEmitterAtLocation", &PopcornFXRequestBus::Events::SpawnEmitterAtLocation)
				->Event("SpawnEmitterEntityAtLocation", &PopcornFXRequestBus::Events::SpawnEmitterEntityAtLocation)
				->Event("ExtractPayloadFloat", &PopcornFXRequestBus::Events::ExtractPayloadFloat)
				->Event("ExtractPayloadFloat2", &PopcornFXRequestBus::Events::ExtractPayloadFloat2)
				->Event("ExtractPayloadFloat3", &PopcornFXRequestBus::Events::ExtractPayloadFloat3)
				->Event("ExtractPayloadFloat4", &PopcornFXRequestBus::Events::ExtractPayloadFloat4)
				->Event("ExtractPayloadInt", &PopcornFXRequestBus::Events::ExtractPayloadInt)
				->Event("ExtractPayloadInt2", &PopcornFXRequestBus::Events::ExtractPayloadInt2)
				->Event("ExtractPayloadInt3", &PopcornFXRequestBus::Events::ExtractPayloadInt3)
				->Event("ExtractPayloadInt4", &PopcornFXRequestBus::Events::ExtractPayloadInt4)
				->Event("ExtractPayloadBool", &PopcornFXRequestBus::Events::ExtractPayloadBool)
				->Event("ExtractPayloadOrientation", &PopcornFXRequestBus::Events::ExtractPayloadOrientation)
				;

			behaviorContext->EBus<PopcornFXProfilerRequestBus>("PopcornFXProfilerRequestBus")
				//->Attribute(AZ::Script::Attributes::ExcludeFrom, AZ::Script::Attributes::ExcludeFlags::Preview)
				->Event("WriteProfileReport", &PopcornFXProfilerRequestBus::Events::WriteProfileReport)
				->Event("ProfilerSetEnable", &PopcornFXProfilerRequestBus::Events::ProfilerSetEnable)
				;
		}
	}

	void	PopcornFXSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType &provided)
	{
		provided.push_back(AZ_CRC("PopcornFXService"));
	}

	void	PopcornFXSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType &incompatible)
	{
		incompatible.push_back(AZ_CRC("PopcornFXService"));
	}

	void	PopcornFXSystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType &required)
	{
		AZ_UNUSED(required);
	}

	void	PopcornFXSystemComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType &dependent)
	{
		dependent.push_back(AZ::RHI::Factory::GetComponentService());
		dependent.push_back(AZ_CRC("RPISystem", 0xf2add773));
	}

	void	PopcornFXSystemComponent::Init()
	{
	}

	void	PopcornFXSystemComponent::Activate()
	{
		auto particleAssetHandler = aznew PopcornFXAssetHandler;
		particleAssetHandler->Register(); // registers self with AssetManager
		m_AssetHandlers.emplace_back(particleAssetHandler);

		// Add asset types and extensions to AssetCatalog. Uses "AssetCatalogService".
		auto assetCatalog = AZ::Data::AssetCatalogRequestBus::FindFirstHandler();
		if (assetCatalog)
		{
			assetCatalog->EnableCatalogForAsset(AZ::AzTypeInfo<PopcornFXAsset>::Uuid());
			assetCatalog->AddExtension("pkfx");
		}

#if !defined(POPCORNFX_BUILDER) && defined(O3DE_USE_PK)
		// setup handler for load pass template mappings
		m_LoadTemplatesHandler = AZ::RPI::PassSystemInterface::OnReadyLoadTemplatesEvent::Handler([this]() { this->LoadPassTemplateMappings(); });
		AZ::RPI::PassSystemInterface::Get()->ConnectEvent(m_LoadTemplatesHandler);

		// Register feature processor
		AZ::RPI::FeatureProcessorFactory	*factory = AZ::RPI::FeatureProcessorFactory::Get();
		AZ_Assert(factory, "Cannot get the factory system.");
		factory->RegisterFeatureProcessorWithInterface<CPopcornFXFeatureProcessor, CPopcornFXFeatureProcessorInterface>();
#endif

#if defined(O3DE_USE_PK)
		const AzFramework::CommandLine	*commandLine = null;
		AzFramework::ApplicationRequests::Bus::BroadcastResult(commandLine, &AzFramework::ApplicationRequests::GetCommandLine);
		if (!commandLine->HasSwitch("ConsoleBatchMode"))
		{
			m_Integration.Activate();
		}
#endif
	}

	void	PopcornFXSystemComponent::Deactivate()
	{
#if defined(O3DE_USE_PK)
		const AzFramework::CommandLine	*commandLine = null;
		AzFramework::ApplicationRequests::Bus::BroadcastResult(commandLine, &AzFramework::ApplicationRequests::GetCommandLine);
		if (!commandLine->HasSwitch("ConsoleBatchMode"))
		{
			m_Integration.Deactivate();
		}
#endif

#if !defined(POPCORNFX_BUILDER)
		m_LoadTemplatesHandler.Disconnect();
#endif

		// AssetHandler's destructor calls Unregister()
		m_AssetHandlers.clear();
	}

#if !defined(POPCORNFX_BUILDER)
	void	PopcornFXSystemComponent::LoadPassTemplateMappings()
	{
		const char	*passTemplatesFile = "Passes/PKPassTemplates.azasset";
		AZ::RPI::PassSystemInterface::Get()->LoadPassTemplateMappings(passTemplatesFile);
	}
#endif
}
