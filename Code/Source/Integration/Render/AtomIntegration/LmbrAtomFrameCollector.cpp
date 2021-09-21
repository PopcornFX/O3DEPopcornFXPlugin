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
#include "LmbrAtomFrameCollector.h"

#if defined(LMBR_USE_PK)

__LMBRPK_BEGIN
//----------------------------------------------------------------------------

CLmbrAtomFrameCollector::CLmbrAtomFrameCollector()
{

}

CLmbrAtomFrameCollector::~CLmbrAtomFrameCollector()
{

}

bool	CLmbrAtomFrameCollector::EarlyCull(const PopcornFX::CAABB &bbox) const
{
	// Can happen if bounds are not active
	if (!bbox.IsFinite() ||
		!bbox.Valid())
		return false;
	return false;
}

//----------------------------------------------------------------------------
__LMBRPK_END

#endif //LMBR_USE_PK
