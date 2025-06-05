#----------------------------------------------------------------------------
# Copyright Persistant Studios, SARL. All Rights Reserved.
# https://www.popcornfx.com/terms-and-conditions/
#----------------------------------------------------------------------------

set(FILES
    shaders/Common/BillboardSrg.azsli
    shaders/Common/ComputeBillboardVertex.azsli
    shaders/Common/LightingHelper.azsli
    shaders/Common/MaterialSrg.azsli
    shaders/Common/MeshSrg.azsli
    shaders/Common/PopcornOptions.azsli
    shaders/Common/RendererFeatures.azsli
    shaders/Common/RendererSrg.azsli
    shaders/Common/RibbonSrg.azsli

    shaders/Depth/Basic/DepthPass.azsl
    shaders/Depth/Basic/DepthPass.shader
    shaders/Depth/Basic/DepthPassTransparentMax.shader
    shaders/Depth/Basic/DepthPassTransparentMin.shader

    shaders/Depth/Billboard/DepthPass.azsl
    shaders/Depth/Billboard/DepthPassBillboard.azsl
    shaders/Depth/Billboard/DepthPassBillboard.shader
    shaders/Depth/Billboard/DepthPassBillboard.shadervariantlist
    shaders/Depth/Billboard/DepthPassTransparentMaxBillboard.shader
    shaders/Depth/Billboard/DepthPassTransparentMaxBillboard.shadervariantlist
    shaders/Depth/Billboard/DepthPassTransparentMinBillboard.shader
    shaders/Depth/Billboard/DepthPassTransparentMinBillboard.shadervariantlist

    shaders/Depth/Mesh/DepthPassMesh.azsl
    shaders/Depth/Mesh/DepthPassMesh.shader
    shaders/Depth/Mesh/DepthPassTransparentMaxMesh.shader
    shaders/Depth/Mesh/DepthPassTransparentMinMesh.shader

    # Billboards:
    shaders/Billboard/Default/Billboard.azsl
    shaders/Billboard/Default/Billboard.shader
    shaders/Billboard/Default/Billboard.shadervariantlist

    shaders/Billboard/Default/BillboardDistortion.azsl
    shaders/Billboard/Default/BillboardDistortion.shader
    shaders/Billboard/Default/BillboardDistortion.shadervariantlist

    shaders/Billboard/Default/BillboardLit.azsl
    shaders/Billboard/Default/BillboardLit.shader
    shaders/Billboard/Default/BillboardLit.shadervariantlist

    shaders/Billboard/Legacy/Billboard_Legacy.azsl
    shaders/Billboard/Legacy/Billboard_Legacy.shader
    shaders/Billboard/Legacy/Billboard_Legacy.shadervariantlist

    shaders/Billboard/Legacy/BillboardLit_Legacy.azsl
    shaders/Billboard/Legacy/BillboardLit_Legacy.shader
    shaders/Billboard/Legacy/BillboardLit_Legacy.shadervariantlist

    # Ribbons:
    shaders/Ribbon/Default/Ribbon.azsl
    shaders/Ribbon/Default/Ribbon.shader
    shaders/Ribbon/Default/Ribbon.shadervariantlist
    shaders/Ribbon/Default/RibbonAtlasBlend.shader
    shaders/Ribbon/Default/RibbonAtlasBlend.shadervariantlist
    shaders/Ribbon/Default/RibbonCorrectDeformation.shader
    shaders/Ribbon/Default/RibbonCorrectDeformation.shadervariantlist

    shaders/Ribbon/Default/RibbonDistortion.azsl
    shaders/Ribbon/Default/RibbonDistortion.shader

    shaders/Ribbon/Default/RibbonLit.azsl
    shaders/Ribbon/Default/RibbonLit.shader
    shaders/Ribbon/Default/RibbonLit.shadervariantlist
    shaders/Ribbon/Default/RibbonLitAtlasBlend.shader
    shaders/Ribbon/Default/RibbonLitAtlasBlend.shadervariantlist
    shaders/Ribbon/Default/RibbonLitCorrectDeformation.shader
    shaders/Ribbon/Default/RibbonLitCorrectDeformation.shadervariantlist

    shaders/Ribbon/Legacy/Ribbon_Legacy.azsl
    shaders/Ribbon/Legacy/Ribbon_Legacy.shader
    shaders/Ribbon/Legacy/RibbonAtlasBlend_Legacy.shader
    shaders/Ribbon/Legacy/RibbonCorrectDeformation_Legacy.shader

    shaders/Ribbon/Legacy/RibbonLit_Legacy.azsl
    shaders/Ribbon/Legacy/RibbonLit_Legacy.shader
    shaders/Ribbon/Legacy/RibbonLitAtlasBlend_Legacy.shader
    shaders/Ribbon/Legacy/RibbonLitCorrectDeformation_Legacy.shader

    # Meshes:
    shaders/Mesh/Default/Mesh.azsl
    shaders/Mesh/Default/Mesh.shader
    shaders/Mesh/Default/Mesh.shadervariantlist

    shaders/Mesh/Default/MeshLit.azsl
    shaders/Mesh/Default/MeshLit.shader
    shaders/Mesh/Default/MeshLit.shadervariantlist

    shaders/Mesh/Legacy/Mesh_Legacy.azsl
    shaders/Mesh/Legacy/Mesh_Legacy.shader
    shaders/Mesh/Legacy/Mesh_Legacy.shadervariantlist

    shaders/Mesh/Legacy/MeshLit_Legacy.azsl
    shaders/Mesh/Legacy/MeshLit_Legacy.shader
    shaders/Mesh/Legacy/MeshLit_Legacy.shadervariantlist

    # Distortion:
    shaders/PostProcess/DistortionPostProcess.azsl
    shaders/PostProcess/DistortionPostProcess.shader
    Passes/DistortionParent.pass
    Passes/DistortionAccumulation.pass
    Passes/DistortionPostProcess.pass
) 
