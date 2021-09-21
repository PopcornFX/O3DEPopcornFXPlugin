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

#include "PopcornFXSamplerImageEditorComponent.h"
#include "PopcornFXSamplerImageGameComponent.h"

#include <AzCore/Serialization/EditContext.h>

__LMBRPK_BEGIN

	void PopcornFXSamplerImageEditorComponent::Activate()
	{
		m_Sampler.m_AttachedToEntityId = m_entity->GetId();
		m_Sampler.Activate();
	}
	
	void PopcornFXSamplerImageEditorComponent::Deactivate()
	{
		m_Sampler.Deactivate();
	}

	void PopcornFXSamplerImageEditorComponent::Reflect(AZ::ReflectContext* reflection)
	{
		if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection))
		{
			// Serializer:
			serializeContext->Class<PopcornFXSamplerImageEditorComponent, AzToolsFramework::Components::EditorComponentBase>()
				->Version(1)
				->Field("Sampler", &PopcornFXSamplerImageEditorComponent::m_Sampler)
				;

			// edit context:
			if (AZ::EditContext* editContext = serializeContext->GetEditContext())
			{
				editContext->Class<PopcornFXSamplerImageEditorComponent>("PopcornFX Sampler Image", "")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "PopcornFX Sampler Image")
						->Attribute(AZ::Edit::Attributes::Category, "PopcornFX")
						->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/PopcornFX_Sampler_Image_Icon.svg")
						->Attribute(AZ::Edit::Attributes::ViewportIcon, "Icons/Components/Viewport/CParticleSamplerTexture.big.png")
						->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game"))
					->DataElement(0, &PopcornFXSamplerImageEditorComponent::m_Sampler, "", "")
						->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
					;
			}
		}
	}

	void PopcornFXSamplerImageEditorComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
	{
		PopcornFXSamplerImageGameComponent::GetProvidedServices(provided);
	}

	void PopcornFXSamplerImageEditorComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
	{
		PopcornFXSamplerImageGameComponent::GetIncompatibleServices(incompatible);
	}

	void PopcornFXSamplerImageEditorComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
	{
		PopcornFXSamplerImageGameComponent::GetRequiredServices(required);
	}

	void PopcornFXSamplerImageEditorComponent::BuildGameEntity(AZ::Entity* gameEntity)
	{
		PopcornFXSamplerImageGameComponent* component = gameEntity->CreateComponent<PopcornFXSamplerImageGameComponent>();

		if (component)
		{
			component->m_Sampler.CopyFrom(m_Sampler);
		}
	}

__LMBRPK_END
