//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------

#pragma once

#if defined(O3DE_USE_PK)

#undef	PV_MODULE_NAME
#undef	PV_MODULE_SYM
#define	PV_MODULE_NAME		"O3DE"
#define	PV_MODULE_SYM		O3DE

#include <pkapi/include/pk_precompiled_default.h>
#include <pk_version.h>

PK_LOG_MODULE_DEFINE();

#else

#define PK_VERSION_CURRENT_STRING	"0.0.0.0"

constexpr auto	null = nullptr;

#endif //O3DE_USE_PK

#if defined(PK_WINDOWS)
#	pragma warning( disable : 4266 )
#endif

#include <AzCore/PlatformIncl.h>
