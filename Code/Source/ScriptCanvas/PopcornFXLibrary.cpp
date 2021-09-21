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
#include "PopcornFXLibrary.h"

#include <AzCore/Serialization/EditContext.h>

// script canvas
#include <ScriptCanvas/Libraries/Libraries.h>

#include "PopcornFXBroadcastNodeable.h"
#include "PopcornFXExtractPayloadNodeable.h"

__LMBRPK_BEGIN

    void PopcornFXLibrary::Reflect(AZ::ReflectContext* reflection)
    {
        AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection);
        if (serializeContext)
        {
            serializeContext->Class<PopcornFXLibrary, LibraryDefinition>()
                ->Version(1)
                ;

            AZ::EditContext* editContext = serializeContext->GetEditContext();
            if (editContext)
            {
                editContext->Class<PopcornFXLibrary>("Event", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Icon, "Editor/Icons/Components/InputConfig.png")
                    ;
            }
        }
    }

    void PopcornFXLibrary::InitNodeRegistry(ScriptCanvas::NodeRegistry& nodeRegistry)
    {
        (void)nodeRegistry;
        ScriptCanvas::Library::AddNodeToRegistry<PopcornFXLibrary, Nodes::PopcornFXBroadcastNodeableNode>(nodeRegistry);
        ScriptCanvas::Library::AddNodeToRegistry<PopcornFXLibrary, Nodes::PopcornFXExtractPayloadNodeableNode>(nodeRegistry);
    }

    AZStd::vector<AZ::ComponentDescriptor*> PopcornFXLibrary::GetComponentDescriptors()
    {
        return AZStd::vector<AZ::ComponentDescriptor*>({
            Nodes::PopcornFXBroadcastNodeableNode::CreateDescriptor(),
            Nodes::PopcornFXExtractPayloadNodeableNode::CreateDescriptor(),
        });
    }

__LMBRPK_END
