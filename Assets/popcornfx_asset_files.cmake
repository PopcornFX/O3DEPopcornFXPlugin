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
    shaders/Common/BillboardSrg.azsli
    shaders/Common/ComputeBillboardVertex.azsli
    shaders/Common/LightingHelper.azsli
    shaders/Common/MaterialSrg.azsli
    shaders/Common/RendererFeatures.azsli
    shaders/Common/RendererSrg.azsli
    shaders/Common/RibbonSrg.azsli

    shaders/Depth/Basic/DepthPass.azsl
    shaders/Depth/Basic/DepthPass.shader
    shaders/Depth/Basic/DepthPassTransparentMax.shader
    shaders/Depth/Basic/DepthPassTransparentMin.shader

    shaders/Depth/Billboard/DepthPassBillboard.azsl
    shaders/Depth/Billboard/DepthPassBillboard.shader
    shaders/Depth/Billboard/DepthPassTransparentMaxBillboard.shader
    shaders/Depth/Billboard/DepthPassTransparentMinBillboard.shader

    # Billboards:
    shaders/Billboard/Default/Billboard.azsl
    shaders/Billboard/Default/Billboard.shader
    shaders/Billboard/Default/Billboard.shadervariantlist

    shaders/Billboard/Legacy/Billboard_Legacy.azsl
    shaders/Billboard/Legacy/Billboard_Legacy.shader
    shaders/Billboard/Legacy/Billboard_Legacy.shadervariantlist

    shaders/Billboard/Legacy/BillboardLit_Legacy.azsl
    shaders/Billboard/Legacy/BillboardLit_Legacy.shader
    shaders/Billboard/Legacy/BillboardLit_Legacy.shadervariantlist

    # Ribbons:
    shaders/Ribbon/Legacy/Ribbon_Legacy.azsl
    shaders/Ribbon/Legacy/Ribbon_Legacy.shader
    shaders/Ribbon/Legacy/RibbonAtlasBlend_Legacy.shader
    shaders/Ribbon/Legacy/RibbonCorrectDeformation_Legacy.shader

    shaders/Ribbon/Legacy/RibbonLit_Legacy.azsl
    shaders/Ribbon/Legacy/RibbonLit_Legacy.shader
    shaders/Ribbon/Legacy/RibbonLitAtlasBlend_Legacy.shader
    shaders/Ribbon/Legacy/RibbonLitCorrectDeformation_Legacy.shader

    # Distortion:
    shaders/PostProcess/DistortionPostProcess.azsl
    shaders/PostProcess/DistortionPostProcess.shader
    passes/DistortionParent.pass
    passes/DistortionAccumulation.pass
    passes/DistortionPostProcess.pass
) 
