{
    "Source" : "RibbonLit.azsl",

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
          "name": "RibbonVS_CorrectDeformation",
          "type": "Vertex"
        },
        {
          "name": "RibbonFS_CorrectDeformation",
          "type": "Fragment"
        }
      ]
    },

    "DrawList" : "transparent"
}
