//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include "PopcornFX_precompiled.h"
#include "AdditionalRendererProperties.h"

#if defined(LMBR_USE_PK)

//----------------------------------------------------------------------------
__LMBRPK_BEGIN

namespace	AdditionalRendererProperties
{
	CStringId	g_SID_LmbrAdditionalData;
	CStringId	g_SID_LmbrAdditionalData_NotAffectedByFog;
	CStringId	g_SID_LmbrAdditionalData_Backlighting;
	CStringId	g_SID_LmbrAdditionalData_EnvProbeLighting;
	CStringId	g_SID_LmbrAdditionalData_NearPlaneFadeDistance;
	CStringId	g_SID_LmbrAdditionalData_FarPlaneFadeDistance;
	CStringId	g_SID_LmbrAdditionalData_Tesselate;
	CStringId	g_SID_LmbrAdditionalData_DiffuseLighting;
	CStringId	g_SID_LmbrAdditionalData_SortDepthOffset;
	CStringId	g_SID_UVScroll;
	CStringId	g_SID_UVScroll_UVOffset;
	CStringId	g_SID_UVScroll_UVScale;
	CStringId	g_SID_UVRotate;
	CStringId	g_SID_UVRotate_UVAngle;
	CStringId	g_SID_LmbrDistortionMask;
	CStringId	g_SID_LmbrDistortionTiling_Tiling;
	CStringId	g_SID_LmbrDistortionTiling_MaskTiling;
	CStringId	g_SID_LmbrDistortionLit;
	CStringId	g_SID_LmbrDistortionLit_Roughness;
	CStringId	g_SID_LmbrDistortionLit_EnvProbeLighting;
	CStringId	g_SID_LmbrDistortionTint;
	CStringId	g_SID_LmbrDistortionTint_TintMap;
	CStringId	g_SID_LmbrDistortionTint_TintColor;
	CStringId	g_SID_LmbrForwardDistortion;
	CStringId	g_SID_LmbrMeshMaterialOverride;
	CStringId	g_SID_LmbrMeshMaterialOverride_MaterialPath;
	CStringId	g_SID_LmbrMeshMaterialOverride_CustomValue1;
	CStringId	g_SID_LmbrMeshMaterialOverride_CustomValue1_Binding1;
	CStringId	g_SID_LmbrMeshMaterialOverride_CustomValue1_CustomValue1;
	CStringId	g_SID_LmbrMeshMaterialOverride_CustomValue2;
	CStringId	g_SID_LmbrMeshMaterialOverride_CustomValue2_Binding2;
	CStringId	g_SID_LmbrMeshMaterialOverride_CustomValue2_CustomValue2;
	CStringId	g_SID_LmbrMeshMaterialOverride_CustomValue3;
	CStringId	g_SID_LmbrMeshMaterialOverride_CustomValue3_Binding3;
	CStringId	g_SID_LmbrMeshMaterialOverride_CustomValue3_CustomValue3;
	CStringId	g_SID_LmbrMeshMaterialOverride_CustomValue4;
	CStringId	g_SID_LmbrMeshMaterialOverride_CustomValue4_Binding4;
	CStringId	g_SID_LmbrMeshMaterialOverride_CustomValue4_CustomValue4;
	CStringId	g_SID_LmbrSound;
	CStringId	g_SID_LmbrSound_StartTrigger;
	CStringId	g_SID_LmbrSound_StopTrigger;
	CStringId	g_SID_LmbrSoundControl;
	CStringId	g_SID_LmbrSoundControl_RTPC;
	CStringId	g_SID_LmbrSoundControl_FXParam;

	CStringId	SID_LmbrAdditionalData()
	{
		return g_SID_LmbrAdditionalData;
	}

	CStringId	SID_LmbrAdditionalData_NotAffectedByFog()
	{
		return g_SID_LmbrAdditionalData_NotAffectedByFog;
	}

	CStringId	SID_LmbrAdditionalData_Backlighting()
	{
		return g_SID_LmbrAdditionalData_Backlighting;
	}

	CStringId	SID_LmbrAdditionalData_EnvProbeLighting()
	{
		return g_SID_LmbrAdditionalData_EnvProbeLighting;
	}

	CStringId	SID_LmbrAdditionalData_NearPlaneFadeDistance()
	{
		return g_SID_LmbrAdditionalData_NearPlaneFadeDistance;
	}

	CStringId	SID_LmbrAdditionalData_FarPlaneFadeDistance()
	{
		return g_SID_LmbrAdditionalData_FarPlaneFadeDistance;
	}

