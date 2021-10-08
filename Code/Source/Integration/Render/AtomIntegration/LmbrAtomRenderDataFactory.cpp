//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include "PopcornFX_precompiled.h"
#include "LmbrAtomRenderDataFactory.h"

#if defined(LMBR_USE_PK)

#include "Integration/Render/LmbrRendererCache.h"
#include "LmbrAtomBillboardingBatchPolicy.h"

#include <pk_particles/include/ps_effect.h>
#include <pk_render_helpers/include/batches/rh_ribbon_batch.h>
#include <pk_render_helpers/include/batches/rh_billboard_batch.h>
#include <pk_render_helpers/include/batches/rh_mesh_batch.h>
#include <pk_render_helpers/include/batches/rh_light_batch.h>
#include <pk_render_helpers/include/batches/rh_decal_batch.h>

#include <Atom/RPI.Public/RPIUtils.h>
#include <Atom/RPI.Public/RPISystemInterface.h>
#include <Atom/RPI.Public/Scene.h>
#include <Atom/RHI.Reflect/InputStreamLayoutBuilder.h>

__LMBRPK_BEGIN
//----------------------------------------------------------------------------

PRendererCacheBase	CLmbrAtomRenderDataFactory::UpdateThread_CreateRendererCache(const PRendererDataBase &renderer, const CParticleDescriptor *particleDesc)
{
	// This is called when the effect is preloaded, or when first instantiated
	// It is required that a renderer cache is created per renderer.

	// Returned value is PRendererCacheBase (strong ref), you can leave PopcornFX handle the cache's lifetime (no need to free it)
	// It will be destroyed when effect is unloaded.

	// We create the renderer cache here:
	PLmbrRendererCache	rendererCache = PK_NEW(CLmbrRendererCache);
	if (!PK_VERIFY(rendererCache != null))
	{
		AZ_Error("PopcornFX", false, "Could not allocate the renderer cache");
		return null;
	}
		
	rendererCache->m_CacheFactory = this;
	rendererCache->InitFromRenderer(*renderer, m_PackPath, particleDesc->ParentEffect()->FilePath());
	if (renderer->m_RendererType != Renderer_Light)
	{
		if (!PK_VERIFY(m_RendererLoader.AddMaterialToCreate(rendererCache->m_BasicDescription.m_MaterialKey, rendererCache->m_BasicDescription.m_PipelineStateKey)))
			return null;
		if (rendererCache->m_RendererType == Renderer_Mesh)
		{
			if (!PK_VERIFY(m_RendererLoader.AddGeometryToLoad(rendererCache->m_BasicDescription.m_MeshPath.ToString(), rendererCache)))
				return null;
		}
	}
	return rendererCache;
}

//----------------------------------------------------------------------------

void	CLmbrAtomRenderDataFactory::UpdateThread_CollectedForRendering(const PRendererCacheBase &rendererCache)
{
	(void)rendererCache;
	// Called each frame if the renderer cache has been collected to get rendered
}

//----------------------------------------------------------------------------

CLmbrAtomRenderDataFactory::CBillboardingBatchInterface		*CLmbrAtomRenderDataFactory::CreateBillboardingBatch(ERendererClass rendererType, const PRendererCacheBase &rendererCache, bool gpuStorage)
{
	(void)rendererCache;

	// Default billboarding batch implementations:
	typedef	TBillboardBatch<CLmbrAtomParticleBatchTypes, CLmbrAtomBillboardingBatchPolicy>	CBillboardBillboardingBatch;
	typedef	TRibbonBatch<CLmbrAtomParticleBatchTypes, CLmbrAtomBillboardingBatchPolicy>		CRibbonBillboardingBatch;
	typedef	TMeshBatch<CLmbrAtomParticleBatchTypes, CLmbrAtomBillboardingBatchPolicy>		CMeshBillboardingBatch;
	typedef	TLightBatch<CLmbrAtomParticleBatchTypes, CLmbrAtomBillboardingBatchPolicy>		CLightBillboardingBatch;
//	typedef	TDecalBatch<CLmbrAtomParticleBatchTypes, CLmbrAtomBillboardingBatchPolicy>		CDecalBillboardingBatch;

	if (!gpuStorage)
	{
		switch (rendererType)
		{
		case	Renderer_Billboard:
		{
			CBillboardBillboardingBatch* batch = PK_NEW(CBillboardBillboardingBatch);
			if (PK_VERIFY(batch != null))
				batch->SetBillboardingLocation(Drawers::BillboardingLocation_VertexShader);
			return batch;
		}
		case	Renderer_Ribbon:
		{
			CRibbonBillboardingBatch* batch = PK_NEW(CRibbonBillboardingBatch);
			return batch;
		}
		case	Renderer_Mesh:
			return PK_NEW(CMeshBillboardingBatch);
		case	Renderer_Light:
			return PK_NEW(CLightBillboardingBatch);
//		case	Renderer_Decal:
//			return PK_NEW(CDecalBillboardingBatch);
		default:
			return null;
		}
	}
	else
	{
		PK_ASSERT_NOT_IMPLEMENTED_MESSAGE("PopcornFX GPU simulation is not implemented in Lumberyard yet...");
	}
	return null;
}

//----------------------------------------------------------------------------

PMaterialCache		CLmbrAtomRenderDataFactory::FindMaterial(const SMaterialCacheKey &key) const
{
	return m_RendererLoader.FindMaterial(key);
}

//----------------------------------------------------------------------------

PPipelineStateCache	CLmbrAtomRenderDataFactory::FindPipelineState(const SPipelineStateCacheKey &key) const
{
	return m_RendererLoader.FindPipelineState(key);
}

//----------------------------------------------------------------------------

PGeometryCache		CLmbrAtomRenderDataFactory::FindGeometryCache(const CString &path) const
{
	return m_RendererLoader.FindGeometryCache(path);
}

//----------------------------------------------------------------------------
__LMBRPK_END

#endif //LMBR_USE_PK
