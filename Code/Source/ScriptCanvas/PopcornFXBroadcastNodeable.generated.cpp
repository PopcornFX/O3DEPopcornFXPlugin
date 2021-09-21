/*
 * Copyright (c) Contributors to the Open 3D Engine Project. For complete copyright and license terms please see the LICENSE at the root of this distribution..
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 * This file is generated automatically at compile time, DO NOT EDIT BY HAND
 * Template Source C:/o3de/Gems/ScriptCanvas/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNodeable_Source.jinja; Data Sources C:\o3de\Gems\PopcornFX\Code\Source\ScriptCanvas\PopcornFXBroadcastNodeable.ScriptCanvasNodeable.xml
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This code was produced with AzAutoGen, any modifications made will not be preserved.
// If you need to modify this code see:
//
// Gems\ScriptCanvas\Code\Include\ScriptCanvas\AutoGen\ScriptCanvasNodeable_Source.jinja
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <ScriptCanvas/Core/Contracts.h>
#include <ScriptCanvas/Core/NodeableNode.h>
#include <ScriptCanvas/Core/SlotExecutionMap.h>
#include <ScriptCanvas/Grammar/ParsingUtilities.h>
#include <ScriptCanvas/Utils/VersionConverters.h>


#include <Source/ScriptCanvas/PopcornFXBroadcastNodeable.h>











namespace LmbrPk 
{
    // ExecutionOuts begin
void LmbrPk::PopcornFXBroadcastNodeable::CallBroadcast() {
    ExecutionOut(0); // Broadcast
}

 size_t LmbrPk::PopcornFXBroadcastNodeable::GetRequiredOutCount() const { return 1; }// ExecutionOuts end




void LmbrPk::PopcornFXBroadcastNodeable::Reflect(AZ::ReflectContext* context)
{
    using namespace ScriptCanvas;

    if (AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
    {
   serializeContext->Class<PopcornFXBroadcastNodeable, ScriptCanvas::Nodeable>()

        ;


        if (AZ::EditContext* editContext = serializeContext->GetEditContext())
        {
   editContext->Class<LmbrPk::PopcornFXBroadcastNodeable>("PopcornFXBroadcast", "PopcornFX Broadcast")
                       ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                       ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                        ->Attribute(AZ::Edit::Attributes::Icon, "Icons/ScriptCanvas/Bus.png")                            ;
            }
        }

    // Behavior Context Reflection
    if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
    {
    behaviorContext->Class<PopcornFXBroadcastNodeable>("PopcornFXBroadcastNodeable")
            ->Attribute(AZ::Script::Attributes::ExcludeFrom, AZ::Script::Attributes::ExcludeFlags::List)
            ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
            // Connect Event
            ->Method(Grammar::ToIdentifier("Connect Event").c_str(), &PopcornFXBroadcastNodeable::ConnectEvent)
        ;
    }
}

ScriptCanvas::NodePropertyInterface* LmbrPk::PopcornFXBroadcastNodeable::GetPropertyInterface([[maybe_unused]] AZ::Crc32 propertyId)
{
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////

Nodes::PopcornFXBroadcastNodeableNode::PopcornFXBroadcastNodeableNode() 
{ 
    SetNodeable(AZStd::make_unique<LmbrPk::PopcornFXBroadcastNodeable>()); 
}

void Nodes::PopcornFXBroadcastNodeableNode::Reflect(AZ::ReflectContext* context)
{
    LmbrPk::PopcornFXBroadcastNodeable::Reflect(context);

    // Serialization Context Reflection
    if (AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
    {
        serializeContext->Class<PopcornFXBroadcastNodeableNode, NodeableNode>()
            ->Version(0)
            ;

        if (AZ::EditContext* editContext = serializeContext->GetEditContext())
        {
            editContext->Class<PopcornFXBroadcastNodeableNode>("PopcornFXBroadcast", "PopcornFX Broadcast")
                ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                ;
        }
    }
}

size_t Nodes::PopcornFXBroadcastNodeableNode::GenerateFingerprint() const
{
    using namespace ScriptCanvas;

    size_t hash = 0;
            // Connect Event
                AZStd::hash_combine(hash, Grammar::ToIdentifier("Connect Event"));

                AZStd::hash_combine(hash, azrtti_typeid<AZStd::string>());
                AZStd::hash_combine(hash, azrtti_typeid<AZ::EntityId>());
                    // Broadcast
                AZStd::hash_combine(hash, Grammar::ToIdentifier("Broadcast"));

            
    return hash;
}

void Nodes::PopcornFXBroadcastNodeableNode::ConfigureVisualExtensions()
{
    OnConfigureVisualExtensions();
}

// Configure Slots and Build the Nodeable Execution Maps
void Nodes::PopcornFXBroadcastNodeableNode::ConfigureSlots()
{
    using namespace ScriptCanvas;
    NodeableNode::ConfigureSlots();
    SlotExecution::Ins ins;
    SlotExecution::Outs outs;    // execution in begin 
    { 
        SlotExecution::In in;

        // ExecutionInput - Connect Event
        ScriptCanvas::ExecutionSlotConfiguration slotConfigurationInput; 
        slotConfigurationInput.m_name = "Connect Event";
        slotConfigurationInput.m_toolTip = "Connect to FX event name.";
        slotConfigurationInput.SetConnectionType(ScriptCanvas::ConnectionType::Input);
        slotConfigurationInput.m_displayGroup = "Connect Event";


        SlotId slotIdConnectEventInput = AddSlot(slotConfigurationInput);
        AZ_Assert(slotIdConnectEventInput.IsValid(), "ExecutionInput slot: Connect Event was not created successfully.");

        in.slotId = slotIdConnectEventInput;

        
        // Data Slot: Event Name (Input)
        {

            ScriptCanvas::DataSlotConfiguration dataSlotConfiguration;
            dataSlotConfiguration.m_name = "Event Name";
            dataSlotConfiguration.m_toolTip = "The broadcasted event name we listen to.";
            dataSlotConfiguration.m_displayGroup = "Connect Event";
            dataSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Input);
            dataSlotConfiguration.SetAZType<AZStd::string>();
            SlotId slotId = AddSlot(dataSlotConfiguration);
            AZ_Assert(slotId.IsValid(), "Data slot is not created successfully.");
            in.inputs.push_back(slotId);
        }
        // Data Slot: Event Name...


        
        // Data Slot: Entity Id (Input)
        {

            ScriptCanvas::DataSlotConfiguration dataSlotConfiguration;
            dataSlotConfiguration.m_name = "Entity Id";
            dataSlotConfiguration.m_toolTip = "The entity id of the effect.";
            dataSlotConfiguration.m_displayGroup = "Connect Event";
            dataSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Input);
            dataSlotConfiguration.SetAZType<AZ::EntityId>();
            SlotId slotId = AddSlot(dataSlotConfiguration);
            AZ_Assert(slotId.IsValid(), "Data slot is not created successfully.");
            in.inputs.push_back(slotId);
        }
        // Data Slot: Entity Id...


        { // execution outs begin    
// NO branches were found, at the default
            SlotExecution::Out out;
            ScriptCanvas::ExecutionSlotConfiguration outSlotConfiguration;
            outSlotConfiguration.m_name = "On Connect Event";
            outSlotConfiguration.m_toolTip = "Connect to FX event name.";
            outSlotConfiguration.m_displayGroup = "Connect Event";
            outSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
            outSlotConfiguration.m_isLatent = false;
            out.name = outSlotConfiguration.m_name;
            SlotId outSlotId = AddSlot(outSlotConfiguration);
            AZ_Assert(outSlotId.IsValid(), "ExecutionOutput slot is not created successfully.");
            out.slotId = outSlotId;
            in.outs.push_back(out);
        } // execution outs end
        ins.push_back(in);
    } // execution in end
  
    // Latent out
    {
        SlotExecution::Out out;
        // ExecutionOutput - Broadcast
        ScriptCanvas::ExecutionSlotConfiguration slotConfigurationOutput; 
        slotConfigurationOutput.m_name = "Broadcast";
        slotConfigurationOutput.m_toolTip = "Signal triggered when the FX broadcast an event.";
        slotConfigurationOutput.SetConnectionType(ScriptCanvas::ConnectionType::Output);
        slotConfigurationOutput.m_displayGroup = "Broadcast";
        slotConfigurationOutput.m_isLatent = true;


        SlotId slotIdBroadcastOutput = AddSlot(slotConfigurationOutput);
        AZ_Assert(slotIdBroadcastOutput.IsValid(), "ExecutionOutput slot: Broadcast was not created successfully.");

        out.slotId = slotIdBroadcastOutput;
        out.name = "Broadcast";
        outs.push_back(out);
    }


    // Generate the execution map
    m_slotExecutionMap = SlotExecution::Map(AZStd::move(ins), AZStd::move(outs));

}


}


