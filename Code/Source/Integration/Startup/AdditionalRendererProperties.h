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

#include <pk_kernel/include/kr_string_id.h>

__LMBRPK_BEGIN

namespace	AdditionalRendererProperties
{
	CStringId	SID_LmbrAdditionalData();
	CStringId	SID_LmbrAdditionalData_NotAffectedByFog();
	CStringId	SID_LmbrAdditionalData_Backlighting();
	CStringId	SID_LmbrAdditionalData_EnvProbeLighting();
	CStringId	SID_LmbrAdditionalData_NearPlaneFadeDistance();
	CStringId	SID_LmbrAdditionalData_FarPlaneFadeDistance();
	CStringId	SID_LmbrAdditionalData_Tesselate();
	CStringId	SID_LmbrAdditionalData_DiffuseLighting();
	CStringId	SID_LmbrAdditionalData_SortDepthOffset();

	CStringId	SID_UVScroll();
	CStringId	SID_UVScroll_UVOffset();
	CStringId	SID_UVScroll_UVScale();
	CStringId	SID_UVRotate();
	CStringId	SID_UVRotate_UVAngle();
	CStringId	SID_LmbrDistortionMask();
	CStringId	SID_LmbrDistortionTiling_Tiling();
	CStringId	SID_LmbrDistortionTiling_MaskTiling();
	CStringId	SID_LmbrDistortionLit();
	CStringId	SID_LmbrDistortionLit_Roughness();
	CStringId	SID_LmbrDistortionLit_EnvProbeLighting();
	CStringId	SID_LmbrDistortionTint();
	CStringId	SID_LmbrDistortionTint_TintMap();
	CStringId	SID_LmbrDistortionTint_TintColor();
	CStringId	SID_LmbrForwardDistortion();

	CStringId	SID_LmbrMeshMaterialOverride();
	CStringId	SID_LmbrMeshMaterialOverride_MaterialPath();

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue1();
	CStringId	SID_LmbrMeshMaterialOverride_CustomValue1_Binding1();
	CStringId	SID_LmbrMeshMaterialOverride_CustomValue1_CustomValue1();

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue2();
	CStringId	SID_LmbrMeshMaterialOverride_CustomValue2_Binding2();
	CStringId	SID_LmbrMeshMaterialOverride_CustomValue2_CustomValue2();

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue3();
	CStringId	SID_LmbrMeshMaterialOverride_CustomValue3_Binding3();
	CStringId	SID_LmbrMeshMaterialOverride_CustomValue3_CustomValue3();

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue4();
	CStringId	SID_LmbrMeshMaterialOverride_CustomValue4_Binding4();
	CStringId	SID_LmbrMeshMaterialOverride_CustomValue4_CustomValue4();

	CStringId	SID_LmbrSound();
	CStringId	SID_LmbrSound_StartTrigger();
	CStringId	SID_LmbrSound_StopTrigger();
	CStringId	SID_LmbrSoundControl();
	CStringId	SID_LmbrSoundControl_RTPC();
	CStringId	SID_LmbrSoundControl_FXParam();

	void		Startup();
	void		Shutdown();
}

__LMBRPK_END

#endif //LMBR_USE_PK
