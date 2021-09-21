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

__LMBRPK_BEGIN
	bool	PopcornStartup(const char *assertScriptFilePath = null, bool installDefaultLogger = true);
	void	PopcornShutdown();
__LMBRPK_END

#endif //LMBR_USE_PK
