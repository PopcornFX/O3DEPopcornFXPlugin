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

#undef	PV_MODULE_NAME
#undef	PV_MODULE_SYM
#define	PV_MODULE_NAME		"O3DE"
#define	PV_MODULE_SYM		O3DE

#include <pkapi/include/pk_precompiled_default.h>
#include <pk_version.h>

PK_LOG_MODULE_DEFINE();

#else

#define	PK_VERSION_CURRENT_STRING	"0.0.0.0"

#endif //LMBR_USE_PK

namespace PopcornFX {}
#define __LMBRPK_BEGIN	namespace	LmbrPk { using namespace PopcornFX;
#define __LMBRPK_END	}

#if defined(POPCORNFX_EDITOR)

#if (defined(PK_MACOSX) || defined(PK_LINUX) || defined(PK_WINDOWS)) && defined(PK_CPU_64BITS)
#	define	USE_FBXIMPORTER
#endif

#endif //POPCORNFX_EDITOR

#if defined(PK_WINDOWS)
#	pragma warning( disable : 4266 )
#endif
