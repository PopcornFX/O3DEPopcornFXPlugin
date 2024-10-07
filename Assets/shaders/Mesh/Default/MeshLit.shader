{
    "Source" : "MeshLit.azsl",

    "DepthStencilState" :
    {
        "Depth" :
        {
            "Enable" : true,
            "CompareFunc" : "GreaterEqual"
        },
        "Stencil" :
        {
            "Enable" : true,
            "ReadMask" : "0x00",
            "WriteMask" : "0xFF",
            "FrontFace" :
            {
                "Func" : "Always",
                "DepthFailOp" : "Keep",
                "FailOp" : "Keep",
                "PassOp" : "Replace"
            },
            "BackFace" :
            {
                "Func" : "Always",
                "DepthFailOp" : "Keep",
                "FailOp" : "Keep",
                "PassOp" : "Replace"
            }
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
          "name": "MeshVS",
          "type": "Vertex"
        },
        {
          "name": "MeshFS",
          "type": "Fragment"
        }
      ]
    },

    "DrawList" : "forward"
}
