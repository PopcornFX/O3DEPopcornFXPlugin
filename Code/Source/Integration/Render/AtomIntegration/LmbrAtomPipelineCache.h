//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#pragma once

#if defined(LMBR_USE_PK)

#include "Integration/Render/LmbrRendererCache.h"
#include "LmbrAtomFrameCollectorTypes.h"

__LMBRPK_BEGIN
//----------------------------------------------------------------------------

enum	EVertexInputSemantic
{
	DrawInstanceTexCoords_Vertex,

    Position_Vertex,
    TexCoord0_Vertex,

    // Linear atlas blend:
    TexCoord1_Vertex,
    TexFrameLerp_Vertex,

    // Ribbon correct deformation:
    UVRemap_Vertex,
    UVFactors_Vertex,

    // lit particle:
    Normal_Vertex,
    Tangent_Vertex,

	__Max_VertexInputSemantic,
};

namespace RibbonSrg
{
    enum	EBuffersSemantic
    {
    	ParticleDiffuseColor_ShaderRead,
    	ParticleEmissiveColor_ShaderRead,
        ParticleDistortionColor_ShaderRead,
    	ParticleAlphaCursor_ShaderRead,
    
    	__Max_BuffersSemantic,
    };

    enum	EConstantsSemantic
    {
    	RendererFlags_ShaderRead,
    	ParticleCount_ShaderRead,
    
    	__Max_ConstantsSemantic,
    };
}

namespace BillboardSrg
{
    enum	EBuffersSemantic
    {
    	ParticleIndices_ShaderRead,
    	ParticlePositions_ShaderRead,
    	ParticleRotations_ShaderRead,
    	ParticleAxis0_ShaderRead,
    	ParticleAxis1_ShaderRead,
    	ParticleSizes_ShaderRead,
    	ParticleSizes2_ShaderRead,
    	ParticleDiffuseColor_ShaderRead,
    	ParticleEmissiveColor_ShaderRead,
        ParticleDistortionColor_ShaderRead,
    	ParticleAlphaCursor_ShaderRead,
    	ParticleAtlasID_ShaderRead,
    	AtlasSubRects_ShaderRead,
    	DrawRequest_ShaderRead,
    
    	__Max_BuffersSemantic,
    };
    
    enum	EConstantsSemantic
    {
    	RendererFlags_ShaderRead,
    	AtlasSubRectsCount_ShaderRead,
    
    	__Max_ConstantsSemantic,
    };
}

namespace MeshSrg
{
	enum	EBuffersSemantic
	{
		Matrices_ShaderRead,
		ParticleDiffuseColor_ShaderRead,
		ParticleEmissiveColor_ShaderRead,
		ParticleAlphaCursor_ShaderRead,
		DrawRequest_ShaderRead,

		__Max_BuffersSemantic,
	};

	enum	EConstantsSemantic
	{
		RendererFlags_ShaderRead,

		__Max_ConstantsSemantic,
	};
}

enum	EMaterialSrgConstantsSemantic
{
	Roughness_ShaderRead,
	Metalness_ShaderRead,
	MotionVectorsScale_ShaderRead,
	InvSoftnessDistance_ShaderRead,
	MaskThreshold_ShaderRead,

	__Max_MaterialSrgConstantsSemantic,
};

enum	EMaterialSrgTexturesSemantic
{
	DiffuseMap_ShaderRead,
	DiffuseRampMap_ShaderRead,
	EmissiveMap_ShaderRead,
	EmissiveRampMap_ShaderRead,
	NormalMap_ShaderRead,
	MotionVectorsMap_ShaderRead,
	AlphaMap_ShaderRead,
	DistortionMap_ShaderRead,

	__Max_MaterialSrgTexturesSemantic,
};

class	CLmbrAtomPipelineCache
{
public:
	CLmbrAtomPipelineCache();
	~CLmbrAtomPipelineCache();

	bool	InitFromRendererCacheIFN(const CLmbrRendererCache *rendererCache);

	void	SetBillboardingSrgBuffer(BillboardSrg::EBuffersSemantic semantic, const AZ::RHI::Ptr<AZ::RHI::BufferView> bufferView);
	template<typename T>
	void	SetBillboardingSrgConstantValue(BillboardSrg::EConstantsSemantic semantic, const T &value)
	{
		if (semantic >= BillboardSrg::__Max_ConstantsSemantic || !m_BillboardingSrgConstBindIdx[semantic].IsValid())
		{
			PK_ASSERT_NOT_REACHED_MESSAGE("Cannot add shader read value");
			return;
		}
		m_RendererSrg->SetConstant<T>(m_BillboardingSrgConstBindIdx[semantic], value, 0);
	}
	void	SetRibbonSrgBuffer(RibbonSrg::EBuffersSemantic semantic, const AZ::RHI::Ptr<AZ::RHI::BufferView> bufferView);
	template<typename T>
	void	SetRibbonSrgConstantValue(RibbonSrg::EConstantsSemantic semantic, const T &value)
	{
		if (semantic >= RibbonSrg::__Max_ConstantsSemantic|| !m_RibbonSrgConstBindIdx[semantic].IsValid())
		{
			PK_ASSERT_NOT_REACHED_MESSAGE("Cannot add shader read value");
			return;
		}
		m_RendererSrg->SetConstant<T>(m_RibbonSrgConstBindIdx[semantic], value, 0);
	}
	void	SetMeshSrgBuffer(MeshSrg::EBuffersSemantic semantic, const AZ::RHI::Ptr<AZ::RHI::BufferView> bufferView);
	template<typename T>
	void	SetMeshSrgConstantValue(MeshSrg::EConstantsSemantic semantic, const T& value)
	{
		if (semantic >= MeshSrg::__Max_ConstantsSemantic || !m_MeshSrgConstBindIdx[semantic].IsValid())
		{
			PK_ASSERT_NOT_REACHED_MESSAGE("Cannot add shader read value");
			return;
		}
		m_RendererSrg->SetConstant<T>(m_MeshSrgConstBindIdx[semantic], value, 0);
	}
	template<typename T>
	void	SetMaterialSrgConstantValue(EMaterialSrgConstantsSemantic semantic, const T &value)
	{
		if (semantic >= __Max_MaterialSrgConstantsSemantic || !m_MaterialSrgConstBindIdx[semantic].IsValid())
		{
			PK_ASSERT_NOT_REACHED_MESSAGE("Cannot add shader read value");
			return;
		}
		m_MaterialSrg->SetConstant<T>(m_MaterialSrgConstBindIdx[semantic], value, 0);
	}
	void	SetMaterialSrgTexture(EMaterialSrgTexturesSemantic semantic, const AZ::RHI::ImageView *imageView);

