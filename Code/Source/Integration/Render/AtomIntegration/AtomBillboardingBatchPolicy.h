//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#if defined(O3DE_USE_PK)

#include "AtomFrameCollectorTypes.h"
#include "AtomPipelineCache.h"
#include "AtomRendererCache.h"
#include <pk_kernel/include/kr_memoryviews.h>
#include <Atom/RHI/Buffer.h>

namespace PopcornFX {
//----------------------------------------------------------------------------

class	CParticleBuffers
{
public:
	CParticleBuffers() { }
	~CParticleBuffers()
	{
		Reset();
	}

	void								ResizeOrCreateAdditionalFieldBufferIFN(CStringId additionalFieldName, CRenderManager *renderManager, AZ::u64 bufferSize);
	void								UnmapAllAdditionalFieldBuffers(CRenderManager *renderManager);
	AZ::RHI::Ptr<AZ::RHI::Buffer>		FindAdditionalFieldBuffer(CStringId additionalFieldName);

	void								Reset();
	void								UnmapAll(CRenderManager *renderManager);

	enum	EGenBuffer
	{
		// Per particle for GPU billbording / per vertex for CPU billboarding:
		GenBuffer_Indices,

		// Per vertex:
		GenBuffer_Position,
		GenBuffer_Normal,
		GenBuffer_Tangent,
		GenBuffer_UV0,
		GenBuffer_UV1,
		GenBuffer_AtlasId,
		GenBuffer_UVRemap,
		GenBuffer_UVFactors,

		// Per particle:
		GenBuffer_Matrices,
		GenBuffer_ParticlePosition,
		GenBuffer_ParticleSize,
		GenBuffer_ParticleSize2,
		GenBuffer_ParticleRotation,
		GenBuffer_ParticleAxis0,
		GenBuffer_ParticleAxis1,
		GenBuffer_ParticlePosition0,
		GenBuffer_ParticlePosition1,
		GenBuffer_ParticlePosition2,

		__GenBuffer_Count
	};

	// Element sizes:
	static const u32	kBufferStrides[];

	PK_STATIC_ASSERT(TIntegerTemplateTools::Log2<Drawers::GenInput_ParticlePosition2>::Value + 1 == __GenBuffer_Count);

	struct	SViewIndependent
	{
		AZ::RHI::Ptr<AZ::RHI::Buffer>	m_GenBuffers[__GenBuffer_Count];
		// For GPU billboarding:
		AZ::RHI::Ptr<AZ::RHI::Buffer>	m_DrawRequests;

		SViewIndependent() { Mem::Clear(*this); }
	};

	struct	SViewDependent
	{
		AZ::RHI::Ptr<AZ::RHI::Buffer>	m_GenBuffers[__GenBuffer_Count];

		SViewDependent() { Mem::Clear(*this); }
	};

	// --------------------------------
	// For ribbons:
	// --------------------------------
	SViewIndependent	m_ViewIndependent;
	// We only handle one camera for the moment:
	SViewDependent		m_ViewDependent;
	// --------------------------------

private:
	struct	SAdditionalField
	{
		CStringId						m_Name;
		AZ::RHI::Ptr<AZ::RHI::Buffer>	m_Buffer;

		SAdditionalField() { }
		SAdditionalField(const CStringId &name)
			: m_Name(name)
			, m_Buffer(null)
		{
		}

		bool	operator==(const SAdditionalField &oth) const { return m_Name == oth.m_Name; }
		bool	operator!=(const SAdditionalField &oth) const { return m_Name != oth.m_Name; }
	};

	PopcornFX::TArray<SAdditionalField>					m_AdditionalFields;
};

class	CAtomBillboardingBatchPolicy
{
public:
	CAtomBillboardingBatchPolicy();
	~CAtomBillboardingBatchPolicy();

