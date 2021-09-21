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

#include <pk_render_helpers/include/frame_collector/rh_frame_collector.h>

__LMBRPK_BEGIN
//----------------------------------------------------------------------------

// Frame collector is templated with user data:
class	CLmbrAtomFrameCollector : public TFrameCollector<CLmbrAtomParticleBatchTypes>
{
public:
	CLmbrAtomFrameCollector();
	virtual ~CLmbrAtomFrameCollector();

private:
	// Early Cull: Culls an entire medium on the update thread (when collecting the frame)
	virtual bool	EarlyCull(const PopcornFX::CAABB &bbox) const override;

	// Late Cull: Cull draw requests or individual pages on the render thread (when collecting draw calls)
	// You can use this method if you don't have render thread views available from the update thread
	// Ideally, cull in EarlyCull, but you should implement both methods
	// Late cull also allows for finer culling (per draw request / per page)
	virtual bool	LateCull(const PopcornFX::CAABB &bbox) const override { return EarlyCull(bbox); }
};

//----------------------------------------------------------------------------
__LMBRPK_END

#endif //LMBR_USE_PK
