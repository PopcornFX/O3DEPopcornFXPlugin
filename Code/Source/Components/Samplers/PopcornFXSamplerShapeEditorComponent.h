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

#include "PopcornFXSamplerShape.h"

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

__LMBRPK_BEGIN

	class PopcornFXSamplerShapeEditorComponent
		: public AzToolsFramework::Components::EditorComponentBase
	{
	public:
		AZ_EDITOR_COMPONENT(PopcornFXSamplerShapeEditorComponent, "{4FF5550D-E9BD-4720-98B6-5862BAF3823B}");

		virtual void Activate() override;
		virtual void Deactivate() override;

		static void Reflect(AZ::ReflectContext* reflection);
		static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
		static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
		static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

		void BuildGameEntity(AZ::Entity* gameEntity) override;

	protected:
		PopcornFXSamplerShape	m_Sampler;
	};

__LMBRPK_END
