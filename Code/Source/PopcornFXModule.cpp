//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
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
#endif

#if defined(POPCORNFX_EDITOR) && !defined(POPCORNFX_BUILDER)
#include "PopcornFXPreviewersSystemComponent.h"
#endif

#if defined(POPCORNFX_BUILDER)
#include "Asset/PopcornFXAssetBuilderComponent.h"
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

namespace PopcornFX {

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
#if defined(POPCORNFX_EDITOR) && !defined(POPCORNFX_BUILDER)
				PopcornFXPreviewersSystemComponent::CreateDescriptor(),
#endif // POPCORNFX_EDITOR && !POPCORNFX_BUILDER
				//-------- TrackView Folder
				PopcornFXTrackViewAttributeGameComponent::CreateDescriptor(),
#if defined(POPCORNFX_EDITOR)
				PopcornFXTrackViewAttributeEditorComponent::CreateDescriptor(),
#endif // POPCORNFX_EDITOR
				//-------- Emitter Folder
#if defined(POPCORNFX_EDITOR)
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
#if defined(POPCORNFX_BUILDER)
				PopcornFXBuilderComponent::CreateDescriptor(),
#endif // POPCORNFX_BUILDER
			});
		}

		/**
		 * Add required SystemComponents to the SystemEntity.
		 */
		AZ::ComponentTypeList GetRequiredSystemComponents() const override
		{
			return AZ::ComponentTypeList{
				azrtti_typeid<PopcornFXSystemComponent>(),
#if defined(POPCORNFX_EDITOR) && !defined(POPCORNFX_BUILDER)
				azrtti_typeid<PopcornFXPreviewersSystemComponent>(),
#endif // POPCORNFX_EDITOR && !POPCORNFX_BUILDER
			};
		}

	};
}

AZ_DECLARE_MODULE_CLASS(Gem_PopcornFX, PopcornFX::PopcornFXModule)
