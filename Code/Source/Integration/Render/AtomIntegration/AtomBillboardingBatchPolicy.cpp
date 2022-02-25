//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "PopcornFX_precompiled.h"
#include "Integration/Managers/RenderManager.h"
#include "AtomBillboardingBatchPolicy.h"

#if defined(O3DE_USE_PK)

#include <pk_render_helpers/include/basic_renderer_properties/rh_basic_renderer_properties.h>
#include <pk_particles/include/Renderers/ps_renderer_feature_fields.h>

namespace PopcornFX {
//----------------------------------------------------------------------------

//! Counts leading zeros
uint32_t CountLeadingZeros32(uint32_t x)
{
	return x == 0 ? 32 : az_clz_u32(x);
}

//! Integer log2
uint32_t IntegerLog2(uint32_t x)
{
	return 31 - CountLeadingZeros32(x);
}

const u32 CParticleBuffers::kBufferStrides[] =
{
	0,	// GenBuffer_Indices

	// Per vertex:
	sizeof(CFloat4), // GenBuffer_Position
	sizeof(CFloat4), // GenBuffer_Normal
	sizeof(CFloat4), // GenBuffer_Tangent
	sizeof(CFloat2), // GenBuffer_UV0
	sizeof(CFloat2), // GenBuffer_UV1
	sizeof(float), // GenBuffer_AtlasId
	sizeof(CFloat4), // GenBuffer_UVRemap
	sizeof(CFloat2), // GenBuffer_UVFactors

	// Per particle:
	sizeof(CFloat4x4), // GenBuffer_Matrices
	sizeof(Drawers::SVertex_PositionDrId), // GenBuffer_ParticlePosition
	sizeof(float), // GenBuffer_ParticleSize
	sizeof(CFloat2), // GenBuffer_ParticleSize2
	sizeof(float), // GenBuffer_ParticleRotation
	sizeof(CFloat3), // GenBuffer_ParticleAxis0
	sizeof(CFloat3), // GenBuffer_ParticleAxis1
	sizeof(CFloat3), // GenBuffer_ParticlePosition0
	sizeof(CFloat3), // GenBuffer_ParticlePosition1
	sizeof(CFloat3), // GenBuffer_ParticlePosition2
};

PK_STATIC_ASSERT(PK_ARRAY_COUNT(CParticleBuffers::kBufferStrides) == CParticleBuffers::__GenBuffer_Count);

void		CParticleBuffers::ResizeOrCreateAdditionalFieldBufferIFN(CStringId additionalFieldName, CRenderManager *renderManager, AZ::u64 bufferSize)
{
	SAdditionalField	*field = m_AdditionalFields.Find(additionalFieldName);

	if (field == null)
	{
		if (PK_VERIFY(m_AdditionalFields.PushBack(additionalFieldName).Valid()))
			field = &m_AdditionalFields.Last();
	}
	if (field)
		field->m_Buffer = renderManager->ResizeOrCreateBufferIFN(field->m_Buffer, bufferSize);
}

//----------------------------------------------------------------------------

void		CParticleBuffers::UnmapAllAdditionalFieldBuffers(CRenderManager *renderManager)
{
	for (u32 i = 0; i < m_AdditionalFields.Count(); ++i)
		renderManager->UnmapBuffer(m_AdditionalFields[i].m_Buffer);
}

//----------------------------------------------------------------------------

AZ::RHI::Ptr<AZ::RHI::Buffer>				CParticleBuffers::FindAdditionalFieldBuffer(CStringId additionalFieldName)
{
	SAdditionalField *field = m_AdditionalFields.Find(additionalFieldName);

	if (field == null)
		return null;
	return field->m_Buffer;
}

void	CParticleBuffers::Reset()
{
	m_AdditionalFields.Clear();
}

//----------------------------------------------------------------------------

void	CParticleBuffers::UnmapAll(CRenderManager *renderManager)
{
	for (u32 i = 0; i < __GenBuffer_Count; ++i)
	{
		if (m_ViewIndependent.m_GenBuffers[i] != null)
			renderManager->UnmapBuffer(m_ViewIndependent.m_GenBuffers[i]);
	}
	if (m_ViewIndependent.m_DrawRequests != null)
		renderManager->UnmapBuffer(m_ViewIndependent.m_DrawRequests);
	for (u32 i = 0; i < __GenBuffer_Count; ++i)
	{
		if (m_ViewDependent.m_GenBuffers[i] != null)
			renderManager->UnmapBuffer(m_ViewDependent.m_GenBuffers[i]);
	}
	UnmapAllAdditionalFieldBuffers(renderManager);
}

//----------------------------------------------------------------------------

CAtomBillboardingBatchPolicy::CAtomBillboardingBatchPolicy()
:	m_VertexCount(0)
,	m_ParticleCount(0)
,	m_DrawRequestCount(0)
,	m_IndexCount(0)
,	m_UseLargeIndices(true)
,	m_RendererType(Renderer_Invalid)
{
	m_HandledAdditionalFields.PushBack(SHandledAdditionalFields(BasicRendererProperties::SID_Diffuse_Color(), BaseType_Float4));
	m_HandledAdditionalFields.PushBack(SHandledAdditionalFields(BasicRendererProperties::SID_Emissive_EmissiveColor(), BaseType_Float3));
	m_HandledAdditionalFields.PushBack(SHandledAdditionalFields(BasicRendererProperties::SID_Distortion_Color(), BaseType_Float4));
	m_HandledAdditionalFields.PushBack(SHandledAdditionalFields(BasicRendererProperties::SID_AlphaRemap_Cursor(), BaseType_Float));
	m_HandledAdditionalFields.PushBack(SHandledAdditionalFields(BasicRendererProperties::SID_Atlas_TextureID(), BaseType_Float));
}

//----------------------------------------------------------------------------

CAtomBillboardingBatchPolicy::~CAtomBillboardingBatchPolicy()
{
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::AreBillboardingBatchable(const PCRendererCacheBase &firstCache, const PCRendererCacheBase &secondCache) const
{
	const CAtomRendererCache	*firstAtomCache = static_cast<const CAtomRendererCache*>(firstCache.Get());
	const CAtomRendererCache	*secondAtomCache = static_cast<const CAtomRendererCache*>(secondCache.Get());

	// Here, you need to implement a proper renderer cache compatibility check
	// Same materials/shader/uniforms/.. should return true
	return *firstAtomCache == *secondAtomCache; // overloaded == operator (see AtomRendererCache.cpp)
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::Tick(SAtomRenderContext &ctx, const TMemoryView<SAtomSceneView> &views)
{
	AZ_UNUSED(views); AZ_UNUSED(ctx);
	// Tick function called on draw calls thread, here we remove ourselves if we haven't been used for rendering after 10 (collected) frames:
	// 10 PKFX Update()/UpdateFence() without being drawn
	if (m_UnusedCounter++ > 10)
		return false;
	return true;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::AllocBuffers(SAtomRenderContext &ctx, const SBuffersToAlloc &allocBuffers, const TMemoryView<SAtomSceneView> &views, ERendererClass rendererType)
{
	PK_NAMEDSCOPEDPROFILE("CAtomBillboardingBatchPolicy::AllocBuffers");
	PK_ASSERT(!allocBuffers.m_DrawRequests.Empty());
	PK_ASSERT(allocBuffers.m_DrawRequests.Count() == allocBuffers.m_RendererCaches.Count());
	PK_ASSERT((allocBuffers.m_TotalVertexCount > 0 && allocBuffers.m_TotalIndexCount > 0) || allocBuffers.m_TotalParticleCount > 0);
	PK_ASSERT(allocBuffers.m_DrawRequests.First() != null);

	m_UnusedCounter = 0;

	++m_FrameIdx;
	m_VertexCount = allocBuffers.m_TotalVertexCount;
	m_ParticleCount = allocBuffers.m_TotalParticleCount;
	m_DrawRequestCount = allocBuffers.m_DrawRequests.Count();
	m_IndexCount = allocBuffers.m_TotalIndexCount;
	m_PerMeshParticleCount = allocBuffers.m_PerMeshParticleCount;
	m_UseLargeIndices = true; // For now only 32bits indices
	m_Views = views;

	if (!PK_VERIFY(allocBuffers.m_RendererCaches.Count() > 0))
		return false;

	CAtomRendererCache *rendererCache = static_cast<CAtomRendererCache*>(allocBuffers.m_RendererCaches.First().Get());
	if (!PK_VERIFY(rendererCache != null))
		return false;

	if (!m_Initialized)
	{
		PK_NAMEDSCOPEDPROFILE("CAtomBillboardingBatchPolicy::AllocBuffers Initialize");
		m_RenderContext = &ctx;
		m_RendererType = rendererType;

		if (m_RendererType == Renderer_Billboard ||
			m_RendererType == Renderer_Ribbon)
		{
			if (rendererCache->m_Atlas != null)
			{
				m_AtlasSubRectsCount = rendererCache->m_Atlas->m_RectsFp32.Count();
				m_AtlasDefinition = m_RenderContext->m_RenderManager->ResizeOrCreateBufferIFN(m_AtlasDefinition, m_AtlasSubRectsCount * sizeof(CFloat4), 0x10);
				if (!PK_VERIFY(m_AtlasDefinition != null))
					return false;
				CFloat4	*atlasSubRects = static_cast<CFloat4*>(m_RenderContext->m_RenderManager->MapBuffer(m_AtlasDefinition, m_AtlasSubRectsCount * sizeof(CFloat4)));
				if (!PK_VERIFY(atlasSubRects != null))
					return false;
				Mem::Copy(atlasSubRects, rendererCache->m_Atlas->m_RectsFp32.RawDataPointer(), m_AtlasSubRectsCount * sizeof(CFloat4));
				m_RenderContext->m_RenderManager->UnmapBuffer(m_AtlasDefinition);
			}
		}

		m_PipelineCaches.Resize(1);

		if (m_RendererType == ERendererClass::Renderer_Billboard)
		{
			if (rendererCache->m_BasicDescription.HasOneRendererFlags(RendererFlags::Has_Capsules))
			{
				m_DrawInstanceIdxCount = 12;
				m_DrawInstanceVtxCount = 6;
			}
			else
			{
				m_DrawInstanceIdxCount = 6;
				m_DrawInstanceVtxCount = 4;
			}

			// Create draw instance buffers:
			m_DrawInstanceVtx = m_RenderContext->m_RenderManager->ResizeOrCreateBufferIFN(m_DrawInstanceVtx, 6 * sizeof(CFloat2), 0x10);
			if (!PK_VERIFY(m_DrawInstanceVtx != null))
				return false;

			float	*texCoords = static_cast<float*>(m_RenderContext->m_RenderManager->MapBuffer(m_DrawInstanceVtx, 6 * sizeof(CFloat2)));
			if (!PK_VERIFY(texCoords != null))
				return false;

			texCoords[0] = -1.0f; // Lower left corner
			texCoords[1] = -1.0f;
			texCoords[2] = -1.0f; // Upper left corner
			texCoords[3] = 1.0f;
			texCoords[4] = 1.0f; // Upper right corner
			texCoords[5] = 1.0f;
			texCoords[6] = 1.0f; // Lower right corner
			texCoords[7] = -1.0f;
			texCoords[8] = 0.0f; // Capsule up
			texCoords[9] = 2.0f;
			texCoords[10] = 0.0f; // Capsule down
			texCoords[11] = -2.0f;

			m_RenderContext->m_RenderManager->UnmapBuffer(m_DrawInstanceVtx);

			m_DrawInstanceIdx = m_RenderContext->m_RenderManager->ResizeOrCreateBufferIFN(m_DrawInstanceIdx, 12 * sizeof(u16), 0x10);
			if (!PK_VERIFY(m_DrawInstanceIdx != null))
				return false;

			u16	*indices = static_cast<u16*>(m_RenderContext->m_RenderManager->MapBuffer(m_DrawInstanceIdx, 12 * sizeof(u16)));
			if (!PK_VERIFY(indices != null))
				return false;

			// In O3DE, counter clockwise winding order defines front face
			indices[0] = 0;
			indices[1] = 3;
			indices[2] = 1;
			indices[3] = 1;
			indices[4] = 3;
			indices[5] = 2;
			indices[6] = 0;
			indices[7] = 5;
			indices[8] = 3;
			indices[9] = 1;
			indices[10] = 2;
			indices[11] = 4;

			m_RenderContext->m_RenderManager->UnmapBuffer(m_DrawInstanceIdx);
		}
		else if (m_RendererType == ERendererClass::Renderer_Mesh)
		{
			m_GeometryCache = rendererCache->m_CacheFactory->FindGeometryCache(rendererCache->m_BasicDescription.m_MeshPath.ToString());
			if (!PK_VERIFY(m_GeometryCache != null))
				return false;
			if (!m_GeometryCache->m_Modified) // not ready yet
				return true;

			m_DrawInstanceIdxCount = 0;
			m_DrawInstanceVtxCount = 0;
		}
		m_Initialized = true;
	}
	if (m_RendererType == Renderer_Light)
	{
		// All good, nothing else to do
		return true;
	}

	// This happen at initialization but also if the geometry cache is reloaded
	// with a different mesh count (enable/disable source merge mesh option)
	if (m_RendererType == ERendererClass::Renderer_Mesh)
	{
		if (m_PipelineCaches.Count() != m_GeometryCache->m_PerGeometryViews.Count())
			m_PipelineCaches.Resize(m_GeometryCache->m_PerGeometryViews.Count());
	}
	for (auto &pipelineCache : m_PipelineCaches)
		pipelineCache.InitFromRendererCacheIFN(rendererCache);

	{
		PK_NAMEDSCOPEDPROFILE("CAtomBillboardingBatchPolicy::AllocBuffers Alloc additional inputs");
		const u32	additionalShaderInputsCount = allocBuffers.m_ToGenerate.m_AdditionalGeneratedInputs.Count();

		for (u32 i = 0; i < additionalShaderInputsCount; ++i)
		{
			const SRendererFeatureFieldDefinition	&curAdditionalShaderInput = allocBuffers.m_ToGenerate.m_AdditionalGeneratedInputs[i];
			SHandledAdditionalFields	*addFieldDesc = m_HandledAdditionalFields.Find(SHandledAdditionalFields(curAdditionalShaderInput.m_Name, curAdditionalShaderInput.m_Type));
			if (addFieldDesc != null)
			{
				const AZ::u64	typeByteSize = CBaseTypeTraits::Traits(curAdditionalShaderInput.m_Type).Size;
				GetCurBuffers().ResizeOrCreateAdditionalFieldBufferIFN(curAdditionalShaderInput.m_Name, m_RenderContext->m_RenderManager, m_ParticleCount * typeByteSize);
			}
		}
	}

	if (!PK_VERIFY(_AllocViewIndependentBuffers(allocBuffers.m_ToGenerate)))
		return false;
	if (!PK_VERIFY(_AllocViewDependentBuffers(allocBuffers.m_ToGenerate)))
		return false;

	{
		PK_NAMEDSCOPEDPROFILE("CAtomBillboardingBatchPolicy::AllocBuffers Compute BBox");
		CAABB	bbox = CAABB::DEGENERATED;

		for (u32 i = 0; i < allocBuffers.m_DrawRequests.Count(); ++i)
		{
			bbox.Add(allocBuffers.m_DrawRequests[i]->StreamToRender().BBox());
		}
		//tmp fix assert
		if (!bbox.Valid())
		{
			bbox = CAABB::ZERO;
		}
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::MapBuffers(SAtomRenderContext &ctx, const TMemoryView<SAtomSceneView> &views, SBillboardBatchJobs *billboardBatch, const SGeneratedInputs &toMap)
{
	AZ_UNUSED(ctx); AZ_UNUSED(views); AZ_UNUSED(billboardBatch); AZ_UNUSED(toMap);
	return false;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::MapBuffers(SAtomRenderContext &ctx, const TMemoryView<SAtomSceneView> &views, SRibbonBatchJobs *billboardBatch, const SGeneratedInputs &toMap)
{
	AZ_UNUSED(ctx); AZ_UNUSED(views); AZ_UNUSED(billboardBatch); AZ_UNUSED(toMap);

	CRenderManager								*renderManager = m_RenderContext->m_RenderManager;
	const CParticleBuffers::SViewIndependent	&viewIndependent = GetCurBuffers().m_ViewIndependent;

	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices] != null)
	{
		const AZ::u64	idxSize = m_UseLargeIndices ? sizeof(AZ::u32) : sizeof(AZ::u16);
		void	*data = renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices], m_IndexCount * idxSize);
		billboardBatch->m_Exec_Indices.m_IndexStream.Setup(data, m_IndexCount, m_UseLargeIndices);
	}
	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Position] != null)
	{
		CFloat3		*data = static_cast<CFloat3*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Position], m_VertexCount * sizeof(CFloat4)));
		billboardBatch->m_Exec_PNT.m_Positions = TStridedMemoryView<CFloat3, 0x10>(data, m_VertexCount, 0x10);
	}
	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Normal] != null)
	{
		CFloat3		*data = static_cast<CFloat3*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Normal], m_VertexCount * sizeof(CFloat4)));
		billboardBatch->m_Exec_PNT.m_Normals = TStridedMemoryView<CFloat3, 0x10>(data, m_VertexCount, 0x10);
	}
	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Tangent] != null)
	{
		CFloat4		*data = static_cast<CFloat4*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Tangent], m_VertexCount * sizeof(CFloat4)));
		billboardBatch->m_Exec_PNT.m_Tangents = TStridedMemoryView<CFloat4>(data, m_VertexCount, 0x10);
	}
	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UV0] != null)
	{
		CFloat2		*data = static_cast<CFloat2*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UV0], m_VertexCount * sizeof(CFloat2)));
		billboardBatch->m_Exec_Texcoords.m_Texcoords = TMemoryView<CFloat2>(data, m_VertexCount);
	}
	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UV1] != null)
	{
		CFloat2		*data = static_cast<CFloat2*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UV1], m_VertexCount * sizeof(CFloat2)));
		billboardBatch->m_Exec_Texcoords.m_Texcoords2 = TMemoryView<CFloat2>(data, m_VertexCount);
	}
	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_AtlasId] != null)
	{
		float		*data = static_cast<float*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_AtlasId], m_VertexCount * sizeof(float)));
		billboardBatch->m_Exec_Texcoords.m_AtlasIds = TMemoryView<float>(data, m_VertexCount);
	}
	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UVRemap] != null)
	{
		CFloat4		*data = static_cast<CFloat4*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UVRemap], m_VertexCount * sizeof(CFloat4)));
		billboardBatch->m_Exec_UVRemap.m_UVRemap = TMemoryView<CFloat4>(data, m_VertexCount);
		billboardBatch->m_Exec_Texcoords.m_ForUVFactor = true;
	}
	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UVFactors] != null)
	{
		CFloat2		*data = static_cast<CFloat2*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UVFactors], m_VertexCount * sizeof(CFloat2)));
		billboardBatch->m_Exec_PNT.m_UVFactors = TMemoryView<CFloat2>(data, m_VertexCount);
	}

	billboardBatch->m_Exec_CopyField.m_PerVertex = false;

	// Additional fields:
	const u32	additionalShaderInputsCount = toMap.m_AdditionalGeneratedInputs.Count();

	m_MappedAdditionalShaderInputs.Clear();
	if (!PK_VERIFY(m_MappedAdditionalShaderInputs.Reserve(m_HandledAdditionalFields.Count())))
		return false;
	for (u32 i = 0; i < additionalShaderInputsCount; ++i)
	{
		const SRendererFeatureFieldDefinition	&curAdditionalShaderInput = toMap.m_AdditionalGeneratedInputs[i];
		SHandledAdditionalFields	*addFieldDesc = m_HandledAdditionalFields.Find(SHandledAdditionalFields(curAdditionalShaderInput.m_Name, curAdditionalShaderInput.m_Type));
		if (addFieldDesc != null)
		{
			if (!PK_VERIFY(m_MappedAdditionalShaderInputs.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc			&desc = m_MappedAdditionalShaderInputs.Last();
			AZ::RHI::Ptr<AZ::RHI::Buffer>	buff = GetCurBuffers().FindAdditionalFieldBuffer(curAdditionalShaderInput.m_Name);
			const AZ::u64					typeByteSize = CBaseTypeTraits::Traits(curAdditionalShaderInput.m_Type).Size;
			u8								*data = static_cast<u8*>(renderManager->MapBuffer(buff, m_ParticleCount * typeByteSize));

			desc.m_AdditionalInputIndex = i;
			desc.m_Storage.m_RawDataPtr = data;
			desc.m_Storage.m_Count = m_ParticleCount;
			desc.m_Storage.m_Stride = static_cast<u32>(typeByteSize);
		}
	}

	billboardBatch->m_Exec_CopyField.m_FieldsToCopy = m_MappedAdditionalShaderInputs;

	// -----------------------------------------
	// View dependent inputs:
	// -----------------------------------------
	const CParticleBuffers::SViewDependent	&viewDependent = GetCurBuffers().m_ViewDependent;
	const u32								viewCount = PKMin(toMap.m_PerViewGeneratedInputs.Count(), 1);

	for (u32 i = 0; i < viewCount; ++i)
	{
		//const SGeneratedInputs::SViewGeneratedInputs	&curView = toMap.m_PerViewGeneratedInputs[i];
		SRibbonBatchJobs::SPerView						&curViewJobs = billboardBatch->m_PerView[i];

		if (viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices] != null)
		{
			const AZ::u64	idxSize = m_UseLargeIndices ? sizeof(AZ::u32) : sizeof(AZ::u16);
			void			*data = renderManager->MapBuffer(viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices], m_IndexCount * idxSize);
			curViewJobs.m_Exec_Indices.m_IndexStream.Setup(data, m_IndexCount, m_UseLargeIndices);
		}
		if (viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Position] != null)
		{
			CFloat3		*data = static_cast<CFloat3*>(renderManager->MapBuffer(viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Position], m_VertexCount * sizeof(CFloat4)));
			curViewJobs.m_Exec_PNT.m_Positions = TStridedMemoryView<CFloat3, 0x10>(data, m_VertexCount, 0x10);
		}
		if (viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Normal] != null)
		{
			CFloat3		*data = static_cast<CFloat3*>(renderManager->MapBuffer(viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Normal], m_VertexCount * sizeof(CFloat4)));
			curViewJobs.m_Exec_PNT.m_Normals = TStridedMemoryView<CFloat3, 0x10>(data, m_VertexCount, 0x10);
		}
		if (viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Tangent] != null)
		{
			CFloat4		*data = static_cast<CFloat4*>(renderManager->MapBuffer(viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Tangent], m_VertexCount * sizeof(CFloat4)));
			curViewJobs.m_Exec_PNT.m_Tangents = TStridedMemoryView<CFloat4>(data, m_VertexCount, 0x10);
		}
		if (viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_UVFactors] != null)
		{
			CFloat2		*data = static_cast<CFloat2*>(renderManager->MapBuffer(viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_UVFactors], m_VertexCount * sizeof(CFloat2)));
			curViewJobs.m_Exec_PNT.m_UVFactors = TMemoryView<CFloat2>(data, m_VertexCount);
		}
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::MapBuffers(SAtomRenderContext &ctx, const TMemoryView<SAtomSceneView> &views, SGPUBillboardBatchJobs *billboardBatch, const SGeneratedInputs &toMap)
{
	AZ_UNUSED(ctx); AZ_UNUSED(views);

	CRenderManager								*renderManager = m_RenderContext->m_RenderManager;
	const CParticleBuffers::SViewIndependent	&viewIndependent = GetCurBuffers().m_ViewIndependent;

	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices] != null)
	{
		const AZ::u64	idxSize = m_UseLargeIndices ? sizeof(AZ::u32) : sizeof(AZ::u16);
		void	*data = renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices], m_IndexCount * idxSize);
		billboardBatch->m_Exec_Indices.m_IndexStream.Setup(data, m_IndexCount, m_UseLargeIndices);
	}
	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticlePosition] != null)
	{
		Drawers::SVertex_PositionDrId	*data = static_cast<Drawers::SVertex_PositionDrId*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticlePosition], m_ParticleCount * sizeof(CFloat4)));
		billboardBatch->m_Exec_CopyBillboardingStreams.m_PositionsDrIds = TMemoryView<Drawers::SVertex_PositionDrId>(data, m_ParticleCount);
	}
	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleRotation] != null)
	{
		float	*data = static_cast<float*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleRotation], m_ParticleCount * sizeof(float)));
		billboardBatch->m_Exec_CopyBillboardingStreams.m_Rotations = TMemoryView<float>(data, m_ParticleCount);
	}
	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleSize] != null)
	{
		float	*data = static_cast<float*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleSize], m_ParticleCount * sizeof(float)));
		billboardBatch->m_Exec_CopyBillboardingStreams.m_Sizes = TMemoryView<float>(data, m_ParticleCount);
	}
	else if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleSize2] != null)
	{
		CFloat2	*data = static_cast<CFloat2*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleSize2], m_ParticleCount * sizeof(CFloat2)));
		billboardBatch->m_Exec_CopyBillboardingStreams.m_Sizes2 = TMemoryView<CFloat2>(data, m_ParticleCount);
	}
	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleAxis0] != null)
	{
		CFloat3	*data = static_cast<CFloat3*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleAxis0], m_ParticleCount * sizeof(CFloat3)));
		billboardBatch->m_Exec_CopyBillboardingStreams.m_Axis0 = TMemoryView<CFloat3>(data, m_ParticleCount);
	}
	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleAxis1] != null)
	{
		CFloat3	*data = static_cast<CFloat3*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleAxis1], m_ParticleCount * sizeof(CFloat3)));
		billboardBatch->m_Exec_CopyBillboardingStreams.m_Axis1 = TMemoryView<CFloat3>(data, m_ParticleCount);
	}

	const u32	additionalShaderInputsCount = toMap.m_AdditionalGeneratedInputs.Count();

	m_MappedAdditionalShaderInputs.Clear();
	if (!PK_VERIFY(m_MappedAdditionalShaderInputs.Reserve(m_HandledAdditionalFields.Count())))
		return false;
	for (u32 i = 0; i < additionalShaderInputsCount; ++i)
	{
		const SRendererFeatureFieldDefinition	&curAdditionalShaderInput = toMap.m_AdditionalGeneratedInputs[i];
		SHandledAdditionalFields	*addFieldDesc = m_HandledAdditionalFields.Find(SHandledAdditionalFields(curAdditionalShaderInput.m_Name, curAdditionalShaderInput.m_Type));
		if (addFieldDesc != null)
		{
			if (!PK_VERIFY(m_MappedAdditionalShaderInputs.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc			&desc = m_MappedAdditionalShaderInputs.Last();
			AZ::RHI::Ptr<AZ::RHI::Buffer>	buff = GetCurBuffers().FindAdditionalFieldBuffer(curAdditionalShaderInput.m_Name);
			const AZ::u64					typeByteSize = CBaseTypeTraits::Traits(curAdditionalShaderInput.m_Type).Size;
			u8								*data = static_cast<u8*>(renderManager->MapBuffer(buff, m_ParticleCount * typeByteSize));

			desc.m_AdditionalInputIndex = i;
			desc.m_Storage.m_RawDataPtr = data;
			desc.m_Storage.m_Count = m_ParticleCount;
			desc.m_Storage.m_Stride = static_cast<u32>(typeByteSize);
		}
	}
	billboardBatch->m_Exec_CopyAdditionalFields.m_FieldsToCopy = m_MappedAdditionalShaderInputs;

	// Draw request buffer:
	if (viewIndependent.m_DrawRequests != null)
	{
		Drawers::SBillboardDrawRequest *data = static_cast<Drawers::SBillboardDrawRequest*>(renderManager->MapBuffer(viewIndependent.m_DrawRequests, m_DrawRequestCount * sizeof(Drawers::SBillboardDrawRequest)));
		billboardBatch->m_Exec_GeomBillboardDrawRequests.m_GeomDrawRequests = TMemoryView<Drawers::SBillboardDrawRequest>(data, m_DrawRequestCount);
	}

	// -----------------------------------------
	// View dependent inputs:
	// -----------------------------------------
	const CParticleBuffers::SViewDependent	&viewDependent = GetCurBuffers().m_ViewDependent;
	const u32								viewCount = PKMin(toMap.m_PerViewGeneratedInputs.Count(), 1);

	for (u32 i = 0; i < viewCount; ++i)
	{
		//const SGeneratedInputs::SViewGeneratedInputs	&curView = toMap.m_PerViewGeneratedInputs[i];
		SGPUBillboardBatchJobs::SPerView				&curViewJobs = billboardBatch->m_PerView[i];

		if (viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices] != null)
		{
			const AZ::u64	idxSize = m_UseLargeIndices ? sizeof(AZ::u32) : sizeof(AZ::u16);
			void			*data = renderManager->MapBuffer(viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices], m_IndexCount * idxSize);
			curViewJobs.m_Exec_Indices.m_IndexStream.Setup(data, m_IndexCount, m_UseLargeIndices);
		}
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::MapBuffers(SAtomRenderContext &ctx, const TMemoryView<SAtomSceneView> &views, SGPURibbonBatchJobs *billboardBatch, const SGeneratedInputs &toMap)
{

	AZ_UNUSED(ctx); AZ_UNUSED(views); AZ_UNUSED(billboardBatch); AZ_UNUSED(toMap);
	return false;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::MapBuffers(SAtomRenderContext &ctx, const TMemoryView<SAtomSceneView> &views, SMeshBatchJobs *billboardBatch, const SGeneratedInputs &toMap)
{
	AZ_UNUSED(ctx); AZ_UNUSED(views); AZ_UNUSED(billboardBatch); AZ_UNUSED(toMap);

	CRenderManager	*renderManager = m_RenderContext->m_RenderManager;
	const CParticleBuffers::SViewIndependent	&viewIndependent = GetCurBuffers().m_ViewIndependent;

	if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Matrices] != null)
	{
		CFloat4x4	*data = static_cast<CFloat4x4*>(renderManager->MapBuffer(viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Matrices], m_ParticleCount * sizeof(CFloat4x4)));
		billboardBatch->m_Exec_Matrices.m_Matrices = TMemoryView<CFloat4x4>(data, m_ParticleCount);
	}

	const u32	additionalShaderInputsCount = toMap.m_AdditionalGeneratedInputs.Count();

	m_MappedAdditionalShaderInputs.Clear();
	if (!PK_VERIFY(m_MappedAdditionalShaderInputs.Reserve(m_HandledAdditionalFields.Count())))
		return false;
	for (u32 i = 0; i < additionalShaderInputsCount; ++i)
	{
		const SRendererFeatureFieldDefinition	&curAdditionalShaderInput = toMap.m_AdditionalGeneratedInputs[i];
		SHandledAdditionalFields	*addFieldDesc = m_HandledAdditionalFields.Find(SHandledAdditionalFields(curAdditionalShaderInput.m_Name, curAdditionalShaderInput.m_Type));
		if (addFieldDesc != null)
		{
			if (!PK_VERIFY(m_MappedAdditionalShaderInputs.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc			&desc = m_MappedAdditionalShaderInputs.Last();
			AZ::RHI::Ptr<AZ::RHI::Buffer>	buff = GetCurBuffers().FindAdditionalFieldBuffer(curAdditionalShaderInput.m_Name);
			const AZ::u64					typeByteSize = CBaseTypeTraits::Traits(curAdditionalShaderInput.m_Type).Size;
			u8								*data = static_cast<u8*>(renderManager->MapBuffer(buff, m_ParticleCount * typeByteSize));

			desc.m_AdditionalInputIndex = i;
			desc.m_Storage.m_RawDataPtr = data;
			desc.m_Storage.m_Count = m_ParticleCount;
			desc.m_Storage.m_Stride = static_cast<u32>(typeByteSize);
		}
	}

	billboardBatch->m_Exec_CopyField.m_FieldsToCopy = m_MappedAdditionalShaderInputs.View();

	return true;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::MapBuffers(SAtomRenderContext &ctx, const TMemoryView<SAtomSceneView> &views, SDecalBatchJobs *decalBatch, const SGeneratedInputs &toMap)
{
	AZ_UNUSED(ctx); AZ_UNUSED(views); AZ_UNUSED(decalBatch); AZ_UNUSED(toMap);
	return false;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::LaunchCustomTasks(SAtomRenderContext &ctx, const TMemoryView<const Drawers::SBillboard_DrawRequest * const> drawRequests, Drawers::CBillboard_CPU *billboardBatch)
{
	AZ_UNUSED(ctx); AZ_UNUSED(drawRequests); AZ_UNUSED(billboardBatch);
	return true;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::LaunchCustomTasks(SAtomRenderContext &ctx, const TMemoryView<const Drawers::SRibbon_DrawRequest * const> drawRequests, Drawers::CRibbon_CPU *ribbonBatch)
{
	AZ_UNUSED(ctx); AZ_UNUSED(drawRequests); AZ_UNUSED(ribbonBatch);
	return true;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::LaunchCustomTasks(SAtomRenderContext &ctx, const TMemoryView<const Drawers::SRibbon_DrawRequest * const> drawRequests, Drawers::CCopyStream_CPU *ribbonBatch)
{
	AZ_UNUSED(ctx); AZ_UNUSED(drawRequests); AZ_UNUSED(ribbonBatch);
	return true;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::LaunchCustomTasks(SAtomRenderContext &ctx, const TMemoryView<const Drawers::SDecal_DrawRequest * const> drawRequests, Drawers::CDecal_CPU *decalBatch)
{
	AZ_UNUSED(ctx); AZ_UNUSED(decalBatch);
	return true;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::WaitForCustomTasks(SAtomRenderContext &ctx)
{
	AZ_UNUSED(ctx);
	return true;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::UnmapBuffers(SAtomRenderContext &ctx)
{
	AZ_UNUSED(ctx);
	CRenderManager	*renderManager = m_RenderContext->m_RenderManager;
	GetCurBuffers().UnmapAll(renderManager);
	return true;
}

//----------------------------------------------------------------------------

void	CAtomBillboardingBatchPolicy::ClearBuffers(SAtomRenderContext &ctx)
{
	AZ_UNUSED(ctx);
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::EmitLights(const SDrawCallDesc &toEmit, SAtomDrawOutputs &output)
{
	if (!PK_VERIFY(output.m_Lights.Reserve(output.m_Lights.Count() + toEmit.m_TotalParticleCount)))
		return false;
	for (u32 iDr = 0; iDr < toEmit.m_DrawRequests.Count(); ++iDr)
	{
		const Drawers::SLight_DrawRequest			*dr = static_cast<const Drawers::SLight_DrawRequest*>(toEmit.m_DrawRequests[iDr]);
		const Drawers::SLight_BillboardingRequest	&br = dr->m_BB;
		const CParticleStreamToRender_MainMemory	*streamToRender = toEmit.m_DrawRequests[iDr]->StreamToRender_MainMemory();
		const u32									pageCount = streamToRender->PageCount();

		for (u32 iPage = 0; iPage < pageCount; ++iPage)
		{
			const CParticlePageToRender_MainMemory	*page = streamToRender->Page(iPage);
			if (page->Culled())
				continue;

			TMemoryView<const CFloat3>	positions = page->StreamForReading<const CFloat3>(br.m_PositionStreamId).ToMemoryView();
			TMemoryView<const CFloat4>	colors = page->StreamForReading<const CFloat4>(br.m_ColorStreamId).ToMemoryView();
			TMemoryView<const float>	ranges = page->StreamForReading<const float>(br.m_RangeStreamId).ToMemoryView();
			TMemoryView<const bool>		enableds = page->StreamForReading<const bool>(br.m_EnabledStreamId).ToMemoryView();

			// emergency abort, in case this material isn't supported.
			if (!PK_VERIFY(!positions.Empty() && !colors.Empty() && !ranges.Empty() && !enableds.Empty()))
				return false;

			PK_ASSERT(positions.Count() == colors.Count());
			PK_ASSERT(positions.Count() == ranges.Count());
			PK_ASSERT(positions.Count() == enableds.Count());

			const u32	particleCount = page->InputParticleCount();
			for (u32 iParticle = 0; iParticle < particleCount; ++iParticle)
			{
				if (enableds[iParticle] == false)
					continue;
				if (!PK_VERIFY(output.m_Lights.PushBack().Valid()))
					return false;
				SAtomDrawOutputs::SLight	&light = output.m_Lights.Last();
				light.m_Position = positions[iParticle];

				// Atom handles the falloff in the shader directly. scale down the color directly with attenuation steepness
				// It's not entirely correct but it's visually close to rendering in PopcornFX Editor
				light.m_Color = colors[iParticle].xyz() * br.m_AttenuationSteepness;

				light.m_AttenuationRadius = ranges[iParticle];
			}
		}
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::EmitDrawCall(SAtomRenderContext &ctx, const SDrawCallDesc &toEmit, SAtomDrawOutputs &output)
{
	AZ_UNUSED(ctx);

	if (m_RendererType == Renderer_Light)
		return EmitLights(toEmit, output);

	SAtomDrawOutputs::SDrawCall		dc;

	for (const auto &pipelineCache : m_PipelineCaches)
	{
		if (!pipelineCache.IsInitialized())
			return true;
	}

	const CParticleBuffers::SViewIndependent	&viewIndependent = GetCurBuffers().m_ViewIndependent;
	const CParticleBuffers::SViewDependent		&viewDependent = GetCurBuffers().m_ViewDependent;
	const CAtomRendererCache					*rendererCache = static_cast<const CAtomRendererCache*>(toEmit.m_RendererCaches.First().Get());

	if (!PK_VERIFY(rendererCache != null))
		return false;

	dc.m_RendererType = m_RendererType;
	// For billboard renderer, we use a draw instance
	if (m_RendererType == Renderer_Billboard)
	{
		if (!PK_VERIFY(m_PipelineCaches.Count() == 1))
			return false;

		// ----------------------------------------------------------------
		// The pipeline cache handles the buffer bindings for the draw call:
		// Add draw instance vertex and index buffers:
		m_PipelineCaches[0].SetVertexInputBuffer(DrawInstanceTexCoords_Vertex, AZ::RHI::StreamBufferView(*m_DrawInstanceVtx, 0, m_DrawInstanceVtxCount * sizeof(CFloat2), sizeof(CFloat2)));
		m_PipelineCaches[0].SetIndexBuffer(AZ::RHI::IndexBufferView(*m_DrawInstanceIdx, 0, m_DrawInstanceIdxCount * sizeof(u16), AZ::RHI::IndexFormat::Uint16));
		m_PipelineCaches[0].SetBillboardingSrgConstantValue(BillboardSrg::RendererFlags_ShaderRead, rendererCache->m_BasicDescription.m_RendererFlags);

		if (m_AtlasDefinition != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = m_AtlasDefinition->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_AtlasSubRectsCount, sizeof(CFloat4)));
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::AtlasSubRects_ShaderRead, buff);
			m_PipelineCaches[0].SetBillboardingSrgConstantValue(BillboardSrg::AtlasSubRectsCount_ShaderRead, m_AtlasSubRectsCount);
		}
		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices] != null)
		{
			// Unsorted indices:
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices]->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount, sizeof(u32)));
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::ParticleIndices_ShaderRead, buff);
		}
		else if (viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices] != null)
		{
			// Sorted indices:
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices]->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount, sizeof(u32)));
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::ParticleIndices_ShaderRead, buff);
		}
		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticlePosition] != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticlePosition]->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount, sizeof(CFloat4)));
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::ParticlePositions_ShaderRead, buff);
		}
		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleRotation] != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleRotation]->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount, sizeof(float)));
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::ParticleRotations_ShaderRead, buff);
		}
		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleAxis0] != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleAxis0]->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount * 3, sizeof(float))); // Cannot use a float3 stride
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::ParticleAxis0_ShaderRead, buff);
		}
		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleAxis1] != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleAxis1]->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount * 3, sizeof(float))); // Cannot use a float3 stride
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::ParticleAxis1_ShaderRead, buff);
		}
		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleSize] != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleSize]->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount, sizeof(float)));
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::ParticleSizes_ShaderRead, buff);
		}
		else if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleSize2] != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_ParticleSize2]->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount, sizeof(CFloat2)));
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::ParticleSizes2_ShaderRead, buff);
		}
		// Additional field buffers:
		AZ::RHI::Ptr<AZ::RHI::Buffer>	textureID = GetCurBuffers().FindAdditionalFieldBuffer(BasicRendererProperties::SID_Atlas_TextureID());
		if (textureID != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = textureID->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount, sizeof(float)));
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::ParticleAtlasID_ShaderRead, buff);
		}
		AZ::RHI::Ptr<AZ::RHI::Buffer>	diffuseColor = GetCurBuffers().FindAdditionalFieldBuffer(BasicRendererProperties::SID_Diffuse_Color());
		if (diffuseColor != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = diffuseColor->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount, sizeof(CFloat4)));
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::ParticleDiffuseColor_ShaderRead, buff);
		}
		AZ::RHI::Ptr<AZ::RHI::Buffer>	emissiveColor = GetCurBuffers().FindAdditionalFieldBuffer(BasicRendererProperties::SID_Emissive_EmissiveColor());
		if (emissiveColor != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = emissiveColor->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount * 3, sizeof(float)));
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::ParticleEmissiveColor_ShaderRead, buff);
		}
		AZ::RHI::Ptr<AZ::RHI::Buffer>	distortionColor = GetCurBuffers().FindAdditionalFieldBuffer(BasicRendererProperties::SID_Distortion_Color());
		if (distortionColor != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = distortionColor->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount, sizeof(CFloat4)));
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::ParticleDistortionColor_ShaderRead, buff);
		}
		AZ::RHI::Ptr<AZ::RHI::Buffer>	alphaRemapCursor = GetCurBuffers().FindAdditionalFieldBuffer(BasicRendererProperties::SID_AlphaRemap_Cursor());
		if (alphaRemapCursor != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = alphaRemapCursor->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount, sizeof(float)));
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::ParticleAlphaCursor_ShaderRead, buff);
		}

		// Draw requests:
		if (viewIndependent.m_DrawRequests != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = viewIndependent.m_DrawRequests->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_DrawRequestCount, sizeof(Drawers::SBillboardDrawRequest)));
			m_PipelineCaches[0].SetBillboardingSrgBuffer(BillboardSrg::DrawRequest_ShaderRead, buff);
		}

		// Fill the draw call:
		m_PipelineCaches[0].ConfigureDrawCall(dc);
		// ----------------------------------------------------------------

		// Draw call bounding box:
		dc.m_BoundingBox = toEmit.m_BBox;

		// Draw call description:
		dc.m_DrawIndexed.m_indexCount = m_DrawInstanceIdxCount;
		dc.m_DrawIndexed.m_indexOffset = 0;
		dc.m_DrawIndexed.m_instanceCount = m_ParticleCount;
		dc.m_DrawIndexed.m_instanceOffset = 0;
		dc.m_DrawIndexed.m_vertexOffset = 0;

		// Draw instance indices and tex-coords:
		return PK_VERIFY(output.m_DrawCalls.PushBack(dc).Valid());
	}
	else if (m_RendererType == Renderer_Ribbon)
	{
		if (!PK_VERIFY(m_PipelineCaches.Count() == 1))
			return false;

		// Position:
		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Position] != null)
			m_PipelineCaches[0].SetVertexInputBuffer(Position_Vertex, AZ::RHI::StreamBufferView(*viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Position], 0, m_VertexCount * sizeof(CFloat4), sizeof(CFloat4)));
		else if (viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Position] != null)
			m_PipelineCaches[0].SetVertexInputBuffer(Position_Vertex, AZ::RHI::StreamBufferView(*viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Position], 0, m_VertexCount * sizeof(CFloat4), sizeof(CFloat4)));
		// Normal:
		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Normal] != null)
			m_PipelineCaches[0].SetVertexInputBuffer(Normal_Vertex, AZ::RHI::StreamBufferView(*viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Normal], 0, m_VertexCount * sizeof(CFloat4), sizeof(CFloat4)));
		else if (viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Normal] != null)
			m_PipelineCaches[0].SetVertexInputBuffer(Normal_Vertex, AZ::RHI::StreamBufferView(*viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Normal], 0, m_VertexCount * sizeof(CFloat4), sizeof(CFloat4)));
		// Tangent:
		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Tangent] != null)
			m_PipelineCaches[0].SetVertexInputBuffer(Tangent_Vertex, AZ::RHI::StreamBufferView(*viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Tangent], 0, m_VertexCount * sizeof(CFloat4), sizeof(CFloat4)));
		else if (viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Tangent] != null)
			m_PipelineCaches[0].SetVertexInputBuffer(Tangent_Vertex, AZ::RHI::StreamBufferView(*viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Tangent], 0, m_VertexCount * sizeof(CFloat4), sizeof(CFloat4)));

		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UV0] != null)
			m_PipelineCaches[0].SetVertexInputBuffer(TexCoord0_Vertex, AZ::RHI::StreamBufferView(*viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UV0], 0, m_VertexCount * sizeof(CFloat2), sizeof(CFloat2)));

		// Linear atlas blending:
		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UV1] != null)
			m_PipelineCaches[0].SetVertexInputBuffer(TexCoord1_Vertex, AZ::RHI::StreamBufferView(*viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UV1], 0, m_VertexCount * sizeof(CFloat2), sizeof(CFloat2)));
		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_AtlasId] != null)
			m_PipelineCaches[0].SetVertexInputBuffer(TexFrameLerp_Vertex, AZ::RHI::StreamBufferView(*viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_AtlasId], 0, m_VertexCount * sizeof(float), sizeof(float)));

		// Correct deformation:
		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UVRemap] != null)
			m_PipelineCaches[0].SetVertexInputBuffer(UVRemap_Vertex, AZ::RHI::StreamBufferView(*viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UVRemap], 0, m_VertexCount * sizeof(CFloat4), sizeof(CFloat4)));
		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UVFactors] != null)
			m_PipelineCaches[0].SetVertexInputBuffer(UVFactors_Vertex, AZ::RHI::StreamBufferView(*viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_UVFactors], 0, m_VertexCount * sizeof(CFloat2), sizeof(CFloat2)));
		else if (viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_UVFactors] != null)
			m_PipelineCaches[0].SetVertexInputBuffer(UVFactors_Vertex, AZ::RHI::StreamBufferView(*viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_UVFactors], 0, m_VertexCount * sizeof(CFloat2), sizeof(CFloat2)));

		const AZ::u32				idxSize = m_UseLargeIndices ? sizeof(AZ::u32) : sizeof(AZ::u16);
		const AZ::RHI::IndexFormat	idxFormat = m_UseLargeIndices ? AZ::RHI::IndexFormat::Uint32 : AZ::RHI::IndexFormat::Uint16;
		if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices] != null)
		{
			// Unsorted indices:
			m_PipelineCaches[0].SetIndexBuffer(AZ::RHI::IndexBufferView(*viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices], 0, m_IndexCount * idxSize, idxFormat));
		}
		else if (viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices] != null)
		{
			// Sorted indices:
			m_PipelineCaches[0].SetIndexBuffer(AZ::RHI::IndexBufferView(*viewDependent.m_GenBuffers[CParticleBuffers::GenBuffer_Indices], 0, m_IndexCount * idxSize, idxFormat));
		}

		m_PipelineCaches[0].SetRibbonSrgConstantValue(RibbonSrg::RendererFlags_ShaderRead, rendererCache->m_BasicDescription.m_RendererFlags);
		m_PipelineCaches[0].SetRibbonSrgConstantValue(RibbonSrg::ParticleCount_ShaderRead, m_ParticleCount);

		// Additional field buffers:
		AZ::RHI::Ptr<AZ::RHI::Buffer>	diffuseColor = GetCurBuffers().FindAdditionalFieldBuffer(BasicRendererProperties::SID_Diffuse_Color());
		if (diffuseColor != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = diffuseColor->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount, sizeof(CFloat4)));
			m_PipelineCaches[0].SetRibbonSrgBuffer(RibbonSrg::ParticleDiffuseColor_ShaderRead, buff);
		}
		AZ::RHI::Ptr<AZ::RHI::Buffer>	emissiveColor = GetCurBuffers().FindAdditionalFieldBuffer(BasicRendererProperties::SID_Emissive_EmissiveColor());
		if (emissiveColor != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = emissiveColor->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount * 3, sizeof(float)));
			m_PipelineCaches[0].SetRibbonSrgBuffer(RibbonSrg::ParticleEmissiveColor_ShaderRead, buff);
		}
		AZ::RHI::Ptr<AZ::RHI::Buffer>	distortionColor = GetCurBuffers().FindAdditionalFieldBuffer(BasicRendererProperties::SID_Distortion_Color());
		if (distortionColor != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = distortionColor->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount, sizeof(CFloat4)));
			m_PipelineCaches[0].SetRibbonSrgBuffer(RibbonSrg::ParticleDistortionColor_ShaderRead, buff);
		}
		AZ::RHI::Ptr<AZ::RHI::Buffer>	alphaRemapCursor = GetCurBuffers().FindAdditionalFieldBuffer(BasicRendererProperties::SID_AlphaRemap_Cursor());
		if (alphaRemapCursor != null)
		{
			AZ::RHI::Ptr<AZ::RHI::BufferView> buff = alphaRemapCursor->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(0, m_ParticleCount, sizeof(float)));
			m_PipelineCaches[0].SetRibbonSrgBuffer(RibbonSrg::ParticleAlphaCursor_ShaderRead, buff);
		}

		// Fill the draw call:
		m_PipelineCaches[0].ConfigureDrawCall(dc);
		// ----------------------------------------------------------------

		// Draw call bounding box:
		dc.m_BoundingBox = toEmit.m_BBox;

		// Draw call description:
		dc.m_DrawIndexed .m_indexCount = m_IndexCount;
		dc.m_DrawIndexed.m_indexOffset = 0;
		dc.m_DrawIndexed.m_instanceCount = 1;
		dc.m_DrawIndexed.m_instanceOffset = 0;
		dc.m_DrawIndexed.m_vertexOffset = 0;

		// Draw instance indices and tex-coords:
		return PK_VERIFY(output.m_DrawCalls.PushBack(dc).Valid());
	}
	else if (m_RendererType == Renderer_Mesh)
	{
		if (!PK_VERIFY(m_GeometryCache != null))
			return false;

		u32		particleOffset = 0;
		const bool	meshAtlas = rendererCache->m_BasicDescription.m_UseMeshAtlas;

		for (u32 i = 0; i < m_GeometryCache->m_PerGeometryViews.Count(); i++)
		{
			const u32 particleCount = meshAtlas ? m_PerMeshParticleCount[i] : m_ParticleCount;
			if (particleCount <= 0)
				continue;
			const CGeometryCache::GPUBufferViews &views = m_GeometryCache->m_PerGeometryViews[i];

			// Setup index and vertex buffers
			// TODO: For mesh and bb draw instanced, vertex buffers are constants. This could be moved
			// in CAtomPipelineCache::InitFromRendererCacheIFN and done once
			// (clear must removed of m_PipelineCache.ConfigureDrawCall(dc))
			// At least finding the geometry cache can be done once
			m_PipelineCaches[i].SetIndexBuffer(views.m_IndexBuffer);

			if (views.m_PositionBuffer.GetBuffer() != null)
				m_PipelineCaches[i].SetVertexInputBuffer(Position_Vertex, views.m_PositionBuffer);
			if (views.m_UVBuffer.GetBuffer() != null)
				m_PipelineCaches[i].SetVertexInputBuffer(TexCoord0_Vertex, views.m_UVBuffer);
			if (rendererCache->m_BasicDescription.HasOneRendererFlags(RendererFlags::Has_Lighting))
			{
				if (views.m_NormalBuffer.GetBuffer() != null)
					m_PipelineCaches[i].SetVertexInputBuffer(Normal_Vertex, views.m_NormalBuffer);
				if (views.m_TangentBuffer.GetBuffer() != null)
					m_PipelineCaches[i].SetVertexInputBuffer(Tangent_Vertex, views.m_TangentBuffer);
			}

			// Constant data: setup renderer flags
			m_PipelineCaches[i].SetMeshSrgConstantValue(MeshSrg::RendererFlags_ShaderRead, rendererCache->m_BasicDescription.m_RendererFlags);

			// Non constant data: matrices
			if (viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Matrices] != null)
			{
				AZ::RHI::Ptr<AZ::RHI::BufferView> buff = viewIndependent.m_GenBuffers[CParticleBuffers::GenBuffer_Matrices]->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(particleOffset, particleCount, sizeof(CFloat4x4)));
				m_PipelineCaches[i].SetMeshSrgBuffer(MeshSrg::Matrices_ShaderRead, buff);
			}

			// Non constant data: additionnal inputs
			AZ::RHI::Ptr<AZ::RHI::Buffer>	diffuseColor = GetCurBuffers().FindAdditionalFieldBuffer(BasicRendererProperties::SID_Diffuse_Color());
			if (diffuseColor != null)
			{
				AZ::RHI::Ptr<AZ::RHI::BufferView> buff = diffuseColor->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(particleOffset, particleCount, sizeof(CFloat4)));
				m_PipelineCaches[i].SetMeshSrgBuffer(MeshSrg::ParticleDiffuseColor_ShaderRead, buff);
			}
			AZ::RHI::Ptr<AZ::RHI::Buffer>	emissiveColor = GetCurBuffers().FindAdditionalFieldBuffer(BasicRendererProperties::SID_Emissive_EmissiveColor());
			if (emissiveColor != null)
			{
				AZ::RHI::Ptr<AZ::RHI::BufferView> buff = emissiveColor->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(particleOffset * 3, particleCount * 3, sizeof(float)));
				m_PipelineCaches[i].SetMeshSrgBuffer(MeshSrg::ParticleEmissiveColor_ShaderRead, buff);
			}
			AZ::RHI::Ptr<AZ::RHI::Buffer>	alphaRemapCursor = GetCurBuffers().FindAdditionalFieldBuffer(BasicRendererProperties::SID_AlphaRemap_Cursor());
			if (alphaRemapCursor != null)
			{
				AZ::RHI::Ptr<AZ::RHI::BufferView> buff = alphaRemapCursor->GetBufferView(AZ::RHI::BufferViewDescriptor::CreateStructured(particleOffset, particleCount, sizeof(float)));
				m_PipelineCaches[i].SetMeshSrgBuffer(MeshSrg::ParticleAlphaCursor_ShaderRead, buff);
			}

			m_PipelineCaches[i].ConfigureDrawCall(dc);

			// Draw call bounding box:
			dc.m_BoundingBox = toEmit.m_BBox;

			// Draw call description:
			dc.m_DrawIndexed.m_indexCount = views.m_IndexCount;
			dc.m_DrawIndexed.m_indexOffset = 0;
			dc.m_DrawIndexed.m_instanceCount = particleCount;
			dc.m_DrawIndexed.m_instanceOffset = 0;
			dc.m_DrawIndexed.m_vertexOffset = 0;

			// Draw instance indices and tex-coords:
			if (!PK_VERIFY(output.m_DrawCalls.PushBack(dc).Valid()))
				return false;
			particleOffset += meshAtlas ? particleCount : 0;
		}
		return true;
	}
	// Unrecognized renderer type
	return false;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::_AllocViewIndependentBuffers(const SGeneratedInputs &inputs)
{
	PK_SCOPEDPROFILE();
	bool	success = true;
	// -----------------------------------------
	// View independent inputs:
	// -----------------------------------------
	CParticleBuffers::SViewIndependent	&viewIndependent = GetCurBuffers().m_ViewIndependent;
	u32									inputsToGenerate = inputs.m_GeneratedInputs;

	while (inputsToGenerate != 0)
	{
		CParticleBuffers::EGenBuffer	currentGenBuffer = static_cast<CParticleBuffers::EGenBuffer>(IntegerLog2(inputsToGenerate));

		PK_ASSERT(currentGenBuffer < CParticleBuffers::EGenBuffer::__GenBuffer_Count);

		AZ::RHI::Ptr<AZ::RHI::Buffer>	&currentBufferPtr = viewIndependent.m_GenBuffers[currentGenBuffer];
		const u32						stride = CParticleBuffers::kBufferStrides[currentGenBuffer];

		if (currentGenBuffer == CParticleBuffers::EGenBuffer::GenBuffer_Indices)
		{
			// Indices:
			const AZ::u64				idxSize = m_UseLargeIndices ? sizeof(AZ::u32) : sizeof(AZ::u16);
			currentBufferPtr = m_RenderContext->m_RenderManager->ResizeOrCreateBufferIFN(currentBufferPtr, m_IndexCount * idxSize);
		}
		else if (currentGenBuffer >= CParticleBuffers::EGenBuffer::GenBuffer_Matrices) // Per particle
			currentBufferPtr = m_RenderContext->m_RenderManager->ResizeOrCreateBufferIFN(currentBufferPtr, m_ParticleCount * stride);
		else // Per vertex
			currentBufferPtr = m_RenderContext->m_RenderManager->ResizeOrCreateBufferIFN(currentBufferPtr, m_VertexCount * stride);

		success &= currentBufferPtr != null;
		inputsToGenerate ^= (1 << currentGenBuffer);
	}
	// Draw request input:
	if (m_RendererType == Renderer_Billboard)
	{
		AZ::RHI::Ptr<AZ::RHI::Buffer>	&currentBufferPtr = GetCurBuffers().m_ViewIndependent.m_DrawRequests;
		currentBufferPtr = m_RenderContext->m_RenderManager->ResizeOrCreateBufferIFN(currentBufferPtr, m_DrawRequestCount * sizeof(Drawers::SBillboardDrawRequest), 0x100);
		success &= currentBufferPtr != null;
	}
	else if (m_RendererType != Renderer_Ribbon && m_RendererType != Renderer_Mesh)
	{
		PK_ASSERT_NOT_REACHED_MESSAGE("Renderer type is not handled");
	}
	return success;
}

