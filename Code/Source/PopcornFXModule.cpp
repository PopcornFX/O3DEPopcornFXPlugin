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

#include <AzCore/Module/Module.h>

#if defined(POPCORNFX_EDITOR)
#include "Components/Attributes/TrackView/PopcornFXTrackViewAttributeEditorComponent.h"
#include "Components/Emitter/PopcornFXEmitterEditorComponent.h"
#include "Components/Samplers/PopcornFXSamplerAnimTrackEditorComponent.h"
#include "Components/Samplers/PopcornFXSamplerImageEditorComponent.h"
#include "Components/Samplers/PopcornFXSamplerShapeEditorComponent.h"
#include "Components/Samplers/PopcornFXSamplerTextEditorComponent.h"
#include "Components/Helpers/PopcornFXHelperLoopEmitterEditorComponent.h"
#include "Components/Helpers/PopcornFXHelperProfilerEditorComponent.h"
#include "Asset/PopcornFXAssetBuilderComponent.h"
#include "PopcornFXPreviewersSystemComponent.h"
#endif
#include "Components/Attributes/TrackView/PopcornFXTrackViewAttributeGameComponent.h"
#include "Components/Emitter/PopcornFXEmitterGameComponent.h"
#include "Components/Samplers/PopcornFXSamplerAnimTrackGameComponent.h"
#include "Components/Samplers/PopcornFXSamplerImageGameComponent.h"
#include "Components/Samplers/PopcornFXSamplerShapeGameComponent.h"
#include "Components/Samplers/PopcornFXSamplerTextGameComponent.h"
#include "Components/Helpers/PopcornFXHelperLoopEmitterGameComponent.h"
#include "Components/Helpers/PopcornFXHelperProfilerGameComponent.h"

#include "PopcornFXSystemComponent.h"
#include "ScriptCanvas/PopcornFXLibrary.h"

__LMBRPK_BEGIN

	class PopcornFXModule
		: public AZ::Module
	{
	public:
		AZ_RTTI(PopcornFXModule, "{AACE331E-39B9-403B-9F99-722DC7E7692C}", AZ::Module);
		AZ_CLASS_ALLOCATOR(PopcornFXModule, AZ::SystemAllocator, 0);

		PopcornFXModule()
			: AZ::Module()
		{
			// Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
			m_descriptors.insert(m_descriptors.end(), {
				PopcornFXSystemComponent::CreateDescriptor(),
#if defined(POPCORNFX_EDITOR)
				PopcornFXPreviewersSystemComponent::CreateDescriptor(),
#endif // POPCORNFX_EDITOR
				//-------- TrackView Folder
				PopcornFXTrackViewAttributeGameComponent::CreateDescriptor(),
#if defined(POPCORNFX_EDITOR)
				PopcornFXTrackViewAttributeEditorComponent::CreateDescriptor(),
#endif // POPCORNFX_EDITOR
				//-------- Emitter Folder
#if defined(POPCORNFX_EDITOR)
				PopcornFXBuilderComponent::CreateDescriptor(),
				PopcornFXEmitterEditorComponent::CreateDescriptor(),
#endif // POPCORNFX_EDITOR
				PopcornFXEmitterGameComponent::CreateDescriptor(),
				//--------
				//-------- Samplers Folder
#if defined(POPCORNFX_EDITOR)
				PopcornFXSamplerAnimTrackEditorComponent::CreateDescriptor(),
				PopcornFXSamplerImageEditorComponent::CreateDescriptor(),
				PopcornFXSamplerShapeEditorComponent::CreateDescriptor(),
				PopcornFXSamplerTextEditorComponent::CreateDescriptor(),
#endif // POPCORNFX_EDITOR
				PopcornFXSamplerAnimTrackGameComponent::CreateDescriptor(),
				PopcornFXSamplerImageGameComponent::CreateDescriptor(),
				PopcornFXSamplerShapeGameComponent::CreateDescriptor(),
				PopcornFXSamplerTextGameComponent::CreateDescriptor(),
				//--------
				//-------- Helpers Folder
#if defined(POPCORNFX_EDITOR)
				PopcornFXHelperLoopEmitterEditorComponent::CreateDescriptor(),
				PopcornFXHelperProfilerEditorComponent::CreateDescriptor(),
#endif // POPCORNFX_EDITOR
				PopcornFXHelperLoopEmitterGameComponent::CreateDescriptor(),
				PopcornFXHelperProfilerGameComponent::CreateDescriptor(),
				//--------
			});

			//-------- ScriptCanvas Folder
			AZStd::vector<AZ::ComponentDescriptor*> componentDescriptors(PopcornFXLibrary::GetComponentDescriptors());
			m_descriptors.insert(m_descriptors.end(), componentDescriptors.begin(), componentDescriptors.end());
			//--------
		}

		/**
		 * Add required SystemComponents to the SystemEntity.
		 */
		AZ::ComponentTypeList GetRequiredSystemComponents() const override
		{
			return AZ::ComponentTypeList{
				azrtti_typeid<PopcornFXSystemComponent>(),
#if defined(POPCORNFX_EDITOR)
				azrtti_typeid<PopcornFXPreviewersSystemComponent>(),
#endif // defined(POPCORNFX_EDITOR)
			};
		}

	};
__LMBRPK_END

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(PopcornFX_185d46ff567b4905a74e972ccf213803, LmbrPk::PopcornFXModule)
