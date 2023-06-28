//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "PopcornFX_precompiled.h"

#include "SceneInterface.h"

#if defined(O3DE_USE_PK)

#if defined(PK_USE_PHYSX)
	#include <AzFramework/Physics/PhysicsScene.h>
	#include <AzFramework/Physics/Material/PhysicsMaterialManager.h>
	#include <AzCore/Interface/Interface.h>
	#include <AzFramework/Physics/ShapeConfiguration.h>
	#include <AzCore/std/smart_ptr/make_shared.h>
	#include <PhysX/Material/PhysXMaterial.h>
#endif

#include <AzCore/Console/IConsole.h>

#include "Integration/PopcornFXUtils.h"

namespace PopcornFX {

	static void PrintPopcornFXPhysicsSurfaceTypesConstants(const AZ::ConsoleCommandContainer&)
	{
		if (auto *materialManager = AZ::Interface<Physics::MaterialManager>::Get())
		{
			AZStd::shared_ptr<Physics::Material> defaultMaterial = materialManager->GetDefaultMaterial();
			if (defaultMaterial != null)
			{
				AZ_Printf("PopcornFX", "physics.surfaceTypes.Default %u", AZ::Crc32(defaultMaterial->GetId().ToString<AZStd::string>()));
			}
		}

		AZ::Data::AssetCatalogRequests::AssetEnumerationCB	popcornFxAssetReloadCb = [](const AZ::Data::AssetId id, const AZ::Data::AssetInfo& info)
		{
			if (info.m_assetType == ::Physics::MaterialAsset::RTTI_Type())
			{
				Physics::MaterialId	materialId = Physics::MaterialId::CreateFromAssetId(id);
				AZStd::string		materialName = info.m_relativePath;

				AZ::StringFunc::Replace(materialName, ".physicsmaterial", "");
				AZ::StringFunc::Replace(materialName, "/", ".");
				
				AZ_Printf("PopcornFX", "physics.surfaceTypes.%s %u", materialName.c_str(), AZ::Crc32(materialId.ToString<AZStd::string>()));
			}
		};
		AZ::Data::AssetCatalogRequestBus::Broadcast(&AZ::Data::AssetCatalogRequestBus::Events::EnumerateAssets, null, popcornFxAssetReloadCb, null);
	}

	AZ_CONSOLEFREEFUNC(PrintPopcornFXPhysicsSurfaceTypesConstants, AZ::ConsoleFunctorFlags::Null, "Print the physics surface types constants to add in your PopcornFX project settings.");

#if defined(PK_USE_PHYSX)
	void	CSceneInterface::RayTracePacket(const Colliders::STraceFilter	&traceFilter,
											const Colliders::SRayPacket		&packet,
											const Colliders::STracePacket	&results)
	{
		AzPhysics::SceneHandle		sceneHandle = AzPhysics::InvalidSceneHandle;
		AzPhysics::SceneInterface	*sceneInterface = AZ::Interface<AzPhysics::SceneInterface>::Get();
		if (sceneInterface != null)
		{
#if defined(POPCORNFX_EDITOR)
			if (!m_InGameMode)
				sceneHandle = sceneInterface->GetSceneHandle(AzPhysics::EditorPhysicsSceneName);
			else
				sceneHandle = sceneInterface->GetSceneHandle(AzPhysics::DefaultPhysicsSceneName);
#else
			sceneHandle = sceneInterface->GetSceneHandle(AzPhysics::DefaultPhysicsSceneName);
#endif
		}

		if (!PK_VERIFY(sceneHandle != AzPhysics::InvalidSceneHandle))
			return;

		Physics::MaterialManager	*materialManager = null;
		if (results.m_ContactSurfaces_Aligned16 != null)
		{
			materialManager = AZ::Interface<Physics::MaterialManager>::Get();
			if (!PK_VERIFY(materialManager != null))
				return;
		}

		AzPhysics::CollisionGroup	collisionGroup(traceFilter.m_FilterFlags == 0 ? AzPhysics::CollisionGroup::All.GetMask() : traceFilter.m_FilterFlags);

		const u32	resCount = results.Count();
		const bool	emptyMasks = packet.m_RayMasks_Aligned16.Empty();
		const bool	emptySphereSweeps = packet.m_RaySweepRadii_Aligned16.Empty();

		for (u32 rayi = 0; rayi < resCount; ++rayi)
		{
			if (!emptyMasks && packet.m_RayMasks_Aligned16[rayi] == 0)
				continue;

			const CFloat4	&_rayDirAndLen = packet.m_RayDirectionsAndLengths_Aligned16[rayi];
			if (_rayDirAndLen.w() <= 0)
				continue;
			const CFloat3	&start = packet.m_RayOrigins_Aligned16[rayi].xyz();
			const CFloat3	&dir = _rayDirAndLen.xyz();

			// Perform raycast
			AzPhysics::SceneQueryHits	hitResult;
			if (emptySphereSweeps || packet.m_RaySweepRadii_Aligned16[rayi] == 0.0f)
			{
				AzPhysics::RayCastRequest	request;
				request.m_start = ToAZ(start);
				request.m_direction = ToAZ(dir);
				request.m_distance = _rayDirAndLen.w();
				request.m_collisionGroup = collisionGroup;
				PK_ASSERT(request.m_reportMultipleHits == false);

				hitResult = sceneInterface->QueryScene(sceneHandle, &request);
			}
			else
			{
				AzPhysics::ShapeCastRequest	request;
				request.m_distance = _rayDirAndLen.w();
				request.m_start = AZ::Transform::CreateTranslation(ToAZ(start));
				request.m_direction = ToAZ(dir);
				request.m_shapeConfiguration = AZStd::make_shared<Physics::SphereShapeConfiguration>(packet.m_RaySweepRadii_Aligned16[rayi]);
				request.m_collisionGroup = collisionGroup;
				PK_ASSERT(request.m_reportMultipleHits == false);

				hitResult = sceneInterface->QueryScene(sceneHandle, &request);
			}
			if (!hitResult.m_hits.empty())
			{
				//m_reportMultipleHits in AzPhysics::RayCastRequest and AzPhysics::ShapeCastRequest are set to false by default, only 1 hit possible
				const AzPhysics::SceneQueryHit	&hit = hitResult.m_hits[0];

				results.m_HitTimes_Aligned16[rayi] = hit.m_distance;
				if (results.m_ContactObjects_Aligned16 != null)
				{
					const bool haveBodyHandle = hit.m_resultFlags & AzPhysics::SceneQuery::ResultFlags::BodyHandle;
					if (!haveBodyHandle)
						results.m_ContactObjects_Aligned16[rayi] = null;
					else
						results.m_ContactObjects_Aligned16[rayi] = sceneInterface->GetSimulatedBodyFromHandle(sceneHandle, hit.m_bodyHandle);
				}
				if (results.m_ContactSurfaces_Aligned16 != null)
				{
					const bool havePhysicsMaterial = hit.m_resultFlags & AzPhysics::SceneQuery::ResultFlags::Material;
					if (!havePhysicsMaterial)
						results.m_ContactSurfaces_Aligned16[rayi] = null;
					else
					{
						AZStd::shared_ptr<PhysX::Material>	material = AZStd::rtti_pointer_cast<PhysX::Material>(materialManager->GetMaterial(hit.m_physicsMaterialId));
						results.m_ContactSurfaces_Aligned16[rayi] = material.get();
					}
				}
				if (results.m_ContactPoints_Aligned16 != null)
					results.m_ContactPoints_Aligned16[rayi].xyz() = ToPk(hit.m_position);
				if (results.m_ContactNormals_Aligned16 != null)
					results.m_ContactNormals_Aligned16[rayi].xyz() = ToPk(hit.m_normal);
			}
		}
	}