//----------------------------------------------------------------------------

bool	CAtomBillboardingBatchPolicy::_AllocViewDependentBuffers(const SGeneratedInputs &inputs)
{
	PK_SCOPEDPROFILE();
	bool	success = true;
	// -----------------------------------------
	// View dependent inputs:
	// -----------------------------------------
	// Just select the first view for now:
	const u32	viewCount = PKMin(inputs.m_PerViewGeneratedInputs.Count(), 1);

	for (u32 i = 0; i < viewCount; ++i)
	{
		const SGeneratedInputs::SViewGeneratedInputs	&curView = inputs.m_PerViewGeneratedInputs[i];

		u32	inputsToGenerate = curView.m_GeneratedInputs;

		while (inputsToGenerate != 0)
		{
			CParticleBuffers::EGenBuffer	currentGenBuffer = static_cast<CParticleBuffers::EGenBuffer>(IntegerLog2(inputsToGenerate));

			PK_ASSERT(currentGenBuffer < CParticleBuffers::EGenBuffer::__GenBuffer_Count);

			AZ::RHI::Ptr<AZ::RHI::Buffer>	&currentBufferPtr = GetCurBuffers().m_ViewDependent.m_GenBuffers[currentGenBuffer];
			const u32						stride = CParticleBuffers::kBufferStrides[currentGenBuffer];

			if (currentGenBuffer == CParticleBuffers::EGenBuffer::GenBuffer_Indices)
			{
				// Indices:
				const AZ::u64				idxSize = m_UseLargeIndices ? sizeof(AZ::u32) : sizeof(AZ::u16);
				currentBufferPtr = m_RenderContext->m_RenderManager->ResizeOrCreateBufferIFN(currentBufferPtr, m_IndexCount * idxSize);
			}
			else if (currentGenBuffer >= CParticleBuffers::EGenBuffer::GenBuffer_Matrices) // Per particle
				currentBufferPtr = m_RenderContext->m_RenderManager->ResizeOrCreateBufferIFN(currentBufferPtr, m_ParticleCount * stride);
			else // Per vertex
				currentBufferPtr = m_RenderContext->m_RenderManager->ResizeOrCreateBufferIFN(currentBufferPtr, m_VertexCount * stride);

			success &= currentBufferPtr != null;
			inputsToGenerate ^= (1 << currentGenBuffer);
		}
	}
	return success;
}

//----------------------------------------------------------------------------
}

#endif //O3DE_USE_PK
