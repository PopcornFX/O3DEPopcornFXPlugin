#
# All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
# its licensors.
#
# For complete copyright and license terms please see the LICENSE at the root of this
# distribution (the "License"). All use of this software is governed by the License,
# or, if provided, by the license below or the license accompanying this file. Do not
# remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#

set(FILES
    Source/Asset/PopcornFXAssetBuilderComponent.cpp
    Source/Asset/PopcornFXAssetBuilderComponent.h
    Source/Asset/PopcornFXAssetBuilderWorker.cpp
    Source/Asset/PopcornFXAssetBuilderWorker.h
    Source/Integration/Editor/PackLoader.cpp
    Source/Integration/Editor/PackLoader.h
    Source/Integration/Editor/BakerManager.cpp
    Source/Integration/Editor/BakerManager.h
    Source/Components/Emitter/PopcornFXEmitterEditorComponent.cpp
    Source/Components/Emitter/PopcornFXEmitterEditorComponent.h
    Source/Components/Attributes/PopcornFXEditorAttributeList.cpp
    Source/Components/Attributes/PopcornFXEditorAttributeList.h
    Source/Components/Attributes/TrackView/PopcornFXTrackViewAttributeEditorComponent.cpp
    Source/Components/Attributes/TrackView/PopcornFXTrackViewAttributeEditorComponent.h
    Source/Components/Samplers/PopcornFXSamplerAnimTrackEditorComponent.cpp
    Source/Components/Samplers/PopcornFXSamplerAnimTrackEditorComponent.h
    Source/Components/Samplers/PopcornFXSamplerImageEditorComponent.cpp
    Source/Components/Samplers/PopcornFXSamplerImageEditorComponent.h
    Source/Components/Samplers/PopcornFXSamplerShapeEditorComponent.cpp
    Source/Components/Samplers/PopcornFXSamplerShapeEditorComponent.h
    Source/Components/Samplers/PopcornFXSamplerTextEditorComponent.cpp
    Source/Components/Samplers/PopcornFXSamplerTextEditorComponent.h
    Source/Components/Helpers/PopcornFXHelperLoopEmitterEditorComponent.cpp
    Source/Components/Helpers/PopcornFXHelperLoopEmitterEditorComponent.h
    Source/Components/Helpers/PopcornFXHelperProfilerEditorComponent.cpp
    Source/Components/Helpers/PopcornFXHelperProfilerEditorComponent.h
	# Content browser asset preview
	Source/Previewer/PopcornFXEffectPreviewer.cpp
	Source/Previewer/PopcornFXEffectPreviewer.h
	Source/Previewer/PopcornFXEffectPreviewer.ui
	Source/Previewer/PopcornFXEffectPreviewerFactory.cpp
	Source/Previewer/PopcornFXEffectPreviewerFactory.h
	Source/PopcornFXPreviewersSystemComponent.cpp
	Source/PopcornFXPreviewersSystemComponent.h
    # Common:
    Source/PopcornFXModule.cpp
    Source/Integration/File/JsonPackPath.cpp
    Source/Integration/File/JsonPackPath.h
    Source/Integration/Managers/SceneViewsManager.cpp
    Source/Integration/Managers/SceneViewsManager.h
    Source/Integration/PopcornFXIntegration.h
    Source/Integration/PopcornFXIntegration.cpp
    Source/Integration/PopcornFXIntegrationBus.h
    Source/Integration/SceneInterface/SceneInterface.cpp
    Source/Integration/SceneInterface/SceneInterface.h
)
