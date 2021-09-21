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
#include "ScriptCanvas/PopcornFXLibrary.h"

__LMBRPK_BEGIN

	AZ_CVAR(AZ::CVarFixedString, p_PopcornFXVersion, PK_VERSION_CURRENT_STRING,
			nullptr,
			AZ::ConsoleFunctorFlags::DontReplicate & AZ::ConsoleFunctorFlags::ReadOnly,
			"PopcornFX runtime version.");


	void PopcornFXSystemComponent::Reflect(AZ::ReflectContext* context)
	{
		CPopcornFXFeatureProcessor::Reflect(context);

		if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
		{
			serialize->Class<PopcornFXSystemComponent, AZ::Component>()
				->Version(3)
				->Attribute(AZ::Edit::Attributes::SystemComponentTags, AZStd::vector<AZ::Crc32>({ AZ_CRC("AssetBuilder", 0xc739c7d7) }))
				;

			if (AZ::EditContext* ec = serialize->GetEditContext())
			{
				ec->Class<PopcornFXSystemComponent>("PopcornFX", "PopcornFX particle system")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
						->Attribute(AZ::Edit::Attributes::Category, "PopcornFX")
						->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System", 0xc94d118b))
						->Attribute(AZ::Edit::Attributes::AutoExpand, true)
					;
			}
		}

		if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
		{
			behaviorContext->EBus<PopcornFXRequestBus>("PopcornFXRequestBus")
				->Attribute(AZ::Edit::Attributes::Category, "PopcornFX")
				->Event("PreloadEffectByName", &PopcornFXRequestBus::Events::PreloadEffectByName)
				->Event("IsEffectsPreloadingDone", &PopcornFXRequestBus::Events::IsEffectsPreloadingDone)
				->Event("SpawnEmitterAtLocation", &PopcornFXRequestBus::Events::SpawnEmitterAtLocation)
				->Event("SpawnEmitterEntityAtLocation", &PopcornFXRequestBus::Events::SpawnEmitterEntityAtLocation)
				;

			behaviorContext->EBus<PopcornFXProfilerRequestBus>("PopcornFXProfilerRequestBus")
				//->Attribute(AZ::Script::Attributes::ExcludeFrom, AZ::Script::Attributes::ExcludeFlags::Preview)
				->Event("WriteProfileReport", &PopcornFXProfilerRequestBus::Events::WriteProfileReport)
				->Event("ProfilerSetEnable", &PopcornFXProfilerRequestBus::Events::ProfilerSetEnable)
				;
		}
		PopcornFXLibrary::Reflect(context);
	}

	void PopcornFXSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
	{
		provided.push_back(AZ_CRC("PopcornFXService"));
	}

	void PopcornFXSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
	{
		incompatible.push_back(AZ_CRC("PopcornFXService"));
	}

	void PopcornFXSystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
	{
		(void)required;
	}

	void PopcornFXSystemComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
	{
		dependent.push_back(AZ::RHI::Factory::GetComponentService());
		dependent.push_back(AZ_CRC("RPISystem", 0xf2add773));
	}

	void	PopcornFXSystemComponent::Init()
	{
		AZ::EnvironmentVariable<ScriptCanvas::NodeRegistry> nodeRegistryVariable = AZ::Environment::FindVariable<ScriptCanvas::NodeRegistry>(ScriptCanvas::s_nodeRegistryName);
		if (nodeRegistryVariable)
		{
			ScriptCanvas::NodeRegistry& nodeRegistry = nodeRegistryVariable.Get();
			PopcornFXLibrary::InitNodeRegistry(nodeRegistry);
		}
	}

	void PopcornFXSystemComponent::Activate()
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

		AZ::Data::AssetManagerNotificationBus::Handler::BusConnect();

#if defined(LMBR_USE_PK)
		const AzFramework::CommandLine* commandLine = nullptr;
		AzFramework::ApplicationRequests::Bus::BroadcastResult(commandLine, &AzFramework::ApplicationRequests::GetCommandLine);
		if (!commandLine->HasSwitch("ConsoleBatchMode"))
		{
			m_Integration.Activate();
		}
#endif
	}

	void PopcornFXSystemComponent::Deactivate()
	{
#if defined(LMBR_USE_PK)
		const AzFramework::CommandLine* commandLine = nullptr;
		AzFramework::ApplicationRequests::Bus::BroadcastResult(commandLine, &AzFramework::ApplicationRequests::GetCommandLine);
		if (!commandLine->HasSwitch("ConsoleBatchMode"))
		{
			m_Integration.Deactivate();
		}
#endif

		// AssetHandler's destructor calls Unregister()
		m_AssetHandlers.clear();

		AZ::Data::AssetManagerNotificationBus::Handler::BusDisconnect();
	}

__LMBRPK_END
