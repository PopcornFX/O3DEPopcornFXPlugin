{
    "Source" : "Ribbon.azsl",

    "DepthStencilState" :
    {
        "Depth" :
        {
            "Enable" : true,
            "CompareFunc" : "GreaterEqual"
        }
    },

    "RasterState" :
    {
        "CullMode" : "None"
    },

    "GlobalTargetBlendState" :
    {
        "Enable" : true,
        "BlendSource" : "One",
        "BlendDest" : "AlphaSourceInverse",
        "BlendAlphaOp" : "Add"
    },

    "ProgramSettings":
    {
      "EntryPoints":
      [
        {
          "name": "RibbonVS_AtlasBlend",
          "type": "Vertex"
        },
        {
          "name": "RibbonFS_AtlasBlend",
          "type": "Fragment"
        }
      ]
    },

    "DrawList" : "transparent"
}