	// Called per CAtomBillboardingBatchPolicy with the compatible draw request and associated renderer cache. Allows batches "cull"
	static bool		CanRender(const Drawers::SBillboard_DrawRequest *request, const PRendererCacheBase rendererCache, SAtomRenderContext &ctx) { AZ_UNUSED(request); AZ_UNUSED(rendererCache); AZ_UNUSED(ctx); return true; }
	static bool		CanRender(const Drawers::SRibbon_DrawRequest *request, const PRendererCacheBase rendererCache, SAtomRenderContext &ctx) { AZ_UNUSED(request); AZ_UNUSED(rendererCache); AZ_UNUSED(ctx); return true; }
	static bool		CanRender(const Drawers::SMesh_DrawRequest *request, const PRendererCacheBase rendererCache, SAtomRenderContext &ctx) { AZ_UNUSED(request); AZ_UNUSED(rendererCache); AZ_UNUSED(ctx); return true; }
	static bool		CanRender(const Drawers::SLight_DrawRequest *request, const PRendererCacheBase rendererCache, SAtomRenderContext &ctx) { AZ_UNUSED(request); AZ_UNUSED(rendererCache); AZ_UNUSED(ctx); return true; }
	static bool		CanRender(const Drawers::SDecal_DrawRequest *request, const PRendererCacheBase rendererCache, SAtomRenderContext &ctx) { AZ_UNUSED(request); AZ_UNUSED(rendererCache); AZ_UNUSED(ctx); return true; }
	static bool		CanRender(const Drawers::SSound_DrawRequest *request, const PRendererCacheBase rendererCache, SAtomRenderContext &ctx) { AZ_UNUSED(request); AZ_UNUSED(rendererCache); AZ_UNUSED(ctx); return false; } // Not implemented in this null policy

	// Mandatory part: Determines whether input renderer caches (and their renderer) can be batched for billboarding
	// Right now, it means that they are drawcall compatible -> Return true when those two caches are compatible (same material/uniforms/..)
	bool			AreBillboardingBatchable(const PCRendererCacheBase &firstCache, const PCRendererCacheBase &secondCache) const;

	// Whether this billboarding batch policy can be re-used by incompatible renderer caches (see CHM documentation for more detail)
	// For example: light renderer billboarding batch policies could return true
	static bool		IsStateless() { return false; }

	// Return false when this billboarding batch policy should be destroyed (ie. nothing was drawn after 10 ticks)
	bool			Tick(SAtomRenderContext &ctx, const TMemoryView<SAtomSceneView> &views);

	bool			AllocBuffers(SAtomRenderContext &ctx, const SBuffersToAlloc &allocBuffers, const TMemoryView<SAtomSceneView> &views, ERendererClass rendererType);

	bool			MapBuffers(SAtomRenderContext &ctx, const TMemoryView<SAtomSceneView> &views, SBillboardBatchJobs *billboardBatch, const SGeneratedInputs &toMap);
	bool			MapBuffers(SAtomRenderContext &ctx, const TMemoryView<SAtomSceneView> &views, SRibbonBatchJobs *billboardBatch, const SGeneratedInputs &toMap);
	bool			MapBuffers(SAtomRenderContext &ctx, const TMemoryView<SAtomSceneView> &views, SGPUBillboardBatchJobs *billboardBatch, const SGeneratedInputs &toMap);
	bool			MapBuffers(SAtomRenderContext &ctx, const TMemoryView<SAtomSceneView> &views, SGPURibbonBatchJobs *billboardBatch, const SGeneratedInputs &toMap);
	bool			MapBuffers(SAtomRenderContext &ctx, const TMemoryView<SAtomSceneView> &views, SMeshBatchJobs *billboardBatch, const SGeneratedInputs &toMap);
	bool			MapBuffers(SAtomRenderContext &ctx, const TMemoryView<SAtomSceneView> &views, SDecalBatchJobs *billboardBatch, const SGeneratedInputs &toMap);

	// Called after main CPU billboarding tasks have been launched. Can be used to launch custom tasks
	bool			LaunchCustomTasks(SAtomRenderContext &ctx, const TMemoryView<const Drawers::SBillboard_DrawRequest * const> drawRequests, Drawers::CBillboard_CPU *billboardBatch);
	bool			LaunchCustomTasks(SAtomRenderContext &ctx, const TMemoryView<const Drawers::SRibbon_DrawRequest * const> drawRequests, Drawers::CRibbon_CPU *ribbonBatch);
	bool			LaunchCustomTasks(SAtomRenderContext &ctx, const TMemoryView<const Drawers::SRibbon_DrawRequest * const> drawRequests, Drawers::CCopyStream_CPU *ribbonBatch);
	bool			LaunchCustomTasks(SAtomRenderContext &ctx, const TMemoryView<const Drawers::SBillboard_DrawRequest * const> drawRequests, Drawers::CCopyStream_CPU *geomBillboardBatch) { AZ_UNUSED(ctx); AZ_UNUSED(drawRequests); AZ_UNUSED(geomBillboardBatch); return true; }
	bool			LaunchCustomTasks(SAtomRenderContext &ctx, const TMemoryView<const Drawers::SMesh_DrawRequest * const> drawRequests, Drawers::CMesh_CPU *meshBatch) { AZ_UNUSED(ctx); AZ_UNUSED(drawRequests); AZ_UNUSED(meshBatch); return true; }
	bool			LaunchCustomTasks(SAtomRenderContext &ctx, const TMemoryView<const Drawers::SLight_DrawRequest * const> drawRequests, Drawers::CBillboard_CPU *batch) { AZ_UNUSED(ctx); AZ_UNUSED(drawRequests); AZ_UNUSED(batch); return true; }
	bool			LaunchCustomTasks(SAtomRenderContext &ctx, const TMemoryView<const Drawers::SSound_DrawRequest * const> drawRequests, Drawers::CBillboard_CPU *batch) { AZ_UNUSED(ctx); AZ_UNUSED(drawRequests); AZ_UNUSED(batch); return false; } // Not implemented in this null policy
	bool			LaunchCustomTasks(SAtomRenderContext &ctx, const TMemoryView<const Drawers::SDecal_DrawRequest * const> drawRequests, Drawers::CDecal_CPU *decalBatch);

