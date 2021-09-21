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

#include <pk_kernel/include/kr_threads_rwlock.h>
#include <pk_maths/include/pk_maths_primitives.h>

#if defined(PK_USE_PHYSX)
	#include <AzCore/Math/Aabb.h>
	#include <AzFramework/Physics/WindBus.h>
#endif

__LMBRPK_BEGIN
//----------------------------------------------------------------------------

class IWindManager
{
public:
	virtual void	Activate() = 0;
	virtual void	Deactivate() = 0;
	virtual bool	Reset(const AZStd::string &libraryPath) = 0;
	virtual void	Update() = 0;
};

#if defined(PK_USE_PHYSX)

//Bind deprecated LmbrWind, remove it in the next version
#define BIND_LMBRWIND 1

class CWindManagerBase
	: public IWindManager
{
public:
	CWindManagerBase();

	virtual bool	Reset(const AZStd::string &libraryPath);

	virtual void	SampleWindField(const TStridedMemoryView<CFloat3>		&dstWind,
									const TStridedMemoryView<const CFloat3>	&srcLocation) = 0;

	static CWindManagerBase	*Get() { return s_Self; }

private:
#if BIND_LMBRWIND
	bool							_BindLmbrSimInterface() const;
	void							_UnbindLmbrSimInterface();
#endif
	bool							_BindSceneSimInterface() const;
	void							_UnbindSceneSimInterface();

#if BIND_LMBRWIND
	CString							m_LmbrWindSimInterfacePath;
#endif
	CString							m_SceneWindSimInterfacePath;
	static CWindManagerBase			*s_Self;
};

class CWindManager
	: public CWindManagerBase
	, public ::Physics::WindNotificationsBus::Handler
{
public:
	virtual void	Activate();
	virtual void	Deactivate();
	virtual void	Update();

	virtual void	SampleWindField(const TStridedMemoryView<CFloat3>		&dstWind,
									const TStridedMemoryView<const CFloat3>	&srcLocation);

	// Physics::WindNotificationsBus::Handler overrides
	void			OnGlobalWindChanged() override;
	void			OnWindChanged(const AZ::Aabb& aabb) override;

	struct SWindHelper
	{
		struct SArea_Helper
		{
			PopcornFX::CAABB	m_WorldBB;
			CFloat4x4			m_WindAreaInvTransform;
			CFloat3				m_Wind;
		};

		CFloat3							m_GlobalWind = CFloat3::ZERO;
		bool							m_WindChanged = false;
		PopcornFX::TArray<SArea_Helper>	m_Areas;
		Threads::CRWLock				m_Lock;
	};

	SWindHelper	&WindHelper() { return m_WindHelper; }

private:
	SWindHelper	m_WindHelper;
};

#else

//Dummy
class CWindManager
	: public IWindManager
{
public:
	virtual void	Activate() { }
	virtual void	Deactivate() { }
	virtual bool	Reset(const AZStd::string &libraryPath) { return true; }
	virtual void	Update() {}
};

#endif

//----------------------------------------------------------------------------
__LMBRPK_END

#endif //LMBR_USE_PK
