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

#include "PopcornFXTrackViewAttributeGameComponent.h"

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Asset/AssetSerializer.h>
#include <AzCore/RTTI/BehaviorContext.h>

__LMBRPK_BEGIN

	void	PopcornFXTrackViewAttributeGameComponent::Reflect(AZ::ReflectContext* context)
	{
		PopcornFXTrackViewAttribute::Reflect(context);

		auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);
		if (serializeContext)
		{
			serializeContext->Class<PopcornFXTrackViewAttributeGameComponent, AZ::Component>()
				->Version(1)
				->Field("AttributeName", &PopcornFXTrackViewAttributeGameComponent::m_AttributeName)
				;
		}
	
		auto* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context);
		if (behaviorContext)
		{
			behaviorContext->Class<PopcornFXTrackViewAttributeGameComponent>()->RequestBus("PopcornFXTrackViewComponentRequestBus");
		}
	}
	
	PopcornFXTrackViewAttributeGameComponent::PopcornFXTrackViewAttributeGameComponent()
	{
	}
	
	PopcornFXTrackViewAttributeGameComponent::~PopcornFXTrackViewAttributeGameComponent()
	{
	}
	
	void	PopcornFXTrackViewAttributeGameComponent::Init()
	{
	}
	
	void	PopcornFXTrackViewAttributeGameComponent::Activate()
	{
		m_TrackViewAttribute.Activate(GetEntityId(), m_AttributeName);
	}
	
	void PopcornFXTrackViewAttributeGameComponent::Deactivate()
	{
		m_TrackViewAttribute.Deactivate();
	}

__LMBRPK_END
