//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "PopcornFX_precompiled.h"

#include "PopcornFxStartup.h"

#if defined(O3DE_USE_PK)

#include "Integration/ResourceHandlers/ImageResourceHandler.h"
#include "Integration/ResourceHandlers/MeshResourceHandler.h"
#include "Integration/File/FileSystemController_O3DE.h"

#include <AzCore/Memory/Memory.h>
#include <AzCore/Jobs/Job.h>

#include <pk_version_base.h>
#include <pk_kernel/include/kr_init.h>
#include <pk_base_object/include/hb_init.h>
#include <pk_engine_utils/include/eu_init.h>
#include <pk_compiler/include/cp_init.h>
#include <pk_imaging/include/im_init.h>
#include <pk_geometrics/include/ge_init.h>
#include <pk_particles/include/ps_init.h>
#include <pk_particles_toolbox/include/pt_init.h>
#include <pk_render_helpers/include/rh_init.h>

#include <pk_kernel/include/kr_assert_internals.h>
#include <pk_kernel/include/kr_static_config_flags.h>

#include <pk_kernel/include/kr_log_listeners.h>

#include <pk_kernel/include/kr_thread_pool_default.h>
#include <pk_kernel/include/kr_thread_pool_details.h>

#include <pk_geometrics/include/ge_coordinate_frame.h>

#ifdef PK_DEBUG
#	include <pk_kernel/include/kr_mem_stats.h>
#endif

PK_LOG_MODULE_DECLARE();

#define PK_USE_O3DE_ASSERTS	0

//----------------------------------------------------------------------------

namespace PopcornFX {

	//----------------------------------------------------------------------------
	//
	// Loggers
	//
	//----------------------------------------------------------------------------

#ifndef	PK_RETAIL
	class	CLogListenerO3DE : public ILogListener
	{
	public:
		virtual void	Notify(CLog::ELogLevel level, CGuid logClass, const char *message) override
		{
			// AZ_Trace/AZ_Info do not append line return (internal implementation calls Printf with input message as-is)
			const char		*lr = (level == PopcornFX::CLog::Level_Debug || PopcornFX::CLog::Level_Info) ? "\n" : "";
			const CString	s = CString::Format("[%s] %s%s", CLog::LogClassToString(logClass), message, lr);
			switch (level)
			{
				case	PopcornFX::CLog::Level_Debug:
					AZ_Trace("PopcornFX", s.Data());
					break;
				case	PopcornFX::CLog::Level_Info:
					AZ_Info("PopcornFX", s.Data());
					break;
				case	PopcornFX::CLog::Level_Warning:
				case	PopcornFX::CLog::Level_ErrorInternal: /* asserts */
					AZ_Warning("PopcornFX", false, s.Data());
					break;
				case	PopcornFX::CLog::Level_Error:
				case	PopcornFX::CLog::Level_ErrorCritical:
					AZ_Error("PopcornFX", false, s.Data());
					break;
				case	PopcornFX::CLog::Level_None: /* used mainly to disable all logging when calling 'SetGlobalLogLevel()' */
					break;
			};
		}
	};
#endif

	//----------------------------------------------------------------------------

	void	PopcornFX_AddLogListener(void *userHandle)
	{
		AZ_UNUSED(userHandle);

#ifndef	PK_RETAIL
		CLog::AddGlobalListener(PK_NEW(CLogListenerO3DE));
#endif
	}

	//----------------------------------------------------------------------------
	//
	// Worker Thread Pool
	//
	//----------------------------------------------------------------------------

#define		USE_O3DE_JOBS		0

#if		USE_O3DE_JOBS
	struct	SO3DEJob : public AZ::Job
	{
		AZ_CLASS_ALLOCATOR(SO3DEJob, AZ::ThreadPoolAllocator, 0);

		SO3DEJob(AZ::JobContext *context = null)
			: AZ::Job(true, context)
			, m_PkfxTask(null)
			, m_ThreadContext(null)
		{
		}

		~SO3DEJob() = default;

		Threads::PAbstractTask		m_PkfxTask;
		Threads::SThreadContext 	*m_ThreadContext;

