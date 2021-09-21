/*
 * Copyright (c) Contributors to the Open 3D Engine Project. For complete copyright and license terms please see the LICENSE at the root of this distribution..
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 * This file is generated automatically at compile time, DO NOT EDIT BY HAND
 * Template Source C:/o3de/Gems/ScriptCanvas/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNodeable_Source.jinja; Data Sources C:\o3de\Gems\PopcornFX\Code\Source\ScriptCanvas\PopcornFXExtractPayloadNodeable.ScriptCanvasNodeable.xml
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


#include <Source/ScriptCanvas/PopcornFXExtractPayloadNodeable.h>











namespace LmbrPk 
{
// ExecutionOuts begin
void LmbrPk::PopcornFXExtractPayloadNodeable::CallFloat([[maybe_unused]] float FloatOutput) {
    ExecutionOut(0, FloatOutput); // Float
}
void LmbrPk::PopcornFXExtractPayloadNodeable::CallFloat2([[maybe_unused]] AZ::Vector2 Float2Output) {
    ExecutionOut(1, Float2Output); // Float2
}
void LmbrPk::PopcornFXExtractPayloadNodeable::CallFloat3([[maybe_unused]] AZ::Vector3 Float3Output) {
    ExecutionOut(2, Float3Output); // Float3
}
void LmbrPk::PopcornFXExtractPayloadNodeable::CallFloat4([[maybe_unused]] AZ::Vector4 Float4Output) {
    ExecutionOut(3, Float4Output); // Float4
}
void LmbrPk::PopcornFXExtractPayloadNodeable::CallInt([[maybe_unused]] float IntOutput) {
    ExecutionOut(4, IntOutput); // Int
}
void LmbrPk::PopcornFXExtractPayloadNodeable::CallInt2([[maybe_unused]] AZ::Vector2 Int2Output) {
    ExecutionOut(5, Int2Output); // Int2
}
void LmbrPk::PopcornFXExtractPayloadNodeable::CallInt3([[maybe_unused]] AZ::Vector3 Int3Output) {
    ExecutionOut(6, Int3Output); // Int3
}
void LmbrPk::PopcornFXExtractPayloadNodeable::CallInt4([[maybe_unused]] AZ::Vector4 Int4Output) {
    ExecutionOut(7, Int4Output); // Int4
}
void LmbrPk::PopcornFXExtractPayloadNodeable::CallBool([[maybe_unused]] bool BoolOutput) {
    ExecutionOut(8, BoolOutput); // Bool
}
void LmbrPk::PopcornFXExtractPayloadNodeable::CallOrientation([[maybe_unused]] AZ::Quaternion OrientationOutput) {
    ExecutionOut(9, OrientationOutput); // Orientation
}

 size_t LmbrPk::PopcornFXExtractPayloadNodeable::GetRequiredOutCount() const { return 10; }// ExecutionOuts end




void LmbrPk::PopcornFXExtractPayloadNodeable::Reflect(AZ::ReflectContext* context)
{
    using namespace ScriptCanvas;

    if (AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
    {
   serializeContext->Class<PopcornFXExtractPayloadNodeable, ScriptCanvas::Nodeable>()

        ;


        if (AZ::EditContext* editContext = serializeContext->GetEditContext())
        {
   editContext->Class<LmbrPk::PopcornFXExtractPayloadNodeable>("PopcornFXExtractPayload", "PopcornFX Extract Payload")
                       ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                       ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                        ->Attribute(AZ::Edit::Attributes::Icon, "Icons/ScriptCanvas/Bus.png")                            ;
            }
        }

    // Behavior Context Reflection
    if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
    {
    behaviorContext->Class<PopcornFXExtractPayloadNodeable>("PopcornFXExtractPayloadNodeable")
            ->Attribute(AZ::Script::Attributes::ExcludeFrom, AZ::Script::Attributes::ExcludeFlags::List)
            ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
            // Extract Payload
            ->Method(Grammar::ToIdentifier("Extract Payload").c_str(), &PopcornFXExtractPayloadNodeable::ExtractPayload)
        ;
    }
}

ScriptCanvas::NodePropertyInterface* LmbrPk::PopcornFXExtractPayloadNodeable::GetPropertyInterface([[maybe_unused]] AZ::Crc32 propertyId)
{
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////

Nodes::PopcornFXExtractPayloadNodeableNode::PopcornFXExtractPayloadNodeableNode() 
{ 
    SetNodeable(AZStd::make_unique<LmbrPk::PopcornFXExtractPayloadNodeable>()); 
}

void Nodes::PopcornFXExtractPayloadNodeableNode::Reflect(AZ::ReflectContext* context)
{
    LmbrPk::PopcornFXExtractPayloadNodeable::Reflect(context);

    // Serialization Context Reflection
    if (AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
    {
        serializeContext->Class<PopcornFXExtractPayloadNodeableNode, NodeableNode>()
            ->Version(0)
            ;

        if (AZ::EditContext* editContext = serializeContext->GetEditContext())
        {
            editContext->Class<PopcornFXExtractPayloadNodeableNode>("PopcornFXExtractPayload", "PopcornFX Extract Payload")
                ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                ;
        }
    }
}

size_t Nodes::PopcornFXExtractPayloadNodeableNode::GenerateFingerprint() const
{
    using namespace ScriptCanvas;

    size_t hash = 0;
            // Extract Payload
                AZStd::hash_combine(hash, Grammar::ToIdentifier("Extract Payload"));

                AZStd::hash_combine(hash, azrtti_typeid<AZStd::string>());
                AZStd::hash_combine(hash, azrtti_typeid<AZStd::string>());
            
    return hash;
}

void Nodes::PopcornFXExtractPayloadNodeableNode::ConfigureVisualExtensions()
{
    OnConfigureVisualExtensions();
}

// Configure Slots and Build the Nodeable Execution Maps
void Nodes::PopcornFXExtractPayloadNodeableNode::ConfigureSlots()
{
    using namespace ScriptCanvas;
    NodeableNode::ConfigureSlots();
    SlotExecution::Ins ins;
    SlotExecution::Outs outs;    // execution in begin 
    { 
        SlotExecution::In in;

        // ExecutionInput - Extract Payload
        ScriptCanvas::ExecutionSlotConfiguration slotConfigurationInput; 
        slotConfigurationInput.m_name = "Extract Payload";
        slotConfigurationInput.m_toolTip = "";
        slotConfigurationInput.SetConnectionType(ScriptCanvas::ConnectionType::Input);
        slotConfigurationInput.m_displayGroup = "Extract Payload";


        SlotId slotIdExtractPayloadInput = AddSlot(slotConfigurationInput);
        AZ_Assert(slotIdExtractPayloadInput.IsValid(), "ExecutionInput slot: Extract Payload was not created successfully.");

        in.slotId = slotIdExtractPayloadInput;

        
        // Data Slot: Payload Name (Input)
        {

            ScriptCanvas::DataSlotConfiguration dataSlotConfiguration;
            dataSlotConfiguration.m_name = "Payload Name";
            dataSlotConfiguration.m_toolTip = "The payload name contained in the broadcasted event.";
            dataSlotConfiguration.m_displayGroup = "Extract Payload";
            dataSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Input);
            dataSlotConfiguration.SetAZType<AZStd::string>();
            SlotId slotId = AddSlot(dataSlotConfiguration);
            AZ_Assert(slotId.IsValid(), "Data slot is not created successfully.");
            in.inputs.push_back(slotId);
        }
        // Data Slot: Payload Name...


        
        // Data Slot: Payload Type (Input)
        {

            ScriptCanvas::DataSlotConfiguration dataSlotConfiguration;
            dataSlotConfiguration.m_name = "Payload Type";
            dataSlotConfiguration.m_toolTip = "The payload type contained in the broadcasted event.";
            dataSlotConfiguration.m_displayGroup = "Extract Payload";
            dataSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Input);
            dataSlotConfiguration.SetAZType<AZStd::string>();
            SlotId slotId = AddSlot(dataSlotConfiguration);
            AZ_Assert(slotId.IsValid(), "Data slot is not created successfully.");
            in.inputs.push_back(slotId);
        }
        // Data Slot: Payload Type...


        { // execution outs begin    
// branches were found
            {
                SlotExecution::Out out;
                ScriptCanvas::ExecutionSlotConfiguration outSlotConfiguration;
                outSlotConfiguration.m_name = "Float";
                outSlotConfiguration.m_toolTip = "";
                outSlotConfiguration.m_displayGroup = "Extract Payload";
                outSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
                outSlotConfiguration.m_isLatent = false;
                out.name = outSlotConfiguration.m_name;
                SlotId outSlotId = AddSlot(outSlotConfiguration);
                AZ_Assert(outSlotId.IsValid(), "ExecutionOutput slot is not created successfully.");
                out.slotId = outSlotId;
                    
        // Data Slot: FloatOutput (Output)
        {

            ScriptCanvas::DataSlotConfiguration dataSlotConfiguration;
            dataSlotConfiguration.m_name = "FloatOutput";
            dataSlotConfiguration.m_toolTip = "";
            dataSlotConfiguration.m_displayGroup = "Extract Payload";
            dataSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
            dataSlotConfiguration.SetAZType<float>();
            SlotId slotId = AddSlot(dataSlotConfiguration);
            AZ_Assert(slotId.IsValid(), "Data slot is not created successfully.");
            out.outputs.push_back(slotId);
        }
        // Data Slot: FloatOutput...


                in.outs.push_back(out);
            }
            {
                SlotExecution::Out out;
                ScriptCanvas::ExecutionSlotConfiguration outSlotConfiguration;
                outSlotConfiguration.m_name = "Float2";
                outSlotConfiguration.m_toolTip = "";
                outSlotConfiguration.m_displayGroup = "Extract Payload";
                outSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
                outSlotConfiguration.m_isLatent = false;
                out.name = outSlotConfiguration.m_name;
                SlotId outSlotId = AddSlot(outSlotConfiguration);
                AZ_Assert(outSlotId.IsValid(), "ExecutionOutput slot is not created successfully.");
                out.slotId = outSlotId;
                    
        // Data Slot: Float2Output (Output)
        {

            ScriptCanvas::DataSlotConfiguration dataSlotConfiguration;
            dataSlotConfiguration.m_name = "Float2Output";
            dataSlotConfiguration.m_toolTip = "";
            dataSlotConfiguration.m_displayGroup = "Extract Payload";
            dataSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
            dataSlotConfiguration.SetAZType<AZ::Vector2>();
            SlotId slotId = AddSlot(dataSlotConfiguration);
            AZ_Assert(slotId.IsValid(), "Data slot is not created successfully.");
            out.outputs.push_back(slotId);
        }
        // Data Slot: Float2Output...


                in.outs.push_back(out);
            }
            {
                SlotExecution::Out out;
                ScriptCanvas::ExecutionSlotConfiguration outSlotConfiguration;
                outSlotConfiguration.m_name = "Float3";
                outSlotConfiguration.m_toolTip = "";
                outSlotConfiguration.m_displayGroup = "Extract Payload";
                outSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
                outSlotConfiguration.m_isLatent = false;
                out.name = outSlotConfiguration.m_name;
                SlotId outSlotId = AddSlot(outSlotConfiguration);
                AZ_Assert(outSlotId.IsValid(), "ExecutionOutput slot is not created successfully.");
                out.slotId = outSlotId;
                    
        // Data Slot: Float3Output (Output)
        {

            ScriptCanvas::DataSlotConfiguration dataSlotConfiguration;
            dataSlotConfiguration.m_name = "Float3Output";
            dataSlotConfiguration.m_toolTip = "";
            dataSlotConfiguration.m_displayGroup = "Extract Payload";
            dataSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
            dataSlotConfiguration.SetAZType<AZ::Vector3>();
            SlotId slotId = AddSlot(dataSlotConfiguration);
            AZ_Assert(slotId.IsValid(), "Data slot is not created successfully.");
            out.outputs.push_back(slotId);
        }
        // Data Slot: Float3Output...


                in.outs.push_back(out);
            }
            {
                SlotExecution::Out out;
                ScriptCanvas::ExecutionSlotConfiguration outSlotConfiguration;
                outSlotConfiguration.m_name = "Float4";
                outSlotConfiguration.m_toolTip = "";
                outSlotConfiguration.m_displayGroup = "Extract Payload";
                outSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
                outSlotConfiguration.m_isLatent = false;
                out.name = outSlotConfiguration.m_name;
                SlotId outSlotId = AddSlot(outSlotConfiguration);
                AZ_Assert(outSlotId.IsValid(), "ExecutionOutput slot is not created successfully.");
                out.slotId = outSlotId;
                    
        // Data Slot: Float4Output (Output)
        {

            ScriptCanvas::DataSlotConfiguration dataSlotConfiguration;
            dataSlotConfiguration.m_name = "Float4Output";
            dataSlotConfiguration.m_toolTip = "";
            dataSlotConfiguration.m_displayGroup = "Extract Payload";
            dataSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
            dataSlotConfiguration.SetAZType<AZ::Vector4>();
            SlotId slotId = AddSlot(dataSlotConfiguration);
            AZ_Assert(slotId.IsValid(), "Data slot is not created successfully.");
            out.outputs.push_back(slotId);
        }
        // Data Slot: Float4Output...


                in.outs.push_back(out);
            }
            {
                SlotExecution::Out out;
                ScriptCanvas::ExecutionSlotConfiguration outSlotConfiguration;
                outSlotConfiguration.m_name = "Int";
                outSlotConfiguration.m_toolTip = "";
                outSlotConfiguration.m_displayGroup = "Extract Payload";
                outSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
                outSlotConfiguration.m_isLatent = false;
                out.name = outSlotConfiguration.m_name;
                SlotId outSlotId = AddSlot(outSlotConfiguration);
                AZ_Assert(outSlotId.IsValid(), "ExecutionOutput slot is not created successfully.");
                out.slotId = outSlotId;
                    
        // Data Slot: IntOutput (Output)
        {

            ScriptCanvas::DataSlotConfiguration dataSlotConfiguration;
            dataSlotConfiguration.m_name = "IntOutput";
            dataSlotConfiguration.m_toolTip = "";
            dataSlotConfiguration.m_displayGroup = "Extract Payload";
            dataSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
            dataSlotConfiguration.SetAZType<float>();
            SlotId slotId = AddSlot(dataSlotConfiguration);
            AZ_Assert(slotId.IsValid(), "Data slot is not created successfully.");
            out.outputs.push_back(slotId);
        }
        // Data Slot: IntOutput...


                in.outs.push_back(out);
            }
            {
                SlotExecution::Out out;
                ScriptCanvas::ExecutionSlotConfiguration outSlotConfiguration;
                outSlotConfiguration.m_name = "Int2";
                outSlotConfiguration.m_toolTip = "";
                outSlotConfiguration.m_displayGroup = "Extract Payload";
                outSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
                outSlotConfiguration.m_isLatent = false;
                out.name = outSlotConfiguration.m_name;
                SlotId outSlotId = AddSlot(outSlotConfiguration);
                AZ_Assert(outSlotId.IsValid(), "ExecutionOutput slot is not created successfully.");
                out.slotId = outSlotId;
                    
        // Data Slot: Int2Output (Output)
        {

            ScriptCanvas::DataSlotConfiguration dataSlotConfiguration;
            dataSlotConfiguration.m_name = "Int2Output";
            dataSlotConfiguration.m_toolTip = "";
            dataSlotConfiguration.m_displayGroup = "Extract Payload";
            dataSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
            dataSlotConfiguration.SetAZType<AZ::Vector2>();
            SlotId slotId = AddSlot(dataSlotConfiguration);
            AZ_Assert(slotId.IsValid(), "Data slot is not created successfully.");
            out.outputs.push_back(slotId);
        }
        // Data Slot: Int2Output...


                in.outs.push_back(out);
            }
            {
                SlotExecution::Out out;
                ScriptCanvas::ExecutionSlotConfiguration outSlotConfiguration;
                outSlotConfiguration.m_name = "Int3";
                outSlotConfiguration.m_toolTip = "";
                outSlotConfiguration.m_displayGroup = "Extract Payload";
                outSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
                outSlotConfiguration.m_isLatent = false;
                out.name = outSlotConfiguration.m_name;
                SlotId outSlotId = AddSlot(outSlotConfiguration);
                AZ_Assert(outSlotId.IsValid(), "ExecutionOutput slot is not created successfully.");
                out.slotId = outSlotId;
                    
        // Data Slot: Int3Output (Output)
        {

            ScriptCanvas::DataSlotConfiguration dataSlotConfiguration;
            dataSlotConfiguration.m_name = "Int3Output";
            dataSlotConfiguration.m_toolTip = "";
            dataSlotConfiguration.m_displayGroup = "Extract Payload";
            dataSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
            dataSlotConfiguration.SetAZType<AZ::Vector3>();
            SlotId slotId = AddSlot(dataSlotConfiguration);
            AZ_Assert(slotId.IsValid(), "Data slot is not created successfully.");
            out.outputs.push_back(slotId);
        }
        // Data Slot: Int3Output...


                in.outs.push_back(out);
            }
            {
                SlotExecution::Out out;
                ScriptCanvas::ExecutionSlotConfiguration outSlotConfiguration;
                outSlotConfiguration.m_name = "Int4";
                outSlotConfiguration.m_toolTip = "";
                outSlotConfiguration.m_displayGroup = "Extract Payload";
                outSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
                outSlotConfiguration.m_isLatent = false;
                out.name = outSlotConfiguration.m_name;
                SlotId outSlotId = AddSlot(outSlotConfiguration);
                AZ_Assert(outSlotId.IsValid(), "ExecutionOutput slot is not created successfully.");
                out.slotId = outSlotId;
                    
        // Data Slot: Int4Output (Output)
        {

            ScriptCanvas::DataSlotConfiguration dataSlotConfiguration;
            dataSlotConfiguration.m_name = "Int4Output";
            dataSlotConfiguration.m_toolTip = "";
            dataSlotConfiguration.m_displayGroup = "Extract Payload";
            dataSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
            dataSlotConfiguration.SetAZType<AZ::Vector4>();
            SlotId slotId = AddSlot(dataSlotConfiguration);
            AZ_Assert(slotId.IsValid(), "Data slot is not created successfully.");
            out.outputs.push_back(slotId);
        }
        // Data Slot: Int4Output...


                in.outs.push_back(out);
            }
            {
                SlotExecution::Out out;
                ScriptCanvas::ExecutionSlotConfiguration outSlotConfiguration;
                outSlotConfiguration.m_name = "Bool";
                outSlotConfiguration.m_toolTip = "";
                outSlotConfiguration.m_displayGroup = "Extract Payload";
                outSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
                outSlotConfiguration.m_isLatent = false;
                out.name = outSlotConfiguration.m_name;
                SlotId outSlotId = AddSlot(outSlotConfiguration);
                AZ_Assert(outSlotId.IsValid(), "ExecutionOutput slot is not created successfully.");
                out.slotId = outSlotId;
                    
        // Data Slot: BoolOutput (Output)
        {

            ScriptCanvas::DataSlotConfiguration dataSlotConfiguration;
            dataSlotConfiguration.m_name = "BoolOutput";
            dataSlotConfiguration.m_toolTip = "";
            dataSlotConfiguration.m_displayGroup = "Extract Payload";
            dataSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
            dataSlotConfiguration.SetAZType<bool>();
            SlotId slotId = AddSlot(dataSlotConfiguration);
            AZ_Assert(slotId.IsValid(), "Data slot is not created successfully.");
            out.outputs.push_back(slotId);
        }
        // Data Slot: BoolOutput...


                in.outs.push_back(out);
            }
            {
                SlotExecution::Out out;
                ScriptCanvas::ExecutionSlotConfiguration outSlotConfiguration;
                outSlotConfiguration.m_name = "Orientation";
                outSlotConfiguration.m_toolTip = "";
                outSlotConfiguration.m_displayGroup = "Extract Payload";
                outSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
                outSlotConfiguration.m_isLatent = false;
                out.name = outSlotConfiguration.m_name;
                SlotId outSlotId = AddSlot(outSlotConfiguration);
                AZ_Assert(outSlotId.IsValid(), "ExecutionOutput slot is not created successfully.");
                out.slotId = outSlotId;
                    
        // Data Slot: OrientationOutput (Output)
        {

            ScriptCanvas::DataSlotConfiguration dataSlotConfiguration;
            dataSlotConfiguration.m_name = "OrientationOutput";
            dataSlotConfiguration.m_toolTip = "";
            dataSlotConfiguration.m_displayGroup = "Extract Payload";
            dataSlotConfiguration.SetConnectionType(ScriptCanvas::ConnectionType::Output);
            dataSlotConfiguration.SetAZType<AZ::Quaternion>();
            SlotId slotId = AddSlot(dataSlotConfiguration);
            AZ_Assert(slotId.IsValid(), "Data slot is not created successfully.");
            out.outputs.push_back(slotId);
        }
        // Data Slot: OrientationOutput...


                in.outs.push_back(out);
            }
        } // execution outs end
        ins.push_back(in);
    } // execution in end


    // Generate the execution map
    m_slotExecutionMap = SlotExecution::Map(AZStd::move(ins), AZStd::move(outs));

}


}


