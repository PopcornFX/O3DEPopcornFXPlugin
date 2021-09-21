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
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <AzCore/IO/FileIO.h>

#include "PopcornFXHelperProfilerEditorComponent.h"

__LMBRPK_BEGIN

PopcornFXHelperProfilerEditorComponent::PopcornFXHelperProfilerEditorComponent()
{
}

void PopcornFXHelperProfilerEditorComponent::Activate()
{
	EditorComponentBase::Activate();
	AZ::TickBus::Handler::BusConnect();
}

void PopcornFXHelperProfilerEditorComponent::Deactivate()
{
	EditorComponentBase::Deactivate();
	if (AZ::TickBus::Handler::BusIsConnected())
		AZ::TickBus::Handler::BusDisconnect();
}

void	PopcornFXHelperProfilerEditorComponent::OnTick(float deltaTime, AZ::ScriptTimePoint time)
{
	(void)deltaTime; (void)time;

	if (m_Recording)
	{
		m_FrameCount++;
		if (m_FrameCount >= m_FrameCountToRecord)
		{
			AZ_Printf("PopcorFX", "Stopping the profiler. ");

			bool	writeOk = false;
			PopcornFXProfilerRequestBus::BroadcastResult(writeOk, &PopcornFXProfilerRequestBus::Events::WriteProfileReport, m_FileName);
			if (writeOk)
			{
				AZ_Printf("PopcorFX", "Profile report wrote to %s.", m_FileName.c_str());
			}
			else
			{
				AZ_Printf("PopcorFX", "Could not open file %s to write profile report.", m_FileName.c_str());
			}

			PopcornFXProfilerRequestBus::Broadcast(&PopcornFXProfilerRequestBus::Events::ProfilerSetEnable, false);
			m_Recording = false;
			m_FrameCount = 0;
			EBUS_EVENT(AzToolsFramework::ToolsApplicationEvents::Bus, InvalidatePropertyDisplay, AzToolsFramework::Refresh_EntireTree);
		}
	}
}

void	PopcornFXHelperProfilerEditorComponent::Reflect(AZ::ReflectContext* context)
{
	if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
	{
		// Serializer:
		serializeContext->Class<PopcornFXHelperProfilerEditorComponent, AzToolsFramework::Components::EditorComponentBase>()
			->Version(1)
			->Field("RecordButton", &PopcornFXHelperProfilerEditorComponent::m_RecordButton)
			->Field("OpenButton", &PopcornFXHelperProfilerEditorComponent::m_OpenButton)
			->Field("FrameCountToRecord", &PopcornFXHelperProfilerEditorComponent::m_FrameCountToRecord)
			->Field("FilePath", &PopcornFXHelperProfilerEditorComponent::m_FileName)
			->Field("EventName", &PopcornFXHelperProfilerEditorComponent::m_EventName)
			;

		// Edit context:
		if (AZ::EditContext* editContext = serializeContext->GetEditContext())
		{
			editContext->Class<PopcornFXHelperProfilerEditorComponent>("PopcornFX Helper Profiler", "Helper to profile")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
				->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game"))
				->Attribute(AZ::Edit::Attributes::Category, "PopcornFX")
				->Attribute(AZ::Edit::Attributes::AutoExpand, true)
				->DataElement(AZ::Edit::UIHandlers::Button, &PopcornFXHelperProfilerEditorComponent::m_RecordButton, "", "")
					->Attribute(AZ::Edit::Attributes::ChangeNotify, &PopcornFXHelperProfilerEditorComponent::OnRecordChanged)
					->Attribute(AZ::Edit::Attributes::ButtonText, "Record")
				->DataElement(AZ::Edit::UIHandlers::Button, &PopcornFXHelperProfilerEditorComponent::m_OpenButton, "", "")
					->Attribute(AZ::Edit::Attributes::ChangeNotify, &PopcornFXHelperProfilerEditorComponent::OnOpenChanged)
					->Attribute(AZ::Edit::Attributes::Visibility, &PopcornFXHelperProfilerEditorComponent::GetOpenVisibility)
					->Attribute(AZ::Edit::Attributes::ButtonText, "Open")
				->DataElement(0, &PopcornFXHelperProfilerEditorComponent::m_FrameCountToRecord, "FrameCountToRecord", "How many frames will be recorded.")
				->DataElement(0, &PopcornFXHelperProfilerEditorComponent::m_FileName, "FilePath", "Path for the profile report.")
				->DataElement(0, &PopcornFXHelperProfilerEditorComponent::m_EventName, "In-game EventName", "Event to start the profiler.")
				;
		}
	}
}

AZ::u32 PopcornFXHelperProfilerEditorComponent::OnRecordChanged()
{
	if (!m_Recording)
	{
		PopcornFXProfilerRequestBus::Broadcast(&PopcornFXProfilerRequestBus::Events::ProfilerSetEnable, true);
		m_Recording = true;
		AZ_Printf("PopcorFX", "Starting the profiler.");
	}
	else
	{
		AZ_Printf("PopcorFX", "The profiler is already running.");
	}
	return AZ::Edit::PropertyRefreshLevels::AttributesAndValues;
}

AZ::u32 PopcornFXHelperProfilerEditorComponent::OnOpenChanged()
{
	AZ_Error("PopcornFX", false, "Open not implemented yep. Please open the pkpr file with the PopcornFX Editor.")
	return AZ::Edit::PropertyRefreshLevels::None;
}

AZ::Crc32	PopcornFXHelperProfilerEditorComponent::GetOpenVisibility()
{
	if (AZ::IO::FileIOBase::GetInstance()->Exists(m_FileName.c_str()))
		return AZ::Edit::PropertyVisibility::Show;
	return AZ::Edit::PropertyVisibility::Hide;
}

void PopcornFXHelperProfilerEditorComponent::BuildGameEntity(AZ::Entity* gameEntity)
{
	PopcornFXHelperProfilerGameComponent* component = gameEntity->CreateComponent<PopcornFXHelperProfilerGameComponent>();
	if (component)
	{
		component->m_FrameCountToRecord = m_FrameCountToRecord;
		component->m_FileName = m_FileName;
		component->m_EventName = m_EventName;
	}
}

__LMBRPK_END
