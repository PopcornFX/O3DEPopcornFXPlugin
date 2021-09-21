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

#include "PopcornFXSamplerText.h"

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

__LMBRPK_BEGIN

	class PopcornFXSamplerTextEditorComponent
		: public AzToolsFramework::Components::EditorComponentBase
	{
	public:
		AZ_EDITOR_COMPONENT(PopcornFXSamplerTextEditorComponent, "{742B8AB2-DBBB-424F-AA8B-9BBFC4D8BDB3}");

		virtual void Activate() override;
		virtual void Deactivate() override;

		static void Reflect(AZ::ReflectContext* reflection);
		static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
		static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
		static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

		void BuildGameEntity(AZ::Entity* gameEntity) override;

	protected:
		PopcornFXSamplerText	m_Sampler;
	};

__LMBRPK_END