	protected:
		void Process() override
		{
			PK_SCOPEDPROFILE();
			if (!CCurrentThread::IsRegistered())
				CCurrentThread::RegisterUserThread();
			m_PkfxTask->Run(*m_ThreadContext);
		}
	};

	class	CO3DEThreadPool : public Threads::CAbstractPool
	{
	public:
		CO3DEThreadPool()
			: m_Context(this, 1)
		{
		}

		virtual ~CO3DEThreadPool()
		{
		}

		// These can be called by tasks to enqueue other dependent tasks
		void	SubmitTask(Threads::CAbstractTask *task) override
		{
			PK_SCOPEDPROFILE();
			if (!PK_VERIFY(task != null))
				return;

			const CPU::CScoped_FpuDisableExceptions	_de;
			const CPU::CScoped_FpuEnableFastMode	_ftz;

			SO3DEJob	*job = new SO3DEJob;

			job->m_ThreadContext = &m_Context;
			job->m_PkfxTask = task;

			job->Start();
		}

		CStringView	ThreadPoolName() const override { return "O3DE PopcornFX thread pool"; };

	private:
		Threads::SThreadContext				m_Context;
	};
	PK_DECLARE_REFPTRCLASS(O3DEThreadPool);
#endif

	static Threads::PAbstractPool		PopcornFX_CreateCustomThreadPool()
	{
		bool						success = true;

#if		USE_O3DE_JOBS
		// O3DE thread pool:
		CO3DEThreadPool				*threadPool = PK_NEW(CO3DEThreadPool);
		return threadPool;
#else

		bool						hasExplicitAffinities = false;
		u32							affinityCount = 0;
		CThreadManager::EPriority	workersPriority = CThreadManager::Priority_High;
		const u32					kWorkerAffinities[] = { 0 };

		PWorkerThreadPool	pool = PK_NEW(CWorkerThreadPool);

		if (PK_VERIFY(pool != null))
		{
			if (hasExplicitAffinities)	// user specified explicit worker affinities: override our own scheduling entirely.
			{
				AZ_Info("PopcornFX", "Found explicit user affinity masks for worker-threads. Overriding default thread-pool creation.");

				CGenericAffinityMask	affinityMask;

				for (u32 i = 0; i < affinityCount; i++)
				{
					if (kWorkerAffinities[i] != 0)
					{
						affinityMask.Clear();
						affinityMask.SetAffinityBlock32(0, kWorkerAffinities[i]);

						success &= (null != pool->AddWorker(workersPriority, &affinityMask));
					}
				}

				if (!success)
				{
					AZ_Error("PopcornFX", false, "Could not create some of the explicitely affinitized worker-threads.");
				}

				success = true;	// still keep what has been created. don't fail here.
			}
			else
			{
#if defined(PK_DESKTOP) || defined(PK_MOBILE)
				// Let the OS shedule our workers
				// leave 2 core for main thread and render thread
				u32		processorCount = PKMin(PKMax(CPU::Caps().ProcessAffinity().NumBitsSet(), 4U) - 3U, 12);

				success = pool->AddFullAffinityWorkers(processorCount, CPU::Caps().ProcessAffinity(), workersPriority);
#else
				success = pool->AddNUMAWorkers();
#endif
			}

			if (!success)
				return null;

			pool->StartWorkers();
		}
		return pool;
#endif
	}


	//----------------------------------------------------------------------------
	//
	// Startup
	//
	//----------------------------------------------------------------------------

	PopcornFX::IFileSystem	*PopcornFX_CreateNewFileSystem()
	{
		return PK_NEW(FileSystemController_O3DE);
	}

	void	*PopcornFX_Alloc(size_t size, PopcornFX::Mem::EAllocType type)
	{
		AZ_UNUSED(type);
		return AZ::AllocatorInstance<AZ::SystemAllocator>::Get().Allocate(size, AZCORE_GLOBAL_NEW_ALIGNMENT); // popcornfx takes care of alignment already
	}

	void	*PopcornFX_Realloc(void *ptr, size_t size, PopcornFX::Mem::EAllocType type)
	{
		AZ_UNUSED(type);
		return AZ::AllocatorInstance<AZ::SystemAllocator>::Get().ReAllocate(ptr, size, AZCORE_GLOBAL_NEW_ALIGNMENT);
	}

