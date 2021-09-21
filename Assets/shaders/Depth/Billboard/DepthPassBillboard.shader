{
     "ProgramSettings" : 
     {
         "EntryPoints":
         [
             {
                 "name": "DepthPassVS",
                 "type" : "Vertex"
             }
         ] 
     },

    "Source" : "DepthPassBillboard.azsl",

    "DepthStencilState" : { 
        "Depth" : { "Enable" : true, "CompareFunc" : "GreaterEqual" }
    },

    "DrawList" : "depth"
}