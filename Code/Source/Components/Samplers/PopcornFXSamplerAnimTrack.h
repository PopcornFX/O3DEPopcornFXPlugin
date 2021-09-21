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

#include <LmbrCentral/Shape/SplineComponentBus.h>
#include <AzFramework/Asset/SimpleAsset.h>
#include <AzCore/Component/TransformBus.h>
#include <AzCore/Component/EntityId.h>

#include "Integration/PopcornFXIntegrationBus.h"

#if defined(LMBR_USE_PK)
#include <pk_particles/include/ps_samplers_animtrack.h>
#include <pk_engine_utils/include/eu_samplers_curve.h>
#endif //LMBR_USE_PK

__LMBRPK_BEGIN

	class PopcornFXSamplerAnimTrack
		: public LmbrCentral::SplineComponentNotificationBus::Handler
		, public AZ::TransformNotificationBus::Handler
		, public PopcornFXSamplerComponentRequestBus::Handler
	{
	public:
		AZ_TYPE_INFO(PopcornFXSamplerAnimTrack, "{0709E447-862A-4771-8EB3-6376F9A782F5}")

		static void	Reflect(AZ::ReflectContext* context);

		void		CopyFrom(const PopcornFXSamplerAnimTrack &other);

		void		Activate();
		void		Deactivate();

		AZ::EntityId	m_AttachedToEntityId;

	protected:
		void		_OnUseRelativeTransformChanged();

		bool		m_UseRelativeTransform = true;

	private:
		static bool VersionConverter(	AZ::SerializeContext& context,
										AZ::SerializeContext::DataElementNode& classElement);

#if defined(LMBR_USE_PK)
	public:
		//////////////////////////////////////////////////////////////////////////
		// PopcornFXSamplerComponentRequestBus interface implementation
		PopcornFX::CParticleSamplerDescriptor*	GetDescriptor() override { return m_SamplerDescriptor.Get(); }
		virtual AZ::u32							GetType() override { return CParticleSamplerDescriptor_AnimTrack::SamplerTypeID(); };
		//////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////
		// LmbrCentral::SplineComponentNotificationBus::Handler interface implementationa
		virtual void	OnSplineChanged() override;
		////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// TransformNotificationBus
		//! Called when the local transform of the entity has changed. Local transform update always implies world transform change too.
		virtual void	OnTransformChanged(const AZ::Transform& local, const AZ::Transform& world) override;
		//////////////////////////////////////////////////////////////////////////

	protected:
		void	_Clean();
		void	_SetTransform();
		bool	_BuildDescriptor(AZ::ConstSplinePtr spline);


		PParticleSamplerDescriptor	m_SamplerDescriptor = null;
		CFloat4x4					m_CurrentMat = CFloat4x4::IDENTITY;
		CFloat4x4					m_CurrentMatUnscaled = CFloat4x4::IDENTITY;
		CCurveDescriptor			*m_Positions = null;
#endif //LMBR_USE_PK
};

__LMBRPK_END
