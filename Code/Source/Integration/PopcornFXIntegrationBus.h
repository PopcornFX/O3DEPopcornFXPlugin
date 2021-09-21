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

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/EBus/Policies.h>
#include <AzCore/std/string/string.h>
#include <AzCore/Math/Vector3.h>
#include <AzFramework/Physics/Configuration/CollisionConfiguration.h>
#include <AzFramework/Physics/Configuration/RigidBodyConfiguration.h>
#include <AzFramework/Physics/Configuration/SceneConfiguration.h>

#include "Asset/PopcornFXAsset.h"

namespace PopcornFX
{
	class CParticleMediumCollection;
	class CParticleSamplerDescriptor;
}

__LMBRPK_BEGIN

	struct	SPayloadValue
	{
		union
		{
			bool	m_ValueBool[4];
			AZ::u32	m_ValueInt[4];
			float	m_ValueFloat[4];
		};
	};

	class PopcornFXIntegrationRequests
		: public AZ::EBusTraits
	{
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
		// Public functions
		virtual ~PopcornFXIntegrationRequests() {}

		virtual void							ReloadAssets() = 0;
		virtual void							PreloadMesh(const AZStd::string &meshPath) = 0;
		virtual CParticleMediumCollection		*GetMediumCollection() = 0;
		virtual bool							RegisterToBroadcast(AZ::EntityId entityId, const AZStd::string &eventName) = 0;
		virtual bool							UnregisterToBroadcast(AZ::EntityId entityId, const AZStd::string &eventName) = 0;
		virtual const SPayloadValue				*GetCurrentPayloadValue(const AZStd::string &payloadName) const = 0;
#if defined(POPCORNFX_EDITOR)
		virtual AZStd::string					BakeSingleAsset(const AZStd::string &assetPath, const AZStd::string &outDir, const AZStd::string &platform) = 0;
		virtual bool							GatherDependencies(const AZStd::string &assetPath, AZStd::vector<AZStd::string> &dependencies) = 0;
		virtual void							PackChanged(const AZStd::string &packPath, const AZStd::string &libraryPath) = 0;
#endif
	};
	using PopcornFXIntegrationBus = AZ::EBus<PopcornFXIntegrationRequests>;

	class PopcornFXLoadRequests
		: public AZ::EBusTraits
	{
	public:
		using MutexType = AZStd::recursive_mutex;

		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
		// Public functions
		virtual ~PopcornFXLoadRequests() {}

		virtual bool							LoadEffect(PopcornFXAsset* asset, const char *assetPath, const AZ::u8 *assetData, const AZ::IO::SizeType assetDataSize) = 0;
		virtual void							UnloadEffect(PopcornFXAsset *asset) = 0;
	};
	using PopcornFXLoadBus = AZ::EBus<PopcornFXLoadRequests>;

	class PopcornFXSamplerComponentRequests
		: public AZ::ComponentBus
	{
	public:
		virtual CParticleSamplerDescriptor	*GetDescriptor() { return nullptr; };
		virtual AZ::u32						GetType() { return (AZ::u32)-1; };
	};
	using PopcornFXSamplerComponentRequestBus = AZ::EBus <PopcornFXSamplerComponentRequests>;

	class PopcornFXSamplerComponentEvents
		: public AZ::ComponentBus
	{
	public:
		/**
		* Custom connection policy to make sure all we are fully in sync
		*/
		template <class Bus>
		struct PopcornFXSamplerComponentConnectionPolicy
			: public AZ::EBusConnectionPolicy<Bus>
		{
			static void Connect(typename Bus::BusPtr& busPtr, typename Bus::Context& context, typename Bus::HandlerNode& handler, typename Bus::Context::ConnectLockGuard& connectLock, const typename Bus::BusIdType& id = 0)
			{
				AZ::EBusConnectionPolicy<Bus>::Connect(busPtr, context, handler, connectLock, id);

				CParticleSamplerDescriptor	*desc = nullptr;
				EBUS_EVENT_ID_RESULT(desc, id, PopcornFXSamplerComponentRequestBus, GetDescriptor);
				if (desc != nullptr)
					handler->OnSamplerReady(id);
			}
		};
		template<typename Bus>
		using ConnectionPolicy = PopcornFXSamplerComponentConnectionPolicy<Bus>;
		//////////////////////////////////////////////////////////////////////////

		virtual void	OnSamplerReady(const AZ::EntityId&) { }
	};
	using PopcornFXSamplerComponentEventsBus = AZ::EBus <PopcornFXSamplerComponentEvents>;

__LMBRPK_END
