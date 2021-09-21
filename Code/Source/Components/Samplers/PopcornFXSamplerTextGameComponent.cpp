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

#include "PopcornFXSamplerTextGameComponent.h"

#include <AzCore/Component/Entity.h>

__LMBRPK_BEGIN

	void PopcornFXSamplerTextGameComponent::Activate()
	{
		m_Sampler.m_AttachedToEntityId = m_entity->GetId();
		m_Sampler.Activate();
	}
	
	void PopcornFXSamplerTextGameComponent::Deactivate()
	{
		m_Sampler.Deactivate();
	}

	void PopcornFXSamplerTextGameComponent::Reflect(AZ::ReflectContext* reflection)
	{
		PopcornFXSamplerText::Reflect(reflection);

		if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection))
		{
			// Serializer:
			serializeContext->Class<PopcornFXSamplerTextGameComponent, AZ::Component>()
				->Version(1)
				->Field("Sampler", &PopcornFXSamplerTextGameComponent::m_Sampler)
				;
		}
	}

	void PopcornFXSamplerTextGameComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
	{
		provided.push_back(AZ_CRC("PopcornFXSamplerTextService"));
	}

	void PopcornFXSamplerTextGameComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
	{
		incompatible.push_back(AZ_CRC("PopcornFXEmitterService"));
		incompatible.push_back(AZ_CRC("PopcornFXSamplerImageService"));
		incompatible.push_back(AZ_CRC("PopcornFXSamplerShapeService"));
		incompatible.push_back(AZ_CRC("PopcornFXSamplerAnimTrackService"));
		incompatible.push_back(AZ_CRC("PopcornFXSamplerTextService"));
	}

	void PopcornFXSamplerTextGameComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
	{
		(void)required;
	}

__LMBRPK_END