	// Generic wait
	bool			WaitForCustomTasks(SAtomRenderContext &ctx);

	// Called once all tasks are done, before emitting draw calls
	bool			UnmapBuffers(SAtomRenderContext &ctx);
	// Called once per new frame
	void			ClearBuffers(SAtomRenderContext &ctx);

	bool			EmitLights(const SDrawCallDesc &toEmit, SAtomDrawOutputs &output);
	bool			EmitDrawCall(SAtomRenderContext &ctx, const SDrawCallDesc &toEmit, SAtomDrawOutputs &output);

private:
	bool			_AllocViewIndependentBuffers(const SGeneratedInputs &inputs);
	bool			_AllocViewDependentBuffers(const SGeneratedInputs &inputs);

	// Store the buffers needed to render the particles here...
	TMemoryView<const u32>	m_PerMeshParticleCount;
	u32						m_VertexCount;
	u32						m_ParticleCount;
	u32						m_DrawRequestCount;
	u32						m_IndexCount;
	bool					m_UseLargeIndices;
	ERendererClass			m_RendererType;
	u32						m_UnusedCounter = 0;
	SAtomRenderContext	*m_RenderContext = null;
	PGeometryCache			m_GeometryCache;

	bool					m_Initialized = false;

	// Job used to copy additional fields once per particle (used for lights and decals):
	Drawers::CCopyStream_Exec_AdditionalField	m_CopyAdditionalFieldsTask;
	Drawers::CCopyStream_CPU					m_CopyTasks;

	// Additional fields mapped buffers:
	PopcornFX::TArray<Drawers::SCopyFieldDesc>			m_MappedAdditionalShaderInputs;
	PopcornFX::TArray<Drawers::SCopyFieldDescPerMesh>	m_MappedAdditionalShaderInputsPerMesh;

	TMemoryView<SAtomSceneView>				m_Views;

	// RHI buffers:
	u32											m_DrawInstanceVtxCount = 0; // 4 for quads, 6 for capsules
	u32											m_DrawInstanceIdxCount = 0; // 6 for quads, 12 for capsules
	AZ::RHI::Ptr<AZ::RHI::Buffer>				m_DrawInstanceVtx = null;
	AZ::RHI::Ptr<AZ::RHI::Buffer>				m_DrawInstanceIdx = null;

	// Atlas definition buffer:
	AZ::RHI::Ptr<AZ::RHI::Buffer>				m_AtlasDefinition = null;
	u32											m_AtlasSubRectsCount = 0;

	// Multi-buffered GPU buffers:
#define MULTI_BUFFERED_COUNT	1
	u32											m_FrameIdx;
	CParticleBuffers							m_Buffers[MULTI_BUFFERED_COUNT];
	CParticleBuffers &GetCurBuffers() { return m_Buffers[m_FrameIdx % MULTI_BUFFERED_COUNT]; }

	TArray<CAtomPipelineCache>				m_PipelineCaches; // We can have several if mesh atlas

	struct	SHandledAdditionalFields
	{
		CStringId						m_Name;
		EBaseTypeID						m_Type = __MaxBaseTypes;

		SHandledAdditionalFields(	CStringId name,
									EBaseTypeID type)
			: m_Name(name)
			, m_Type(type)
		{
		}

		bool	operator==(const SHandledAdditionalFields &oth) const
		{
			return	m_Name == oth.m_Name &&
					m_Type == oth.m_Type;
		}
	};
	TSemiDynamicArray<SHandledAdditionalFields, 6>	m_HandledAdditionalFields;
};

//----------------------------------------------------------------------------
}

#endif //O3DE_USE_PK
