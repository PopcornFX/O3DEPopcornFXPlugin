//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#if defined(O3DE_USE_PK)

#include "AtomFrameCollectorTypes.h"
#include "AtomRendererCache.h"
#include "Integration/Preloader/PopcornFXRendererLoader.h"
#include <Atom/Utils/AssetCollectionAsyncLoader.h>
#include <pk_render_helpers/include/frame_collector/rh_particle_render_data_factory.h>
#include <pk_kernel/include/kr_threads_basics.h>

namespace PopcornFX {
//----------------------------------------------------------------------------

class	CRenderManager;

class	CAtomRenderDataFactory
{
public:
	CAtomRenderDataFactory() { }
	~CAtomRenderDataFactory() { }

	CRendererBatchDrawer	*CreateBatchDrawer(ERendererClass rendererType, const PRendererCacheBase &rendererCache, bool gpuStorage);
	PRendererCacheBase		CreateRendererCache(const PRendererDataBase &renderer, const CParticleDescriptor *particleDesc);

	void								SetPackPath(const char *path) { m_PackPath = CString(path); }

	void								UpdatePendingRendererCache() { m_RendererLoader.UpdatePendingRendererCache(); }

private:
	CString										m_PackPath;
	PopcornFXRendererLoader						m_RendererLoader;
};

//----------------------------------------------------------------------------
}

#endif //O3DE_USE_PK
