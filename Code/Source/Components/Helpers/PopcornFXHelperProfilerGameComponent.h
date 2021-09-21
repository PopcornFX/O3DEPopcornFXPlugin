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

#if defined(PK_USE_STARTINGPOINTINPUT)
    #include <StartingPointInput/InputEventNotificationBus.h>
#endif
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <PopcornFX/PopcornFXBus.h>


__LMBRPK_BEGIN

class PopcornFXHelperProfilerGameComponent
	: public AZ::Component
	, public AZ::TickBus::Handler
#if defined(PK_USE_STARTINGPOINTINPUT)
	, public StartingPointInput::InputEventNotificationBus::Handler
#endif
{
public:
	AZ_COMPONENT(PopcornFXHelperProfilerGameComponent, "{6395FDB9-46D0-4EF7-99C3-7A032E070B87}");

	PopcornFXHelperProfilerGameComponent();

	void			Activate() override;
	void			Deactivate() override;

	static void		Reflect(AZ::ReflectContext* reflection);

	////////////////////////////////////////////////////////////////////////
	// AZ::TickBus::Handler interface implementation
	virtual void	OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
	////////////////////////////////////////////////////////////////////////

#if defined(PK_USE_STARTINGPOINTINPUT)
	////////////////////////////////////////////////////////////////////////
	// AZ::InputEventNotificationBus::Handler interface implementation
	virtual void	OnPressed(float /*value*/) override;
	////////////////////////////////////////////////////////////////////////
#endif

	static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
	{
		provided.push_back(AZ_CRC("PopcornFXHelperProfilerService"));
	}

	static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
	{
		incompatible.push_back(AZ_CRC("PopcornFXHelperProfilerService"));
	}

	static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& /*required*/)
	{
	}

	static void GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
	{
#if defined(PK_USE_STARTINGPOINTINPUT)
		dependent.push_back(AZ_CRC("InputConfigurationService"));
#endif
    }

	AZ::u32			m_FrameCountToRecord;
	AZStd::string	m_FileName;
	AZStd::string	m_EventName;

protected:
	bool			m_Recording = false;
	AZ::u32			m_FrameCount = 0;
};

__LMBRPK_END
