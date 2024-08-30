//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#if defined(O3DE_USE_PK)

#include "ParticleBuffers.h"
#include <pk_render_helpers/include/batch_jobs/rh_batch_jobs_light_std.h>
#include <pk_render_helpers/include/draw_requests/rh_draw_requests.h>

namespace PopcornFX {
//----------------------------------------------------------------------------

class	CLightBatchDrawer :	public CRendererBatchJobs_Light_Std,
							public CBatchDrawerCommon
{
public:
	typedef CRendererBatchJobs_Light_Std	Super;

	CLightBatchDrawer();
	virtual ~CLightBatchDrawer();

	virtual bool	AreRenderersCompatible(const CRendererDataBase *rendererA, const CRendererDataBase *rendererB) const override;
	virtual bool	EmitDrawCall(SRenderContext &ctx, const SDrawCallDesc &toEmit) override;

	virtual bool	Step_AllocBuffers(SRenderContext &ctx) { AZ_UNUSED(ctx); return true; }
	virtual bool	Step_MapBuffers(SRenderContext &ctx) { AZ_UNUSED(ctx); return true;  }
	virtual bool	Step_LaunchBillboardingTasks(SRenderContext &ctx, Drawers::PAsynchronousJob_PostRenderTasks &syncJob) { AZ_UNUSED(ctx);  AZ_UNUSED(syncJob); return true; }
	virtual bool	Step_WaitForBillboardingTasks(SRenderContext &ctx) { AZ_UNUSED(ctx); return true; }
	virtual bool	Step_UnmapBuffers(SRenderContext &ctx) { AZ_UNUSED(ctx); return true; }
};

//----------------------------------------------------------------------------
}

#endif //O3DE_USE_PK