	void	CSceneInterface::ResolveContactMaterials(	[[maybe_unused]] const TMemoryView<void * const>	&contactObjects,
														const TMemoryView<void * const>						&contactSurfaces,
														const TMemoryView<Colliders::SSurfaceProperties>	&outSurfaceProperties) const
	{
		PK_ASSERT(contactObjects.Count() == contactSurfaces.Count());
		PK_ASSERT(contactObjects.Count() == outSurfaceProperties.Count());
	
		static const PopcornFX::Colliders::SSurfaceProperties		kDefaultSurface;

		const u32	materialCount = contactSurfaces.Count();
		for (u32 iMaterial = 0; iMaterial < materialCount; ++iMaterial)
		{
			PopcornFX::Colliders::SSurfaceProperties	&surface = outSurfaceProperties[iMaterial];
			surface = kDefaultSurface;

			PhysX::Material	*material = reinterpret_cast<PhysX::Material*>(contactSurfaces[iMaterial]);
			if (material == null)
				continue;

			surface.m_Restitution = material->GetRestitution();
			surface.m_StaticFriction = material->GetStaticFriction();
			surface.m_DynamicFriction = material->GetDynamicFriction();
			surface.m_SurfaceType = AZ::Crc32(material->GetId().ToString<AZStd::string>());

#define REMAP_COMBINE_MODE(__member, __val) \
		switch (__val) \
		{ \
			case	PhysX::CombineMode::Average: \
				surface.__member = PopcornFX::Colliders::ECombineMode::Combine_Average; \
				break; \
			case	PhysX::CombineMode::Minimum: \
				surface.__member = PopcornFX::Colliders::ECombineMode::Combine_Min; \
				break; \
			case	PhysX::CombineMode::Maximum: \
				surface.__member = PopcornFX::Colliders::ECombineMode::Combine_Max; \
				break; \
			case	PhysX::CombineMode::Multiply: \
				surface.__member = PopcornFX::Colliders::ECombineMode::Combine_Multiply; \
				break; \
			default: \
				PK_ASSERT_NOT_REACHED(); \
				break; \
		}

			REMAP_COMBINE_MODE(m_FrictionCombineMode, material->GetFrictionCombineMode());
			REMAP_COMBINE_MODE(m_RestitutionCombineMode, material->GetRestitutionCombineMode());

#undef REMAP_COMBINE_MODE
		}
	}
#endif //PK_USE_PHYSX

}

#endif //O3DE_USE_PK
