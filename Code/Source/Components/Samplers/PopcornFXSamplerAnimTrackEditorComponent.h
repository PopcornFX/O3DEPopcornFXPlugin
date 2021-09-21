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

#include "PopcornFXSamplerAnimTrack.h"

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

__LMBRPK_BEGIN

	class PopcornFXSamplerAnimTrackEditorComponent
		: public AzToolsFramework::Components::EditorComponentBase
	{
	public:
		AZ_EDITOR_COMPONENT(PopcornFXSamplerAnimTrackEditorComponent, "{9237AEB0-968E-4B36-8BB8-C7DEFB65D505}");

		virtual void Activate() override;
		virtual void Deactivate() override;

		static void Reflect(AZ::ReflectContext* reflection);
		static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
		static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
		static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

		void BuildGameEntity(AZ::Entity* gameEntity) override;

	protected:
		PopcornFXSamplerAnimTrack	m_Sampler;
	};

__LMBRPK_END
