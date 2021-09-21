/*
 * Copyright (c) Contributors to the Open 3D Engine Project. For complete copyright and license terms please see the LICENSE at the root of this distribution..
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 * This file is generated automatically at compile time, DO NOT EDIT BY HAND
 * Template Source C:/o3de/Gems/ScriptCanvas/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNodeable_Header.jinja; Data Sources C:\o3de\Gems\PopcornFX\Code\Source\ScriptCanvas\PopcornFXBroadcastNodeable.ScriptCanvasNodeable.xml
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
//// PopcornFXBroadcastNodeable
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


// The following will be injected directly into the source header file for which AZ AutoGen is being run.
// You must #include the generated header into the source header
#define SCRIPTCANVAS_NODE_PopcornFXBroadcastNodeable \
public: \
    AZ_RTTI(PopcornFXBroadcastNodeable, "{FF8B82F3-D9B3-DE56-2A73-BE70E8321501}", ScriptCanvas::Nodeable); \
    static void Reflect(AZ::ReflectContext* reflection); \
    static void ExtendReflectionSerialize([[maybe_unused]] AZ::SerializeContext::ClassBuilder* builder){} \
    static void ExtendReflectionEdit([[maybe_unused]] AZ::EditContext::ClassBuilder* builder){} \
    static void ExtendReflectionBehavior([[maybe_unused]] AZ::BehaviorContext::ClassBuilder<PopcornFXBroadcastNodeable>* builder){} \
    static const char* GetDescription() { return "PopcornFX Broadcast"; } \
    ScriptCanvas::NodePropertyInterface* GetPropertyInterface(AZ::Crc32 propertyId) override; \
    bool IsActive() const override { return false; } \
void ConnectEvent([[maybe_unused]] AZStd::string EventName, [[maybe_unused]] AZ::EntityId EntityId); \
                    void CallBroadcast(); \
size_t GetRequiredOutCount() const override; 
/* end #define SCRIPTCANVAS_NODE_PopcornFXBroadcastNodeable */


      namespace Nodes
      {
          class PopcornFXBroadcastNodeableNode : public ScriptCanvas::Nodes::NodeableNode
          {
          public:

              AZ_COMPONENT(PopcornFXBroadcastNodeableNode, "{4827D3BB-D3E2-B99E-B237-525FFA7FFA20}", ScriptCanvas::Nodes::NodeableNode);

              static void Reflect(AZ::ReflectContext* context);

              static void ExtendReflectionSerialize([[maybe_unused]] AZ::SerializeContext::ClassBuilder* builder){}
              static void ExtendReflectionEdit([[maybe_unused]] AZ::EditContext::ClassBuilder* builder){}
              static void ExtendReflectionBehavior([[maybe_unused]] AZ::BehaviorContext::ClassBuilder<PopcornFXBroadcastNodeableNode>* builder){}
              void ConfigureSlots() override;

              /* no slot configuration extension, Use Class attribute 'ExtendConfigureSlots' to extend them */

              void ConfigureVisualExtensions() override;

              size_t GenerateFingerprint() const override;

              PopcornFXBroadcastNodeableNode();

              
          };
      }

}




