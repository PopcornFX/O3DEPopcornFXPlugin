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

#include <AzCore/Serialization/EditContext.h>
#include <PopcornFX/PopcornFXBus.h>

#include "PopcornFXHelperLoopEmitterEditorComponent.h"

__LMBRPK_BEGIN

PopcornFXHelperLoopEmitterEditorComponent::PopcornFXHelperLoopEmitterEditorComponent()
{
}

void PopcornFXHelperLoopEmitterEditorComponent::Init()
{
	EditorComponentBase::Init();
}

void PopcornFXHelperLoopEmitterEditorComponent::Activate()
{
	EditorComponentBase::Activate();
	if (m_LoopInEditor)
		AZ::TickBus::Handler::BusConnect();
}

void PopcornFXHelperLoopEmitterEditorComponent::Deactivate()
{
	EditorComponentBase::Deactivate();
	if (AZ::TickBus::Handler::BusIsConnected())
		AZ::TickBus::Handler::BusDisconnect();
}

void	PopcornFXHelperLoopEmitterEditorComponent::OnTick(float deltaTime, AZ::ScriptTimePoint time)
{
	(void)time;
	AZ_Assert(m_LoopInEditor, "Should tick only if LoopInEditor is enable");

	bool enabled = false;
	EBUS_EVENT_ID_RESULT(enabled, GetEntityId(), PopcornFXEmitterComponentRequestBus, IsEnabled);
	if (!enabled)
	{
		m_CurrentTime = 0.0f;
		return;
	}

	m_CurrentTime += deltaTime;
	if (m_CurrentTime >= m_Delay)
	{
		EBUS_EVENT_ID(GetEntityId(), PopcornFXEmitterComponentRequestBus, Restart, m_KillOnRestart);
		m_CurrentTime = 0.0f;
	}
}

void PopcornFXHelperLoopEmitterEditorComponent::Reflect(AZ::ReflectContext* context)
{
	if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
	{
		// Serializer:
		serializeContext->Class<PopcornFXHelperLoopEmitterEditorComponent, AzToolsFramework::Components::EditorComponentBase>()
			->Version(1)
			->Field("LoopInEditor", &PopcornFXHelperLoopEmitterEditorComponent::m_LoopInEditor)
			->Field("KillOnRestart", &PopcornFXHelperLoopEmitterEditorComponent::m_KillOnRestart)
			->Field("Delay", &PopcornFXHelperLoopEmitterEditorComponent::m_Delay)
			;

		// Edit context:
		if (AZ::EditContext* editContext = serializeContext->GetEditContext())
		{
			editContext->Class<PopcornFXHelperLoopEmitterEditorComponent>("PopcornFX Helper Loop Emitter", "Helper to make the emitter loop")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
				->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game"))
				->Attribute(AZ::Edit::Attributes::Category, "PopcornFX")
				->Attribute(AZ::Edit::Attributes::AutoExpand, true)
				->DataElement(0, &PopcornFXHelperLoopEmitterEditorComponent::m_LoopInEditor, "LoopInEditor", "Determine if the emitter will loop in editor mode.")
					->Attribute(AZ::Edit::Attributes::ChangeNotify, &PopcornFXHelperLoopEmitterEditorComponent::OnLoopInEditorChanged)
				->DataElement(0, &PopcornFXHelperLoopEmitterEditorComponent::m_KillOnRestart, "KillOnRestart", "Determine if the emitter will be killed or terminated on restart.")
				->DataElement(0, &PopcornFXHelperLoopEmitterEditorComponent::m_Delay, "Delay", "Loop delay in seconds.")
				;
		}
	}
}

AZ::u32 PopcornFXHelperLoopEmitterEditorComponent::OnLoopInEditorChanged()
{
	m_CurrentTime = 0.0f;

	if (m_LoopInEditor)
	{
		if (!AZ::TickBus::Handler::BusIsConnected())
			AZ::TickBus::Handler::BusConnect();
		EBUS_EVENT_ID(GetEntityId(), PopcornFXEmitterComponentRequestBus, Start);
	}
	else if (AZ::TickBus::Handler::BusIsConnected())
		AZ::TickBus::Handler::BusDisconnect();
	
	return AZ::Edit::PropertyRefreshLevels::ValuesOnly;
}

void PopcornFXHelperLoopEmitterEditorComponent::BuildGameEntity(AZ::Entity* gameEntity)
{
	PopcornFXHelperLoopEmitterGameComponent* component = gameEntity->CreateComponent<PopcornFXHelperLoopEmitterGameComponent>();
	if (component)
	{
		component->m_Delay = m_Delay;
		component->m_KillOnRestart = m_KillOnRestart;
	}
}

__LMBRPK_END
