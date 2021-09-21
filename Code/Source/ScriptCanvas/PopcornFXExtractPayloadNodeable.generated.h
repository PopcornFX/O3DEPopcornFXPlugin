/*
 * Copyright (c) Contributors to the Open 3D Engine Project. For complete copyright and license terms please see the LICENSE at the root of this distribution..
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 * This file is generated automatically at compile time, DO NOT EDIT BY HAND
 * Template Source C:/o3de/Gems/ScriptCanvas/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNodeable_Header.jinja; Data Sources C:\o3de\Gems\PopcornFX\Code\Source\ScriptCanvas\PopcornFXExtractPayloadNodeable.ScriptCanvasNodeable.xml
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This code was produced with AzAutoGen, any modifications made will not be preserved.
// If you need to modify this code see:
//
// Gems\ScriptCanvas\Code\Include\ScriptCanvas\AutoGen\ScriptCanvasNodeable_Header.jinja
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <AzCore/RTTI/TypeInfo.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

#include <ScriptCanvas/Core/Nodeable.h>
#include <ScriptCanvas/Core/NodeableNode.h>





namespace LmbrPk
{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// PopcornFXExtractPayloadNodeable
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


// The following will be injected directly into the source header file for which AZ AutoGen is being run.
// You must #include the generated header into the source header
#define SCRIPTCANVAS_NODE_PopcornFXExtractPayloadNodeable \
public: \
    AZ_RTTI(PopcornFXExtractPayloadNodeable, "{F58CB993-03D8-B268-21CB-49D75C993E42}", ScriptCanvas::Nodeable); \
    static void Reflect(AZ::ReflectContext* reflection); \
    static void ExtendReflectionSerialize([[maybe_unused]] AZ::SerializeContext::ClassBuilder* builder){} \
    static void ExtendReflectionEdit([[maybe_unused]] AZ::EditContext::ClassBuilder* builder){} \
    static void ExtendReflectionBehavior([[maybe_unused]] AZ::BehaviorContext::ClassBuilder<PopcornFXExtractPayloadNodeable>* builder){} \
    static const char* GetDescription() { return "PopcornFX Extract Payload"; } \
    ScriptCanvas::NodePropertyInterface* GetPropertyInterface(AZ::Crc32 propertyId) override; \
    bool IsActive() const override { return false; } \
    void ExtractPayload([[maybe_unused]] AZStd::string PayloadName, [[maybe_unused]] AZStd::string PayloadType); \
                    void CallFloat([[maybe_unused]] float FloatOutput); \
                    void CallFloat2([[maybe_unused]] AZ::Vector2 Float2Output); \
                    void CallFloat3([[maybe_unused]] AZ::Vector3 Float3Output); \
                    void CallFloat4([[maybe_unused]] AZ::Vector4 Float4Output); \
                    void CallInt([[maybe_unused]] float IntOutput); \
                    void CallInt2([[maybe_unused]] AZ::Vector2 Int2Output); \
                    void CallInt3([[maybe_unused]] AZ::Vector3 Int3Output); \
                    void CallInt4([[maybe_unused]] AZ::Vector4 Int4Output); \
                    void CallBool([[maybe_unused]] bool BoolOutput); \
                    void CallOrientation([[maybe_unused]] AZ::Quaternion OrientationOutput); \
size_t GetRequiredOutCount() const override; 
/* end #define SCRIPTCANVAS_NODE_PopcornFXExtractPayloadNodeable */


      namespace Nodes
      {
          class PopcornFXExtractPayloadNodeableNode : public ScriptCanvas::Nodes::NodeableNode
          {
          public:

              AZ_COMPONENT(PopcornFXExtractPayloadNodeableNode, "{A2D70ACF-1493-E82C-150C-73AF12514CA4}", ScriptCanvas::Nodes::NodeableNode);

              static void Reflect(AZ::ReflectContext* context);

              static void ExtendReflectionSerialize([[maybe_unused]] AZ::SerializeContext::ClassBuilder* builder){}
              static void ExtendReflectionEdit([[maybe_unused]] AZ::EditContext::ClassBuilder* builder){}
              static void ExtendReflectionBehavior([[maybe_unused]] AZ::BehaviorContext::ClassBuilder<PopcornFXExtractPayloadNodeableNode>* builder){}
              void ConfigureSlots() override;

              /* no slot configuration extension, Use Class attribute 'ExtendConfigureSlots' to extend them */

              void ConfigureVisualExtensions() override;

              size_t GenerateFingerprint() const override;

              PopcornFXExtractPayloadNodeableNode();

              
          };
      }

}




