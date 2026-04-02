//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------

#pragma once

#include "PopcornFXSamplerShape.h"

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

namespace PopcornFX {

	class PopcornFXSamplerShapeEditorComponent
		: public AzToolsFramework::Components::EditorComponentBase
	{
	public:
		AZ_EDITOR_COMPONENT(PopcornFXSamplerShapeEditorComponent, "{4FF5550D-E9BD-4720-98B6-5862BAF3823B}");

		virtual void	Activate() override;
		virtual void	Deactivate() override;

		static void		Reflect(AZ::ReflectContext *reflection);
		static void		GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType &provided);
		static void		GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType &incompatible);
		static void		GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType &required);

		void			BuildGameEntity(AZ::Entity *gameEntity) override;

	protected:
		PopcornFXSamplerShape	m_Sampler;
	};

}
