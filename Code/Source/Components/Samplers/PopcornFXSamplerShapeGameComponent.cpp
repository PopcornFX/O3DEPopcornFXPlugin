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

#include "PopcornFXSamplerShapeGameComponent.h"

#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/EditContext.h>

__LMBRPK_BEGIN

	void PopcornFXSamplerShapeGameComponent::Activate()
	{
		m_Sampler.m_AttachedToEntityId = m_entity->GetId();
		m_Sampler.Activate();
	}

	void PopcornFXSamplerShapeGameComponent::Deactivate()
	{
		m_Sampler.Deactivate();
	}

	void PopcornFXSamplerShapeGameComponent::Reflect(AZ::ReflectContext* reflection)
	{
		PopcornFXSamplerShape::Reflect(reflection);

		if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection))
		{
			// Serializer:
			serializeContext->Class<PopcornFXSamplerShapeGameComponent, AZ::Component>()
				->Version(1)
				->Field("Sampler", &PopcornFXSamplerShapeGameComponent::m_Sampler)
				;
		}
	}

	void PopcornFXSamplerShapeGameComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
	{
		provided.push_back(AZ_CRC("PopcornFXSamplerShapeService"));
	}

	void PopcornFXSamplerShapeGameComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
	{
		incompatible.push_back(AZ_CRC("PopcornFXEmitterService"));
		incompatible.push_back(AZ_CRC("PopcornFXSamplerImageService"));
		incompatible.push_back(AZ_CRC("PopcornFXSamplerShapeService"));
		incompatible.push_back(AZ_CRC("PopcornFXSamplerAnimTrackService"));
		incompatible.push_back(AZ_CRC("PopcornFXSamplerTextService"));
	}

	void PopcornFXSamplerShapeGameComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
	{
		(void)required;
	}

	void PopcornFXSamplerShapeGameComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
	{
		dependent.push_back(AZ_CRC("MeshService"));
		dependent.push_back(AZ_CRC("EMotionFXActorService"));
	}

__LMBRPK_END
