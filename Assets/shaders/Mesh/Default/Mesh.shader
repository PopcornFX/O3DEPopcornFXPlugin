{
    "Source" : "Mesh.azsl",

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
          "name": "MeshVS",
          "type": "Vertex"
        },
        {
          "name": "MeshFS",
          "type": "Fragment"
        }
      ]
    },

    "DrawList" : "transparent"
}
