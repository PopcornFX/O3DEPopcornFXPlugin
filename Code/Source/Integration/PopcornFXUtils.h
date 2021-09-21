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

#include <PopcornFX/PopcornFXBus.h>

#include <AzCore/Math/Vector3.h>
#include <AzCore/Math/Vector4.h>
#include <AzCore/Math/Matrix4x4.h>
#include <AzCore/Math/Aabb.h>
#if !AZ_TRAIT_USE_PLATFORM_SIMD_SSE
	#include <AzCore/Math/VectorConversions.h>
#endif

#include <pk_maths/include/pk_maths.h>
#include <pk_maths/include/pk_maths_simd_vector.h>
#include <pk_maths/include/pk_maths_primitives.h>

#include <pk_kernel/include/kr_base_types.h>

__LMBRPK_BEGIN

	template <typename _OutType, typename _InType>
	PK_FORCEINLINE const _OutType		&_Reinterpret(const _InType &vec)
	{
		PK_STATIC_ASSERT(sizeof(_InType) == sizeof(_OutType));
		return *reinterpret_cast<const _OutType*>(&vec);
	}

	// Vector: ToAZ from PK
#if AZ_TRAIT_USE_PLATFORM_SIMD_SSE && (PK_SIMD == PK_SIMD_SSE)
	PK_FORCEINLINE const AZ::Vector4	&ToAZSIMD4(const SIMD::Float4 &vec) { return _Reinterpret<AZ::Vector4>(vec); }
	PK_FORCEINLINE const AZ::Vector3	&ToAZSIMD3(const SIMD::Float4 &vec) { return _Reinterpret<AZ::Vector3>(vec); }
	PK_FORCEINLINE const AZ::Vector2	&ToAZSIMD2(const SIMD::Float4 &vec) { return _Reinterpret<AZ::Vector2>(vec); }
#elif AZ_TRAIT_USE_PLATFORM_SIMD_NEON && (PK_SIMD == PK_SIMD_NEON)
	PK_FORCEINLINE const AZ::Vector4	&ToAZSIMD4(const SIMD::Float4 &vec) { return _Reinterpret<AZ::Vector4>(vec); }
	PK_FORCEINLINE const AZ::Vector3	&ToAZSIMD3(const SIMD::Float4 &vec) { return _Reinterpret<AZ::Vector3>(vec); }
	PK_FORCEINLINE const AZ::Vector2	ToAZSIMD2(const SIMD::Float4 &vec) { return AZ::Vector4ToVector2(_Reinterpret<AZ::Vector4>(vec)); }
#else //AZ_TRAIT_USE_PLATFORM_SIMD_SCALAR or AZ != PK
	PK_FORCEINLINE const AZ::Vector4	ToAZSIMD4(const SIMD::Float4 &vec) { return AZ::Vector4(vec.x().AsFloat(), vec.y().AsFloat(), vec.z().AsFloat(), vec.w().AsFloat()); }
	PK_FORCEINLINE const AZ::Vector3	ToAZSIMD3(const SIMD::Float4 &vec) { return AZ::Vector3(vec.x().AsFloat(), vec.y().AsFloat(), vec.z().AsFloat()); }
	PK_FORCEINLINE const AZ::Vector2	ToAZSIMD2(const SIMD::Float4 &vec) { return AZ::Vector2(vec.x().AsFloat(), vec.y().AsFloat()); }
#endif
	PK_FORCEINLINE AZ::Vector4			ToAZ(const CFloat4 &vec) { return ToAZSIMD4(vec); }
	PK_FORCEINLINE AZ::Vector3			ToAZ(const CFloat3 &vec) { return ToAZSIMD3(vec); }
	PK_FORCEINLINE AZ::Vector2			ToAZ(const CFloat2 &vec) { return ToAZSIMD2(vec); }

	// Vector: ToPk from AZ
#if AZ_TRAIT_USE_PLATFORM_SIMD_SSE && (PK_SIMD == PK_SIMD_SSE)
	PK_FORCEINLINE const SIMD::Float4	&ToPkSIMD(const AZ::Vector4 &vec) { return _Reinterpret<SIMD::Float4>(vec); }
	PK_FORCEINLINE const SIMD::Float4	&ToPkSIMD(const AZ::Vector3 &vec) { return _Reinterpret<SIMD::Float4>(vec); }
	PK_FORCEINLINE const SIMD::Float4	&ToPkSIMD(const AZ::Vector2 &vec) { return _Reinterpret<SIMD::Float4>(vec); }
#elif AZ_TRAIT_USE_PLATFORM_SIMD_NEON && (PK_SIMD == PK_SIMD_NEON)
	PK_FORCEINLINE const SIMD::Float4	&ToPkSIMD(const AZ::Vector4 &vec) { return _Reinterpret<SIMD::Float4>(vec); }
	PK_FORCEINLINE const SIMD::Float4	&ToPkSIMD(const AZ::Vector3 &vec) { return _Reinterpret<SIMD::Float4>(vec); }
	PK_FORCEINLINE const SIMD::Float4	ToPkSIMD(const AZ::Vector2 &vec) { return _Reinterpret<SIMD::Float4>(AZ::Vector2ToVector4(vec)); }
