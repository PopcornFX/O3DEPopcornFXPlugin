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

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>
#include <AzCore/Component/Component.h>

#include "PopcornFXTrackViewAttribute.h"

__LMBRPK_BEGIN

	class PopcornFXTrackViewAttributeEditorComponent
		: public AzToolsFramework::Components::EditorComponentBase
		, public PopcornFXEmitterComponentEventsBus::Handler
		, public PopcornFXEmitterEditorComponentEventsBus::Handler
	{
	public:
		AZ_EDITOR_COMPONENT(PopcornFXTrackViewAttributeEditorComponent, "{A699B3E8-126D-4AB9-927E-BD404A7B85E0}")

		PopcornFXTrackViewAttributeEditorComponent();
		~PopcornFXTrackViewAttributeEditorComponent();
	
		// AZ::Component interface implementation
		void Init() override;
		void Activate() override;
		void Deactivate() override;

		void BuildGameEntity(AZ::Entity* gameEntity) override;
	
		static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
		{
			provided.push_back(AZ_CRC("PopcornFXTrackViewAttributeService"));
		}
		static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
		{
			(void)dependent;
		}
		static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
		{
			(void)required;
		}
		static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
		{
			incompatible.push_back(AZ_CRC("PopcornFXTrackViewAttributeService"));
		}
		static void Reflect(AZ::ReflectContext* context);

		// PopcornFXEmitterComponentEventsBus::Handler
		void	OnFxCreated(const AZ::EntityId &entityId) override;

		// PopcornFXEmitterEditorComponentEventsBus::Handler
		void	OnAttributeChanged() override;

	private:
		AZ::u32							OnAttributeNameChanged();
		AZStd::vector<AZStd::string>	GetAttributeNames() const { return m_AttributeNames; }
		AZStd::string					GetAttributeDescription();

		AZStd::string				m_AttributeName;
		AZStd::string				m_AttributeType;

		AZStd::vector<AZStd::string> m_AttributeNames;
		PopcornFXTrackViewAttribute	m_TrackViewAttribute;
	};

__LMBRPK_END