	CStringId	SID_LmbrAdditionalData_Tesselate()
	{
		return g_SID_LmbrAdditionalData_Tesselate;
	}

	CStringId	SID_LmbrAdditionalData_DiffuseLighting()
	{
		return g_SID_LmbrAdditionalData_DiffuseLighting;
	}

	CStringId	SID_LmbrAdditionalData_SortDepthOffset()
	{
		return g_SID_LmbrAdditionalData_SortDepthOffset;
	}

	CStringId	SID_UVScroll()
	{
		return g_SID_UVScroll;
	}

	CStringId	SID_UVScroll_UVOffset()
	{
		return g_SID_UVScroll_UVOffset;
	}

	CStringId	SID_UVScroll_UVScale()
	{
		return g_SID_UVScroll_UVScale;
	}

	CStringId	SID_UVRotate()
	{
		return g_SID_UVRotate;
	}

	CStringId	SID_UVRotate_UVAngle()
	{
		return g_SID_UVRotate_UVAngle;
	}

	CStringId	SID_LmbrDistortionMask()
	{
		return g_SID_LmbrDistortionMask;
	}

	CStringId	SID_LmbrDistortionTiling_Tiling()
	{
		return g_SID_LmbrDistortionTiling_Tiling;
	}

	CStringId	SID_LmbrDistortionTiling_MaskTiling()
	{
		return g_SID_LmbrDistortionTiling_MaskTiling;
	}

	CStringId	SID_LmbrDistortionLit()
	{
		return g_SID_LmbrDistortionLit;
	}

	CStringId	SID_LmbrDistortionLit_Roughness()
	{
		return g_SID_LmbrDistortionLit_Roughness;
	}

	CStringId	SID_LmbrDistortionLit_EnvProbeLighting()
	{
		return g_SID_LmbrDistortionLit_EnvProbeLighting;
	}

	CStringId	SID_LmbrDistortionTint()
	{
		return g_SID_LmbrDistortionTint;
	}

	CStringId	SID_LmbrDistortionTint_TintMap()
	{
		return g_SID_LmbrDistortionTint_TintMap;
	}

	CStringId	SID_LmbrDistortionTint_TintColor()
	{
		return g_SID_LmbrDistortionTint_TintColor;
	}

	CStringId	SID_LmbrForwardDistortion()
	{
		return g_SID_LmbrForwardDistortion;
	}

	CStringId	SID_LmbrMeshMaterialOverride()
	{
		return g_SID_LmbrMeshMaterialOverride;
	}

	CStringId	SID_LmbrMeshMaterialOverride_MaterialPath()
	{
		return g_SID_LmbrMeshMaterialOverride_MaterialPath;
	}

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue1()
	{
		return g_SID_LmbrMeshMaterialOverride_CustomValue1;
	}

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue1_Binding1()
	{
		return g_SID_LmbrMeshMaterialOverride_CustomValue1_Binding1; 
	}

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue1_CustomValue1()
	{
		return g_SID_LmbrMeshMaterialOverride_CustomValue1_CustomValue1; 
	}

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue2()
	{
		return g_SID_LmbrMeshMaterialOverride_CustomValue2; 
	}

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue2_Binding2()
	{
		return g_SID_LmbrMeshMaterialOverride_CustomValue2_Binding2; 
	}

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue2_CustomValue2()
	{
		return g_SID_LmbrMeshMaterialOverride_CustomValue2_CustomValue2; 
	}

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue3()
	{
		return g_SID_LmbrMeshMaterialOverride_CustomValue3; 
	}

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue3_Binding3()
	{
		return g_SID_LmbrMeshMaterialOverride_CustomValue3_Binding3; 
	}

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue3_CustomValue3()
	{
		return g_SID_LmbrMeshMaterialOverride_CustomValue3_CustomValue3; 
	}

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue4()
	{
		return g_SID_LmbrMeshMaterialOverride_CustomValue4; 
	}

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue4_Binding4()
	{
		return g_SID_LmbrMeshMaterialOverride_CustomValue4_Binding4; 
	}

	CStringId	SID_LmbrMeshMaterialOverride_CustomValue4_CustomValue4()
	{
		return g_SID_LmbrMeshMaterialOverride_CustomValue4_CustomValue4; 
	}

	CStringId	SID_LmbrSound()
	{
		return g_SID_LmbrSound;
	}

	CStringId	SID_LmbrSound_StartTrigger()
	{
		return g_SID_LmbrSound_StartTrigger;
	}

	CStringId	SID_LmbrSound_StopTrigger()
	{
		return g_SID_LmbrSound_StopTrigger;
	}

