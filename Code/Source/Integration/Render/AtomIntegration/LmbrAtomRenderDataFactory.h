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

#include "LmbrAtomFrameCollectorTypes.h"
#include "Integration/Render/LmbrRendererCache.h"
#include "Integration/Preloader/PopcornFXRendererLoader.h"
#include <Atom/Utils/AssetCollectionAsyncLoader.h>
#include <pk_render_helpers/include/frame_collector/rh_particle_render_data_factory.h>
#include <pk_kernel/include/kr_threads_basics.h>

__LMBRPK_BEGIN
//----------------------------------------------------------------------------

class	CRenderManager;

class	CLmbrAtomRenderDataFactory : public TParticleRenderDataFactory<CLmbrAtomParticleBatchTypes>
{
public:
	CLmbrAtomRenderDataFactory() { }
	virtual ~CLmbrAtomRenderDataFactory() { }

	virtual PRendererCacheBase			UpdateThread_CreateRendererCache(const PRendererDataBase &renderer, const CParticleDescriptor *particleDesc) override;
	virtual void						UpdateThread_CollectedForRendering(const PRendererCacheBase &rendererCache) override;
	virtual CBillboardingBatchInterface	*CreateBillboardingBatch(ERendererClass rendererType, const PRendererCacheBase &rendererCache, bool gpuStorage) override;

	void								SetPackPath(const char* path) { m_PackPath = CString(path); }
	void								SetRenderManager(CRenderManager *renderManager) { m_RendererLoader.SetRenderManager(renderManager); }

	PMaterialCache						FindMaterial(const SMaterialCacheKey &key) const;
	PPipelineStateCache					FindPipelineState(const SPipelineStateCacheKey &key) const;
	PGeometryCache						FindGeometryCache(const CString &path) const;

private:
	CString										m_PackPath;
	PopcornFXRendererLoader						m_RendererLoader;
};

//----------------------------------------------------------------------------
__LMBRPK_END

#endif //LMBR_USE_PK
