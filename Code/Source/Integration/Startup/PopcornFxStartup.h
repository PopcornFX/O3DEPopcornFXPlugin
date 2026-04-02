//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------

#pragma once

#if defined(O3DE_USE_PK)

#include <pk_module_api.h>
#include <pk_endian.h>
#include <pk_maths/include/pk_maths.h>

// hh_kernel specific headers
#include <pk_kernel/include/kr_mem.h>
#include <pk_kernel/include/kr_containers.h>
#include <pk_kernel/include/kr_containers_hash.h>
#include <pk_kernel/include/kr_string.h>
#include <pk_kernel/include/kr_log.h>
#include <pk_kernel/include/kr_timers.h>
#include <pk_kernel/include/kr_buffer.h>
#include <pk_kernel/include/kr_file.h>

namespace PopcornFX {
	bool	PopcornStartup(const char *assertScriptFilePath = null);
	void	PopcornShutdown();
}

#endif //O3DE_USE_PK
