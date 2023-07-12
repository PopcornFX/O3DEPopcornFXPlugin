//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "PopcornFX_precompiled.h"
#include "Integration/Managers/RenderManager.h"
#include "LightBatchDrawer.h"

#if defined(O3DE_USE_PK)

#include <pk_render_helpers/include/render_features/rh_features_basic.h>
#include <pk_particles/include/Renderers/ps_renderer_feature_fields.h>

namespace PopcornFX {
//----------------------------------------------------------------------------

CLightBatchDrawer::CLightBatchDrawer()
{
}

//----------------------------------------------------------------------------

CLightBatchDrawer::~CLightBatchDrawer()
{
}

//----------------------------------------------------------------------------

bool	CLightBatchDrawer::AreRenderersCompatible(const CRendererDataBase *rendererA, const CRendererDataBase *rendererB) const
{
	if (!Super::AreRenderersCompatible(rendererA, rendererB))
		return false;
	const CAtomRendererCache	*firstAtomCache = static_cast<const CAtomRendererCache*>(rendererA->m_RendererCache.Get());
	const CAtomRendererCache	*secondAtomCache = static_cast<const CAtomRendererCache*>(rendererB->m_RendererCache.Get());

	// Here, you need to implement a proper renderer cache compatibility check
	// Same materials/shader/uniforms/.. should return true
	return *firstAtomCache == *secondAtomCache; // overloaded == operator (see AtomRendererCache.cpp)
}

//----------------------------------------------------------------------------

bool	CLightBatchDrawer::AllocBuffers(SRenderContext &ctx, const SRendererBatchDrawPass &drawPass)
{
	AZ_UNUSED(drawPass);
	PK_SCOPEDPROFILE();
	PK_ASSERT(!drawPass.m_DrawRequests.Empty());
	PK_ASSERT(drawPass.m_DrawRequests.Count() == drawPass.m_RendererCaches.Count());
	PK_ASSERT(drawPass.m_TotalParticleCount > 0);
	PK_ASSERT(drawPass.m_DrawRequests.First() != null && drawPass.m_RendererCaches.First() != null);

	if (m_RenderContext == null)
	{
		m_RenderContext = static_cast<SAtomRenderContext*>(&ctx);
		PK_ASSERT(m_RenderContext != null);
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CLightBatchDrawer::MapBuffers(SRenderContext &ctx, const SRendererBatchDrawPass &drawPass)
{
	AZ_UNUSED(ctx);
	AZ_UNUSED(drawPass);
	return true;
}

//----------------------------------------------------------------------------

bool	CLightBatchDrawer::UnmapBuffers(SRenderContext &ctx, const SRendererBatchDrawPass &drawPass)
{
	AZ_UNUSED(ctx);
	AZ_UNUSED(drawPass);
	return true;
}

//----------------------------------------------------------------------------

bool	CLightBatchDrawer::EmitDrawCall(SRenderContext &ctx, const SRendererBatchDrawPass &drawPass, const SDrawCallDesc &toEmit)
{
	AZ_UNUSED(ctx);
	AZ_UNUSED(drawPass);
	if (!PK_VERIFY(m_RenderContext->m_Lights.Reserve(m_RenderContext->m_Lights.Count() + toEmit.m_TotalParticleCount)))
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
			PK_ASSERT(page != null);
			if (page->Culled())
				continue;
			const u32	pcount = page == null ? 0 : page->InputParticleCount();
			if (pcount == 0)
				continue;

			// Position
			TStridedMemoryView<const CFloat3>	positions = page->StreamForReading<CFloat3>(br.m_PositionStreamId);
			PK_ASSERT(positions.Count() == pcount);

			// Radius
			TStridedMemoryView<const float>		ranges;
			PK_ALIGN(0x10) float				defaultSize = 0.0f;
			if (PK_VERIFY(br.m_RangeStreamId.Valid()))
				ranges = page->StreamForReading<float>(br.m_RangeStreamId);
			else
				ranges = TStridedMemoryView<const float>(&defaultSize, pcount, 0);

			// Color
			TStridedMemoryView<const CFloat3>	colors(&CFloat3::ONE, pcount, 0);
			if (br.m_ColorStreamId.Valid())
				colors = TStridedMemoryView<const CFloat3>::Reinterpret(page->StreamForReading<CFloat4>(br.m_ColorStreamId));

			// Enabled
			const u8						enabledTrue = u8(-1);
			TStridedMemoryView<const u8>	enableds;
			if (br.m_EnabledStreamId.Valid())
				enableds = page->StreamForReading<bool>(br.m_EnabledStreamId);
			else
				enableds = TStridedMemoryView<const u8>(&enabledTrue, pcount, 0);

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
				if (!PK_VERIFY(m_RenderContext->m_Lights.PushBack().Valid()))
					return false;
				SAtomRenderContext::SLight	&light = m_RenderContext->m_Lights.Last();
				light.m_Position = positions[iParticle];

				// Atom handles the falloff in the shader directly. scale down the color directly with attenuation steepness
				// It's not entirely correct but it's visually close to rendering in PopcornFX Editor
				light.m_Color = colors[iParticle] * br.m_AttenuationSteepness;

				light.m_AttenuationRadius = ranges[iParticle];
			}
		}
	}
	return true;
}

//----------------------------------------------------------------------------
}

#endif //O3DE_USE_PK