	CStringId	SID_LmbrSoundControl()
	{
		return g_SID_LmbrSoundControl;
	}

	CStringId	SID_LmbrSoundControl_RTPC()
	{
		return g_SID_LmbrSoundControl_RTPC;
	}

	CStringId	SID_LmbrSoundControl_FXParam()
	{
		return g_SID_LmbrSoundControl_FXParam;
	}

	void		Startup()
	{
		g_SID_LmbrAdditionalData.Reset("LumberyardAdditionalData");
		g_SID_LmbrAdditionalData_NotAffectedByFog.Reset("LumberyardAdditionalData.NotAffectedByFog");
		g_SID_LmbrAdditionalData_Backlighting.Reset("LumberyardAdditionalData.Backlighting");
		g_SID_LmbrAdditionalData_EnvProbeLighting.Reset("LumberyardAdditionalData.EnvProbeLighting");
		g_SID_LmbrAdditionalData_NearPlaneFadeDistance.Reset("LumberyardAdditionalData.NearPlaneFadeDistance");
		g_SID_LmbrAdditionalData_FarPlaneFadeDistance.Reset("LumberyardAdditionalData.FarPlaneFadeDistance");
		g_SID_LmbrAdditionalData_Tesselate.Reset("LumberyardAdditionalData.Tesselate");
		g_SID_LmbrAdditionalData_DiffuseLighting.Reset("LumberyardAdditionalData.DiffuseLighting");
		g_SID_LmbrAdditionalData_SortDepthOffset.Reset("LumberyardAdditionalData.SortDepthOffset");

		g_SID_UVScroll.Reset("UVScroll");
		g_SID_UVScroll_UVOffset.Reset("UVScroll.UVOffset");
		g_SID_UVScroll_UVScale.Reset("UVScroll.UVScale");
		g_SID_UVRotate.Reset("UVRotate");
		g_SID_UVRotate_UVAngle.Reset("UVRotate.UVAngle");
		g_SID_LmbrDistortionMask.Reset("LumberyardDistortionMask.Mask");
		g_SID_LmbrDistortionTiling_Tiling.Reset("LumberyardDistortionTiling.Tiling");
		g_SID_LmbrDistortionTiling_MaskTiling.Reset("LumberyardDistortionTiling.MaskTiling");
		g_SID_LmbrDistortionLit.Reset("LumberyardDistortionLit");
		g_SID_LmbrDistortionLit_Roughness.Reset("LumberyardDistortionLit.Roughness");
		g_SID_LmbrDistortionLit_EnvProbeLighting.Reset("LumberyardDistortionLit.EnvProbeLighting");
		g_SID_LmbrDistortionTint.Reset("LumberyardDistortionTint");
		g_SID_LmbrDistortionTint_TintMap.Reset("LumberyardDistortionTint.TintMap");
		g_SID_LmbrDistortionTint_TintColor.Reset("LumberyardDistortionTint.TintColor");
		g_SID_LmbrForwardDistortion.Reset("LumberyardForwardDistortion");

		g_SID_LmbrMeshMaterialOverride.Reset("LumberyardMeshMaterialOverride");
		g_SID_LmbrMeshMaterialOverride_MaterialPath.Reset("LumberyardMeshMaterialOverride.MaterialPath");

		g_SID_LmbrMeshMaterialOverride_CustomValue1.Reset("LumberyardMeshMaterialOverride_CustomValue1");
		g_SID_LmbrMeshMaterialOverride_CustomValue1_Binding1.Reset("LumberyardMeshMaterialOverride_CustomValue1.Binding1");
		g_SID_LmbrMeshMaterialOverride_CustomValue1_CustomValue1.Reset("LumberyardMeshMaterialOverride_CustomValue1.CustomValue1");
		g_SID_LmbrMeshMaterialOverride_CustomValue2.Reset("LumberyardMeshMaterialOverride_CustomValue2");
		g_SID_LmbrMeshMaterialOverride_CustomValue2_Binding2.Reset("LumberyardMeshMaterialOverride_CustomValue2.Binding2");
		g_SID_LmbrMeshMaterialOverride_CustomValue2_CustomValue2.Reset("LumberyardMeshMaterialOverride_CustomValue2.CustomValue2");
		g_SID_LmbrMeshMaterialOverride_CustomValue3.Reset("LumberyardMeshMaterialOverride_CustomValue3");
		g_SID_LmbrMeshMaterialOverride_CustomValue3_Binding3.Reset("LumberyardMeshMaterialOverride_CustomValue3.Binding3");
		g_SID_LmbrMeshMaterialOverride_CustomValue3_CustomValue3.Reset("LumberyardMeshMaterialOverride_CustomValue3.CustomValue3");
		g_SID_LmbrMeshMaterialOverride_CustomValue4.Reset("LumberyardMeshMaterialOverride_CustomValue4");
		g_SID_LmbrMeshMaterialOverride_CustomValue4_Binding4.Reset("LumberyardMeshMaterialOverride_CustomValue4.Binding4");
		g_SID_LmbrMeshMaterialOverride_CustomValue4_CustomValue4.Reset("LumberyardMeshMaterialOverride_CustomValue4.CustomValue4");

		g_SID_LmbrSound.Reset("LumberyardSound");
		g_SID_LmbrSound_StartTrigger.Reset("LumberyardSound.StartTrigger");
		g_SID_LmbrSound_StopTrigger.Reset("LumberyardSound.StopTrigger");
		g_SID_LmbrSoundControl.Reset("LumberyardSoundControl");
		g_SID_LmbrSoundControl_RTPC.Reset("LumberyardSoundControl.RTPC");
		g_SID_LmbrSoundControl_FXParam.Reset("LumberyardSoundControl.FXParam");
	}