	void	PopcornFX_Free(void *ptr, PopcornFX::Mem::EAllocType type)
	{
		AZ_UNUSED(type);
		return AZ::AllocatorInstance<AZ::SystemAllocator>::Get().DeAllocate(ptr);
	}

	void	PrettyFormatAssert_Unsafe(char *buffer, u32 bufferLen, PK_ASSERT_CATCHER_PARAMETERS)
	{
		PK_ASSERT_CATCHER_KILLARGS;
		using namespace PopcornFX;

		const CThreadID		threadId = CCurrentThread::ThreadID();

#if	(PK_CALL_SCOPE_ENABLED != 0)
		PopcornFX::CString	callCtxStr = CCallContext::ReadStackToString();
		const char			*callCtxStrPtr = callCtxStr.Data();
#else
		const char			*callCtxStrPtr = null;
#endif	//(PK_CALL_SCOPE_ENABLED != 0)

		// pointer compare ok (compiler probably removed duplicated static strings, else we dont care)
		const bool		pexp = (expanded != failed);
		const bool		pmsg = (message != failed && message != expanded);
		const bool		cctx = callCtxStrPtr != null;
		SNativeStringUtils::SPrintf(
			buffer, bufferLen,
			"!! PopcornFX Assertion failed !!"
			"\nFile       : %s(%d)"
			"\nFunction   : %s(...)"
			"%s%s"// Message
			"\nCondition  : %s"
			"%s%s" // Expanded
			"%s%s" // Call context
			"\nThreadID   : %d"
			"\n"
			, file, line
			, function
			, (pmsg	? "\nMessage    : " : ""), (pmsg ? message : "")
			, failed
			, (pexp	? "\nExpanded   : " : ""), (pexp ? expanded : "")
			, (cctx ? "\nCallContext: " : ""), (cctx ? callCtxStrPtr : "")
			, u32(threadId));
	}

	PopcornFX::Assert::EResult	PopcornFX_Assert(PK_ASSERT_CATCHER_PARAMETERS)
	{
		PK_ASSERT_CATCHER_KILLARGS;

		char						_buffer[2048];
		PopcornFX::PrettyFormatAssert_Unsafe(_buffer, sizeof(_buffer), PK_ASSERT_CATCHER_ARGUMENTS);
		[[maybe_unused]] const char	*prettyMessage = _buffer;

#if (PK_USE_O3DE_ASSERTS != 0)
		// Let O3DE handle the assert.
		AZ::Debug::Trace::Instance().Assert(file, line, function, prettyMessage);

		return PopcornFX::Assert::Result_Skip; // O3DE does its thing internally (keeps track of ignored state per assert).
#else
		// Reroute the assert into an O3DE error and skip additional occurences of the assert.
		// This avoids embedding PopcornFX default assert handlers in the plugin, breaks for the first occurence of an assert.
		// PK_USE_O3DE_ASSERTS should be turned on at some point if we can invoke O3DE's assert handler without printing out the callstack (stalls for several minutes in debug)
#	if !defined(POPCORNFX_BUILDER) // Don't crash builders
		AZ_Error("PopcornFX", false, prettyMessage);
		PK_BREAKPOINT();
#else
		AZ_Warning("PopcornFX", false, prettyMessage);
#	endif // !defined(POPCORNFX_BUILDER)
		return PopcornFX::Assert::Result_Ignore;
#endif // (PK_USE_O3DE_ASSERTS)
	}

