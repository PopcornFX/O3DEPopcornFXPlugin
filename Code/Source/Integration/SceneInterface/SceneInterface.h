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

#include <pk_particles/include/ps_scene.h>

#define RESOLVE_MATERIAL_PROPERTIES 0
#define RESOLVE_CONTACT_OBJECT 0

__LMBRPK_BEGIN

	class	CSceneInterface : public IParticleScene
	{
	public:
		virtual void	RayTracePacket(	const Colliders::STraceFilter	&traceFilter,
										const Colliders::SRayPacket		&packet,
										const Colliders::STracePacket	&results) override;

#if RESOLVE_MATERIAL_PROPERTIES
		virtual void	ResolveContactMaterials(const TMemoryView<void * const>					&contactObjects,
												const TMemoryView<void * const>					&contactSurfaces,
												const TMemoryView<Colliders::SSurfaceProperties>&outSurfaceProperties) const override;
#endif

		void	SetInGameMode(bool inGameMode) { m_InGameMode = inGameMode; }

	private:
		bool	m_InGameMode = false;
	};

__LMBRPK_END

#endif //LMBR_USE_PK