	void	SetVertexInputBuffer(EVertexInputSemantic semantic, const AZ::RHI::StreamBufferView &streamBufferView);
	void	SetIndexBuffer(const AZ::RHI::IndexBufferView &idxBufferView);
	void	ConfigureDrawCall(SLmbrAtomDrawOutputs::SDrawCall &drawCall);

	bool	IsInitialized() const { return m_IsInitialized; }

private:
	AZ::Data::Instance<AZ::RPI::ShaderResourceGroup>	_CreateShaderResourceGroup(AZ::Data::Instance<AZ::RPI::Shader> shader, const char *srgName);
	void												_FillMaterialSrgBindIndices(const CLmbrRendererCache *rendererCache);
	void												_FillBillboardingSrgBindIndices(const CLmbrRendererCache *rendererCache);
	void												_FillRibbonSrgBindIndices(const CLmbrRendererCache *rendererCache);
	void												_FillMeshSrgBindIndices(const CLmbrRendererCache* rendererCache);
	void												_FillVertexInputsIndices(const CLmbrRendererCache *rendererCache);

	// Paul H. : Isn't it weird to have rendererSrg hold material buffer? Can't we have a srg for billboarding / mesh matrices
	// and a material srg holding all the material related data, samplers, constants, and per particle material buffers?
	AZ::Data::Instance<AZ::RPI::ShaderResourceGroup>	m_MaterialSrg;
	AZ::RHI::ShaderInputConstantIndex					m_MaterialSrgConstBindIdx[__Max_MaterialSrgConstantsSemantic];
	AZ::RHI::ShaderInputImageIndex						m_MaterialSrgTextureBindIdx[__Max_MaterialSrgTexturesSemantic];

	AZ::Data::Instance<AZ::RPI::ShaderResourceGroup>	m_RendererSrg;
	AZ::RHI::ShaderInputBufferIndex						m_BillboardingSrgBuffBindIdx[BillboardSrg::__Max_BuffersSemantic];
	AZ::RHI::ShaderInputConstantIndex					m_BillboardingSrgConstBindIdx[BillboardSrg::__Max_ConstantsSemantic];
	AZ::RHI::ShaderInputBufferIndex						m_RibbonSrgBuffBindIdx[RibbonSrg::__Max_BuffersSemantic];
	AZ::RHI::ShaderInputConstantIndex					m_RibbonSrgConstBindIdx[RibbonSrg::__Max_ConstantsSemantic];
	AZ::RHI::ShaderInputBufferIndex						m_MeshSrgBuffBindIdx[MeshSrg::__Max_BuffersSemantic];
	AZ::RHI::ShaderInputConstantIndex					m_MeshSrgConstBindIdx[MeshSrg::__Max_ConstantsSemantic];

	AZ::Data::Instance<AZ::RPI::ShaderResourceGroup>	m_ObjectSrg;

	CGuid												m_VertexInputsIdx[__Max_VertexInputSemantic];

	AZ::RHI::IndexBufferView							m_IndexBuffer;
	TSemiDynamicArray<AZ::RHI::StreamBufferView, 0x10>	m_VertexInputs;

	AZ::RHI::ConstPtr<AZ::RHI::PipelineState>			m_MaterialPipelineState;
	AZ::RHI::DrawListTag								m_MaterialDrawList;

	AZ::RHI::ConstPtr<AZ::RHI::PipelineState>			m_OpaqueDepthPipelineState;
	AZ::RHI::DrawListTag								m_OpaqueDepthDrawList;

	AZ::RHI::ConstPtr<AZ::RHI::PipelineState>			m_TransparentDepthMinPipelineState;
	AZ::RHI::DrawListTag								m_TransparentDepthMinDrawList;

	AZ::RHI::ConstPtr<AZ::RHI::PipelineState>			m_TransparentDepthMaxPipelineState;
	AZ::RHI::DrawListTag								m_TransparentDepthMaxDrawList;

	bool												m_IsInitialized;
};

//----------------------------------------------------------------------------
__LMBRPK_END

#endif