	bool	PopcornStartup(const char *assertScriptFilePath /*= null*/)
	{
		AZ_UNUSED(assertScriptFilePath);

#ifdef PK_DEBUG
		PopcornFX::CMemStats::EnableCounterTracking(true);
#endif

		SDllVersion	engineVersion;

		PK_ASSERT(engineVersion.Major == PK_VERSION_MAJOR);
		PK_ASSERT(engineVersion.Minor == PK_VERSION_MINOR);
#ifdef	PK_DEBUG
		PK_ASSERT(engineVersion.Debug == true);
#else
		PK_ASSERT(engineVersion.Debug == false);
#endif
		CPKKernel::Config	configKernel;

		// Setup FileSystem
		configKernel.m_NewFileSystem = PopcornFX_CreateNewFileSystem;

		// Setup Allocator
		configKernel.m_DefaultAllocator_Alloc = &PopcornFX_Alloc;
		configKernel.m_DefaultAllocator_Realloc = &PopcornFX_Realloc;
		configKernel.m_DefaultAllocator_Free = &PopcornFX_Free;
		// Let O3DE's allocator handle out of memory situations
		// configKernel.m_DefaultAllocator_OutOfMemory = &PopcornFX_AllocFailed;

#if	(PK_ASSERTS_IN_DEBUG != 0) || (PK_ASSERTS_IN_RELEASE != 0)
		configKernel.m_AssertCatcher = &PopcornFX_Assert;
#endif // (PK_ASSERTS_IN_DEBUG != 0) || (PK_ASSERTS_IN_RELEASE != 0)

		configKernel.m_CreateThreadPool = &PopcornFX_CreateCustomThreadPool;

		// depending on wether or not we want default loggers, override the callback:
		configKernel.m_AddDefaultLogListeners = &PopcornFX_AddLogListener;

		CPKParticles::Config	particleConfig;

		particleConfig.m_EnableBillboardingStats = true;
		if (CPKKernel::Startup(engineVersion, configKernel) &&
			CPKBaseObject::Startup(engineVersion, CPKBaseObject::Config()) &&
			CPKEngineUtils::Startup(engineVersion, CPKEngineUtils::Config()) &&
			CPKCompiler::Startup(engineVersion, CPKCompiler::Config()) &&
			CPKImaging::Startup(engineVersion, CPKImaging::Config()) &&
			CPKGeometrics::Startup(engineVersion, CPKGeometrics::Config()) &&
			CPKParticles::Startup(engineVersion, particleConfig) &&
			ParticleToolbox::Startup() &&
			CPKRenderHelpers::Startup(engineVersion, CPKRenderHelpers::Config()) &&
			Kernel::CheckStaticConfigFlags(Kernel::g_BaseStaticConfig, SKernelConfigFlags()))
		{
			PK_LOG_MODULE_INIT_START;

			// Set the coordinate frame:
			CCoordinateFrame::SetGlobalFrame(Frame_RightHand_Z_Up);

			// Bind resource handlers:
			CImageResourceHandler	*imgHandler = PK_NEW(CImageResourceHandler);
			if (imgHandler != null)
				Resource::DefaultManager()->RegisterHandler<CImage>(imgHandler);

			CMeshResourceHandler		*meshHandler = PK_NEW(CMeshResourceHandler);
			if (meshHandler != null)
				Resource::DefaultManager()->RegisterHandler<CResourceMesh>(meshHandler);

			PK_LOG_MODULE_INIT_END;
			return true;
		}

		PopcornShutdown();	// shutdown the modules we were able to load...
		return false;
	}

	//----------------------------------------------------------------------------

	void	PopcornShutdown()
	{
		PK_LOG_MODULE_RELEASE_START;

		IResourceHandler	*imgHandler;
		imgHandler = Resource::DefaultManager()->FindHandler<CImage>();
		Resource::DefaultManager()->UnregisterHandler<CImage>(imgHandler);
		PK_DELETE(imgHandler);

		IResourceHandler	*meshHandler;
		meshHandler = Resource::DefaultManager()->FindHandler<CResourceMesh>();
		Resource::DefaultManager()->UnregisterHandler<CResourceMesh>(meshHandler);
		PK_DELETE(meshHandler);

		PK_LOG_MODULE_RELEASE_END;

		CPKRenderHelpers::Shutdown();
		ParticleToolbox::Shutdown();
		CPKParticles::Shutdown();
		CPKGeometrics::Shutdown();
		CPKImaging::Shutdown();
		CPKCompiler::Shutdown();
		CPKEngineUtils::Shutdown();
		CPKBaseObject::Shutdown();
		CPKKernel::Shutdown();
	}

}
//----------------------------------------------------------------------------

#endif //O3DE_USE_PK

