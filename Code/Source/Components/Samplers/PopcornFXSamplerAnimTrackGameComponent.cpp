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

#include "PopcornFXSamplerAnimTrackGameComponent.h"

#include <AzCore/Component/Entity.h>

__LMBRPK_BEGIN

	void PopcornFXSamplerAnimTrackGameComponent::Activate()
	{
		m_Sampler.m_AttachedToEntityId = m_entity->GetId();
		m_Sampler.Activate();
	}
	
	void PopcornFXSamplerAnimTrackGameComponent::Deactivate()
	{
		m_Sampler.Deactivate();
	}

	void PopcornFXSamplerAnimTrackGameComponent::Reflect(AZ::ReflectContext* reflection)
	{
		PopcornFXSamplerAnimTrack::Reflect(reflection);

		if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection))
		{
			// Serializer:
			serializeContext->Class<PopcornFXSamplerAnimTrackGameComponent, AZ::Component>()
				->Version(1)
				->Field("Sampler", &PopcornFXSamplerAnimTrackGameComponent::m_Sampler)
				;
		}
	}

	void PopcornFXSamplerAnimTrackGameComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
	{
		provided.push_back(AZ_CRC("PopcornFXSamplerAnimTrackService"));
	}

	void PopcornFXSamplerAnimTrackGameComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
	{
		incompatible.push_back(AZ_CRC("PopcornFXEmitterService"));
		incompatible.push_back(AZ_CRC("PopcornFXSamplerImageService"));
		incompatible.push_back(AZ_CRC("PopcornFXSamplerShapeService"));
		incompatible.push_back(AZ_CRC("PopcornFXSamplerAnimTrackService"));
		incompatible.push_back(AZ_CRC("PopcornFXSamplerTextService"));
	}

	void PopcornFXSamplerAnimTrackGameComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
	{
		required.push_back(AZ_CRC("SplineService", 0x2b674d3c));
	}

__LMBRPK_END
