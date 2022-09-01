{ 
    "Source" : "DistortionPostProcess.azsl",

    "DepthStencilState" : 
    {
        "Depth" : 
        { 
            "Enable" : false 
        },
        "Stencil" :
        {
            "Enable" : false
        }
    },

    "BlendState" : {
        "Enable" : true,
        "BlendSource" : "One",
        "BlendDest" : "Zero",
        "BlendOp" : "Add"
    },

    "DrawList" : "forward",

    "ProgramSettings":
    {
      "EntryPoints":
      [
        {
          "name": "MainVS",
          "type": "Vertex"
        },
        {
          "name": "MainPS",
          "type": "Fragment"
        }
      ]
    }   
}