#else //AZ_TRAIT_USE_PLATFORM_SIMD_SCALAR or AZ != PK
	PK_FORCEINLINE const SIMD::Float4	ToPkSIMD(const AZ::Vector4 &vec) { return SIMD::Float4(vec.GetX(), vec.GetY(), vec.GetZ(), vec.GetW()); }
	PK_FORCEINLINE const SIMD::Float4	ToPkSIMD(const AZ::Vector3 &vec) { return SIMD::Float4(vec.GetX(), vec.GetY(), vec.GetZ()); }
	PK_FORCEINLINE const SIMD::Float4	ToPkSIMD(const AZ::Vector2 &vec) { return SIMD::Float4(vec.GetX(), vec.GetY(), 0.0f); }
#endif
	PK_FORCEINLINE CFloat4				ToPk(const AZ::Vector4 &vec) { return ToPkSIMD(vec).AsFloat4(); }
	PK_FORCEINLINE CFloat3				ToPk(const AZ::Vector3 &vec) { return ToPkSIMD(vec).AsFloat3(); }
	PK_FORCEINLINE CFloat2				ToPk(const AZ::Vector2 &vec) { return ToPkSIMD(vec).AsFloat3().xy(); }

	// Matrix, Quaternion, Aabb: ToPk from AZ
	PK_FORCEINLINE CFloat4x4			ToPk(const AZ::Matrix4x4& mat) { CFloat4x4 ret; mat.StoreToColumnMajorFloat16(&ret.XAxis().x()); return ret; }
	PK_FORCEINLINE CFloat4				ToPk(const AZ::Quaternion &quat) { CFloat4 ret; quat.StoreToFloat4(&ret.x()); return ret; }
	PK_FORCEINLINE CAABB				ToPk(const AZ::Aabb &aabb) { return CAABB(ToPk(aabb.GetMin()), ToPk(aabb.GetMax())); }


	PK_FORCEINLINE EPopcornFXType	BaseTypeToLmbrPopcornFXType(const EBaseTypeID type)
	{
		switch (type)
		{
		case	PopcornFX::EBaseTypeID::BaseType_Bool:
			return EPopcornFXType::Type_Bool;
		case	PopcornFX::EBaseTypeID::BaseType_Bool2:
			return EPopcornFXType::Type_Bool2;
		case	PopcornFX::EBaseTypeID::BaseType_Bool3:
			return EPopcornFXType::Type_Bool3;
		case	PopcornFX::EBaseTypeID::BaseType_Bool4:
			return EPopcornFXType::Type_Bool4;
		case	PopcornFX::EBaseTypeID::BaseType_U32:
		case	PopcornFX::EBaseTypeID::BaseType_I32:
		case	PopcornFX::EBaseTypeID::BaseType_U64:
		case	PopcornFX::EBaseTypeID::BaseType_I64:
			return EPopcornFXType::Type_Int;
		case	PopcornFX::EBaseTypeID::BaseType_UInt2:
		case	PopcornFX::EBaseTypeID::BaseType_Int2:
			return EPopcornFXType::Type_Int2;
		case	PopcornFX::EBaseTypeID::BaseType_UInt3:
		case	PopcornFX::EBaseTypeID::BaseType_Int3:
			return EPopcornFXType::Type_Int3;
		case	PopcornFX::EBaseTypeID::BaseType_UInt4:
		case	PopcornFX::EBaseTypeID::BaseType_Int4:
			return EPopcornFXType::Type_Int4;
		case	PopcornFX::EBaseTypeID::BaseType_Float:
		case	PopcornFX::EBaseTypeID::BaseType_Double:
			return EPopcornFXType::Type_Float;
		case	PopcornFX::EBaseTypeID::BaseType_Float2:
			return EPopcornFXType::Type_Float2;
		case	PopcornFX::EBaseTypeID::BaseType_Float3:
			return EPopcornFXType::Type_Float3;
		case	PopcornFX::EBaseTypeID::BaseType_Float4:
		case	PopcornFX::EBaseTypeID::BaseType_Quaternion:
			return EPopcornFXType::Type_Float4;
		default:
			PK_ASSERT_NOT_REACHED();
			return EPopcornFXType::Type_Unknown;
		}
	}

	PK_FORCEINLINE AZStd::string	LmbrPopcornFXTypeToString(const EPopcornFXType type)
	{
		switch (type)
		{
		case Type_Bool:
			return "Bool";
		case Type_Bool2:
			return "Bool2";
		case Type_Bool3:
			return "Bool3";
		case Type_Bool4:
			return "Bool4";
		case Type_Float:
			return "Float";
		case Type_Float2:
			return "Float2";
		case Type_Float3:
			return "Float3";
		case Type_Float4:
			return "Float4";
		case Type_Int:
			return "Int";
		case Type_Int2:
			return "Int2";
		case Type_Int3:
			return "Int3";
		case Type_Int4:
			return "Int4";
		case Type_Quaternion:
			return "Quaternion";
		default:
			return "Unknown";
			break;
		}
	}

__LMBRPK_END

#endif //LMBR_USE_PK
