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

#include "PopcornFXExtractPayloadNodeable.h"

#include <AzCore/Component/Entity.h>
#include <ScriptCanvas/Libraries/Libraries.h>

#include "Integration/PopcornFXIntegrationBus.h"

__LMBRPK_BEGIN

    PopcornFXExtractPayloadNodeable::~PopcornFXExtractPayloadNodeable()
    {
    }
    
    void PopcornFXExtractPayloadNodeable::ExtractPayload(AZStd::string payloadName, AZStd::string payloadType)
    {
        const LmbrPk::SPayloadValue* payloadValue;
        LmbrPk::PopcornFXIntegrationBus::BroadcastResult(payloadValue, &LmbrPk::PopcornFXIntegrationBus::Handler::GetCurrentPayloadValue, payloadName);

        if (payloadValue == null)
        {
            AZ_Error("PopcornFX", false, "Cannot find payload \"%s\" of type %s", payloadName.c_str(), payloadType.c_str());
            return;
        }

        if (payloadType == "Float")
        {
            CallFloat(payloadValue->m_ValueFloat[0]);
        }
        else if (payloadType == "Float2")
        {
            CallFloat2(AZ::Vector2(payloadValue->m_ValueFloat[0], payloadValue->m_ValueFloat[1]));
        }
        else if (payloadType == "Float3")
        {
            CallFloat3(AZ::Vector3(payloadValue->m_ValueFloat[0], payloadValue->m_ValueFloat[1], payloadValue->m_ValueFloat[2]));
        }
        else if (payloadType == "Float4")
        {
            CallFloat4(AZ::Vector4(payloadValue->m_ValueFloat[0], payloadValue->m_ValueFloat[1], payloadValue->m_ValueFloat[2], payloadValue->m_ValueFloat[3]));
        }
		if (payloadType == "Int")
		{
			CallInt(static_cast<float>(payloadValue->m_ValueInt[0]));
		}
		else if (payloadType == "Int2")
		{
			CallInt2(AZ::Vector2(static_cast<float>(payloadValue->m_ValueInt[0]), static_cast<float>(payloadValue->m_ValueInt[1])));
		}
		else if (payloadType == "Int3")
		{
			CallInt3(AZ::Vector3(static_cast<float>(payloadValue->m_ValueInt[0]), static_cast<float>(payloadValue->m_ValueInt[1]), static_cast<float>(payloadValue->m_ValueInt[2])));
		}
		else if (payloadType == "Int4")
		{
			CallInt4(AZ::Vector4(static_cast<float>(payloadValue->m_ValueInt[0]), static_cast<float>(payloadValue->m_ValueInt[1]), static_cast<float>(payloadValue->m_ValueInt[2]), static_cast<float>(payloadValue->m_ValueInt[3])));
		}
		else if (payloadType == "Bool")
		{
			CallBool(payloadValue->m_ValueBool[0]);
		}
		else if (payloadType == "Orientation")
		{
			CallOrientation(AZ::Quaternion(payloadValue->m_ValueFloat[0], payloadValue->m_ValueFloat[1], payloadValue->m_ValueFloat[2], payloadValue->m_ValueFloat[3]));
		}
    }

    void PopcornFXExtractPayloadNodeable::OnDeactivate()
    {
    }

__LMBRPK_END