	void		Shutdown()
	{
		g_SID_LmbrAdditionalData.Clear();
		g_SID_LmbrAdditionalData_NotAffectedByFog.Clear();
		g_SID_LmbrAdditionalData_Backlighting.Clear();
		g_SID_LmbrAdditionalData_EnvProbeLighting.Clear();
		g_SID_LmbrAdditionalData_NearPlaneFadeDistance.Clear();
		g_SID_LmbrAdditionalData_FarPlaneFadeDistance.Clear();
		g_SID_LmbrAdditionalData_Tesselate.Clear();
		g_SID_LmbrAdditionalData_DiffuseLighting.Clear();
		g_SID_LmbrAdditionalData_SortDepthOffset.Clear();

		g_SID_UVScroll.Clear();
		g_SID_UVScroll_UVOffset.Clear();
		g_SID_UVScroll_UVScale.Clear();
		g_SID_UVRotate.Clear();
		g_SID_UVRotate_UVAngle.Clear();
		g_SID_LmbrDistortionMask.Clear();
		g_SID_LmbrDistortionTiling_Tiling.Clear();
		g_SID_LmbrDistortionTiling_MaskTiling.Clear();
		g_SID_LmbrDistortionLit.Clear();
		g_SID_LmbrDistortionLit_Roughness.Clear();
		g_SID_LmbrDistortionLit_EnvProbeLighting.Clear();
		g_SID_LmbrDistortionTint.Clear();
		g_SID_LmbrDistortionTint_TintMap.Clear();
		g_SID_LmbrDistortionTint_TintColor.Clear();
		g_SID_LmbrForwardDistortion.Clear();

		g_SID_LmbrMeshMaterialOverride.Clear();
		g_SID_LmbrMeshMaterialOverride_MaterialPath.Clear();

		g_SID_LmbrMeshMaterialOverride_CustomValue1.Clear();
		g_SID_LmbrMeshMaterialOverride_CustomValue1_Binding1.Clear();
		g_SID_LmbrMeshMaterialOverride_CustomValue1_CustomValue1.Clear();
		g_SID_LmbrMeshMaterialOverride_CustomValue2.Clear();
		g_SID_LmbrMeshMaterialOverride_CustomValue2_Binding2.Clear();
		g_SID_LmbrMeshMaterialOverride_CustomValue2_CustomValue2.Clear();
		g_SID_LmbrMeshMaterialOverride_CustomValue3.Clear();
		g_SID_LmbrMeshMaterialOverride_CustomValue3_Binding3.Clear();
		g_SID_LmbrMeshMaterialOverride_CustomValue3_CustomValue3.Clear();
		g_SID_LmbrMeshMaterialOverride_CustomValue4.Clear();
		g_SID_LmbrMeshMaterialOverride_CustomValue4_Binding4.Clear();
		g_SID_LmbrMeshMaterialOverride_CustomValue4_CustomValue4.Clear();

		g_SID_LmbrSound.Clear();
		g_SID_LmbrSound_StartTrigger.Clear();
		g_SID_LmbrSound_StopTrigger.Clear();
		g_SID_LmbrSoundControl.Clear();
		g_SID_LmbrSoundControl_RTPC.Clear();
		g_SID_LmbrSoundControl_FXParam.Clear();
	}
}

__LMBRPK_END
//----------------------------------------------------------------------------

#endif //LMBR_USE_PK
