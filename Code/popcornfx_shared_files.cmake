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
    Include/PopcornFX/PopcornFXBus.h

    Source/Integration/Audio/PopcornFXAudioPool.h
    Source/Integration/Audio/PopcornFXAudioPool.cpp

    Source/Integration/Render/AtomIntegration/LmbrAtomBillboardingBatchPolicy.h
    Source/Integration/Render/AtomIntegration/LmbrAtomBillboardingBatchPolicy.cpp
    Source/Integration/Render/AtomIntegration/LmbrAtomFrameCollector.h
    Source/Integration/Render/AtomIntegration/LmbrAtomFrameCollector.cpp
    Source/Integration/Render/AtomIntegration/LmbrAtomFrameCollectorTypes.h
    Source/Integration/Render/AtomIntegration/LmbrAtomRenderDataFactory.h
    Source/Integration/Render/AtomIntegration/LmbrAtomRenderDataFactory.cpp
    Source/Integration/Render/AtomIntegration/LmbrAtomPipelineCache.h
    Source/Integration/Render/AtomIntegration/LmbrAtomPipelineCache.cpp
    Source/Integration/Render/AtomIntegration/PopcornFXFeatureProcessor.h
    Source/Integration/Render/AtomIntegration/PopcornFXFeatureProcessor.cpp

    Source/Integration/Render/BasicEditorRendererFeatures.cpp
    Source/Integration/Render/BasicEditorRendererFeatures.h
    Source/Integration/Render/LmbrRendererCache.cpp
    Source/Integration/Render/LmbrRendererCache.h
    Source/Integration/Render/CameraInfo.h

    Source/Integration/Managers/EmittersManager.cpp
    Source/Integration/Managers/EmittersManager.h
    Source/Integration/Managers/RenderManager.cpp
    Source/Integration/Managers/RenderManager.h
    Source/Integration/Managers/BroadcastManager.cpp
    Source/Integration/Managers/BroadcastManager.h
    Source/Integration/Managers/MediumCollectionManager.cpp
    Source/Integration/Managers/MediumCollectionManager.h
    Source/Integration/Managers/StatsManager.cpp
    Source/Integration/Managers/StatsManager.h
    Source/Integration/Managers/WindManager.cpp
    Source/Integration/Managers/WindManager.h
    Source/Components/Emitter/PopcornFXEmitterGameComponent.cpp
    Source/Components/Emitter/PopcornFXEmitterGameComponent.h
    Source/Components/Emitter/PopcornFXEmitterRuntime.cpp
    Source/Components/Emitter/PopcornFXEmitterRuntime.h
    Source/Components/Emitter/PopcornFXEmitter.cpp
    Source/Components/Emitter/PopcornFXEmitter.h
    Source/Components/Attributes/PopcornFXAttributeList.cpp
    Source/Components/Attributes/PopcornFXAttributeList.h
    Source/Components/Attributes/TrackView/PopcornFXTrackViewAttributeGameComponent.cpp
    Source/Components/Attributes/TrackView/PopcornFXTrackViewAttributeGameComponent.h
    Source/Components/Attributes/TrackView/PopcornFXTrackViewAttribute.cpp
    Source/Components/Attributes/TrackView/PopcornFXTrackViewAttribute.h
    Source/Components/Samplers/PopcornFXSamplerAnimTrack.cpp
    Source/Components/Samplers/PopcornFXSamplerAnimTrack.h
    Source/Components/Samplers/PopcornFXSamplerAnimTrackGameComponent.cpp
    Source/Components/Samplers/PopcornFXSamplerAnimTrackGameComponent.h
    Source/Components/Samplers/PopcornFXSamplerImage.cpp
    Source/Components/Samplers/PopcornFXSamplerImage.h
    Source/Components/Samplers/PopcornFXSamplerImageGameComponent.cpp
    Source/Components/Samplers/PopcornFXSamplerImageGameComponent.h
    Source/Components/Samplers/PopcornFXSkinnedMesh.cpp
    Source/Components/Samplers/PopcornFXSkinnedMesh.h
    Source/Components/Samplers/PopcornFXSamplerShape.cpp
    Source/Components/Samplers/PopcornFXSamplerShape.h
    Source/Components/Samplers/PopcornFXSamplerShapeGameComponent.cpp
    Source/Components/Samplers/PopcornFXSamplerShapeGameComponent.h
    Source/Components/Samplers/PopcornFXSamplerText.cpp
    Source/Components/Samplers/PopcornFXSamplerText.h
    Source/Components/Samplers/PopcornFXSamplerTextGameComponent.cpp
    Source/Components/Samplers/PopcornFXSamplerTextGameComponent.h
    Source/Components/Helpers/PopcornFXHelperLoopEmitterGameComponent.cpp
    Source/Components/Helpers/PopcornFXHelperLoopEmitterGameComponent.h
    Source/Components/Helpers/PopcornFXHelperProfilerGameComponent.cpp
    Source/Components/Helpers/PopcornFXHelperProfilerGameComponent.h
    Source/Integration/Preloader/PopcornFXPreloader.cpp
    Source/Integration/Preloader/PopcornFXPreloader.h
    Source/Integration/Preloader/PopcornFXRendererLoader.cpp
    Source/Integration/Preloader/PopcornFXRendererLoader.h
    Source/Integration/ResourceHandlers/ImageResourceHandler.cpp
    Source/Integration/ResourceHandlers/ImageResourceHandler.h
    Source/Integration/ResourceHandlers/MeshResourceHandler.cpp
    Source/Integration/ResourceHandlers/MeshResourceHandler.h
    Source/Asset/PopcornFXAssetHandler.cpp
    Source/Asset/PopcornFXAssetHandler.h
    Source/Asset/PopcornFXAsset.h
    Source/ScriptCanvas/PopcornFXBroadcastNodeable.cpp
    Source/ScriptCanvas/PopcornFXBroadcastNodeable.h
    Source/ScriptCanvas/PopcornFXBroadcastNodeable.ScriptCanvasNodeable.xml
    Source/ScriptCanvas/PopcornFXExtractPayloadNodeable.cpp
    Source/ScriptCanvas/PopcornFXExtractPayloadNodeable.h
    Source/ScriptCanvas/PopcornFXExtractPayloadNodeable.ScriptCanvasNodeable.xml
    Source/ScriptCanvas/PopcornFXLibrary.cpp
    Source/ScriptCanvas/PopcornFXLibrary.h
    Source/PopcornFXSystemComponent.cpp
    Source/PopcornFXSystemComponent.h
    Source/Integration/PopcornFXStartUpdate.cpp
    Source/Integration/PopcornFXStartUpdate.h
    Source/Integration/PopcornFXStopUpdate.cpp
    Source/Integration/PopcornFXStopUpdate.h
    Source/Integration/PopcornFXUtils.h
    Source/Integration/Startup/PopcornFxStartup.cpp
    Source/Integration/Startup/PopcornFxStartup.h
    Source/Integration/Startup/PopcornFxPlugins.cpp
    Source/Integration/Startup/PopcornFxPlugins.h
    Source/Integration/Startup/AdditionalRendererProperties.cpp
    Source/Integration/Startup/AdditionalRendererProperties.h
    Source/Integration/File/FileSystemController_LMBR.cpp
    Source/Integration/File/FileSystemController_LMBR.h
)
