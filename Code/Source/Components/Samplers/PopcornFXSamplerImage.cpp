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

#include "PopcornFXSamplerImage.h"

#if defined(LMBR_USE_PK)
#include <pk_particles/include/ps_samplers_classes.h>
#include <pk_kernel/include/kr_refcounted_buffer.h>
#endif //LMBR_USE_PK

__LMBRPK_BEGIN

	void PopcornFXSamplerImage::Reflect(AZ::ReflectContext* context)
	{
		AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);
		if (serializeContext)
		{
			serializeContext->Class<PopcornFXSamplerImage>()
				->Version(1, &VersionConverter)
				->Field("Texture", &PopcornFXSamplerImage::m_Texture)
				->Field("TexcoordMode", &PopcornFXSamplerImage::m_TexcoordMode)
				;

			// edit context:
			if (AZ::EditContext* editContext = serializeContext->GetEditContext())
			{
				editContext->Class<PopcornFXSamplerImage>("PopcornFX Sampler Image", "")
					->DataElement(0, &PopcornFXSamplerImage::m_Texture, "Texture", "")
						->Attribute(AZ::Edit::Attributes::ChangeNotify, &PopcornFXSamplerImage::_OnDataChanged)
					->DataElement("ComboBox", &PopcornFXSamplerImage::m_TexcoordMode, "Texcoord Mode", "")
						->Attribute(AZ::Edit::Attributes::ChangeNotify, &PopcornFXSamplerImage::_OnTexcoordModeChanged)
						->EnumAttribute(PopcornFXSamplerImage::TexcoordMode::Clamp, "Clamp")
						->EnumAttribute(PopcornFXSamplerImage::TexcoordMode::Wrap, "Wrap")
					;
			}
		}
	}

	// Private Static
	bool PopcornFXSamplerImage::VersionConverter(	AZ::SerializeContext& context,
													AZ::SerializeContext::DataElementNode& classElement)
	{
		(void)context; (void)classElement;
		return true;
	}

	void	PopcornFXSamplerImage::CopyFrom(const PopcornFXSamplerImage &other)
	{
		m_Texture = other.m_Texture;
		m_TexcoordMode = other.m_TexcoordMode;
	}

#if 1//!defined(LMBR_USE_PK)

	void	PopcornFXSamplerImage::Activate()
	{
	}

	void	PopcornFXSamplerImage::Deactivate()
	{
	}

	AZ::u32	PopcornFXSamplerImage::_OnDataChanged()
	{
		return AZ::Edit::PropertyRefreshLevels::None;
	}

	void	PopcornFXSamplerImage::_OnTexcoordModeChanged()
	{
	}

#else

	void	PopcornFXSamplerImage::Activate()
	{
		PopcornFXSamplerComponentRequestBus::Handler::BusConnect(m_AttachedToEntityId);
		_LoadTexture();
	}

	void	PopcornFXSamplerImage::Deactivate()
	{
		PopcornFXSamplerComponentRequestBus::Handler::BusDisconnect(m_AttachedToEntityId);
		if (AZ::TickBus::Handler::BusIsConnected())
			AZ::TickBus::Handler::BusDisconnect();
		_Clean();
	}

	AZ::u32	PopcornFXSamplerImage::_OnDataChanged()
	{
		_LoadTexture();
		return AZ::Edit::PropertyRefreshLevels::ValuesOnly;
	}

	void	PopcornFXSamplerImage::_OnTexcoordModeChanged()
	{
		const CImageSamplerInterface::ETexcoordMode	texcoordMode =	m_TexcoordMode == TexcoordMode::Clamp ?
																	CImageSamplerInterface::TexcoordMode_Clamp :
																	CImageSamplerInterface::TexcoordMode_Wrap;

		PParticleSamplerDescriptor_Image	desc = reinterpret_cast<CParticleSamplerDescriptor_Image *>(m_SamplerDescriptor.Get());
		if (desc == null)
			return;
#if 0 //TODO_V2
		desc->m_DefaultTCMode = texcoordMode;
#endif
	}

	void	PopcornFXSamplerImage::OnTick(float deltaTime, AZ::ScriptTimePoint time)
	{
		(void)deltaTime; (void)time;

		if (m_LoadTexturePending)
			_LoadTexture();
	}

	static CImage::EFormat	ToPk(ETEX_Format format)
	{
		switch (format)
		{
			//case eTF_R8G8B8A8S:
			//	break;
			//case eTF_R8G8B8A8:
			//	break;
			//case eTF_A8:
			//	break;
			//case eTF_R8:
			//	break;
			//case eTF_R8S:
			//	break;
			//case eTF_R16:
			//	break;
		case eTF_R16F:
			return CImage::Format_Fp16Lum;
		case eTF_R32F:
			return CImage::Format_Fp32Lum;
			//	break;
			//case eTF_R8G8:
			//	break;
			//case eTF_R8G8S:
			//	break;
			//case eTF_R16G16:
			//	break;
			//case eTF_R16G16S:
			//	break;
			//case eTF_R16G16F:
			//	break;
			//case eTF_R11G11B10F:
			//	break;
			//case eTF_R10G10B10A2:
			//	break;
			//case eTF_R16G16B16A16:
			//	break;
			//case eTF_R16G16B16A16S:
			//	break;
			//case eTF_R16G16B16A16F:
			//	break;
			//case eTF_R32G32B32A32F:
			//	break;
		case eTF_CTX1:
			return CImage::Format_DXT1;
		case eTF_BC1:
			return CImage::Format_DXT1;
		case eTF_BC2:
			return CImage::Format_DXT3;
		case eTF_BC3:
			return CImage::Format_DXT5;
			//case eTF_BC4U:
			//	break;
			//case eTF_BC4S:
			//	break;
		case eTF_BC5U:
			return CImage::Format_BC5_UNorm;
		case eTF_BC5S:
			return CImage::Format_BC5_SNorm;
			//case eTF_BC6UH:
			//	break;
			//case eTF_BC6SH:
			//	break;
			//case eTF_BC7:
			//	break;
			//case eTF_R9G9B9E5:
			//	break;
			//case eTF_D16:
			//	break;
			//case eTF_D24S8:
			//	break;
			//case eTF_D32F:
			//	break;
			//case eTF_D32FS8:
			//	break;
			//case eTF_B5G6R5:
			//	break;
			//case eTF_B5G5R5:
			//	break;
			//case eTF_B4G4R4A4:
			//	break;
			//case eTF_EAC_R11:
			//	break;
			//case eTF_EAC_RG11:
			//	break;
		case eTF_ETC2:
			return CImage::Format_RGBA8_ETC2;
		case eTF_ETC2A:
			return CImage::Format_RGB8A1_ETC2;
			//case eTF_A8L8:
			//	return Format_LumAlpha8;
		case eTF_L8:
			return CImage::Format_Lum8;
			//case eTF_L8V8U8:
			//	break;
		case eTF_B8G8R8:
			return CImage::Format_BGR8;
			//case eTF_L8V8U8X8:
			//	break;
		case eTF_B8G8R8X8:
			return CImage::Format_BGRA8;
		case eTF_B8G8R8A8:
			return CImage::Format_BGRA8;
			//case eTF_PVRTC2:
			//	break;
			//case eTF_PVRTC4:
			//	break;
			//case eTF_ASTC_4x4:
			//	break;
			//case eTF_ASTC_5x4:
			//	break;
			//case eTF_ASTC_5x5:
			//	break;
			//case eTF_ASTC_6x5:
			//	break;
			//case eTF_ASTC_6x6:
			//	break;
			//case eTF_ASTC_8x5:
			//	break;
			//case eTF_ASTC_8x6:
			//	break;
			//case eTF_ASTC_8x8:
			//	break;
			//case eTF_ASTC_10x5:
			//	break;
			//case eTF_ASTC_10x6:
			//	break;
			//case eTF_ASTC_10x8:
			//	break;
			//case eTF_ASTC_10x10:
			//	break;
			//case eTF_ASTC_12x10:
			//	break;
			//case eTF_ASTC_12x12:
			//	break;
			//case eTF_R16U:
			//	break;
			//case eTF_R16G16U:
			//	break;
			//case eTF_R10G10B10A2UI:
			//	break;
			//case eTF_MaxFormat:
			//	break;
		default:
			break;
		}
		return CImage::Format_Invalid;
	}

	void	PopcornFXSamplerImage::_LoadTexture()
	{
#if 1
		AZ_Error("PopcornFX", false, "Texture loading not implemented");
#else
		// load the texture file
		uint32		loadTextureFlags = (FT_IGNORE_PRECACHE | FT_USAGE_READBACK | FT_NOMIPS | FT_DONT_STREAM);
		ITexture	*texture = gEnv->pRenderer->EF_LoadTexture(m_Texture.GetAssetPath().c_str(), loadTextureFlags);

		if (!PK_VERIFY(texture != null))
		{
			AZ_Error("PopcornFX", false, "Failed to find texture %s", m_Texture.GetAssetPath().c_str());
			return;
		}

		if (!texture->IsTextureLoaded() || texture->IsPostponed())
		{
			if (!AZ::TickBus::Handler::BusIsConnected())
				AZ::TickBus::Handler::BusConnect();
			m_LoadTexturePending = true;
			return;
		}
		else
		{
			if (AZ::TickBus::Handler::BusIsConnected())
				AZ::TickBus::Handler::BusDisconnect();
			m_LoadTexturePending = false;
		}

		if (!PK_VERIFY(texture->GetFlags() & FT_USAGE_READBACK))
		{
			AZ_Error("PopcornFX", false, "Failed to read in texture %s", m_Texture.GetAssetPath().c_str());
			return;
		}

		if (!PK_VERIFY(_BuildDescriptor(texture)))
		{
			AZ_Error("PopcornFX", false, "Build descriptor failed for Sampler Image");
		}
#endif
	}

	bool	PopcornFXSamplerImage::_BuildDescriptor(ITexture *texture)
	{
		const CImageSamplerInterface::ETexcoordMode	texcoordMode =	m_TexcoordMode == TexcoordMode::Clamp ?
																	CImageSamplerInterface::TexcoordMode_Clamp :
																	CImageSamplerInterface::TexcoordMode_Wrap;

		if (m_SamplerDescriptor == null)
		{
			m_SamplerDescriptor = PK_NEW(CParticleSamplerDescriptor_Image_Default());
			if (!PK_VERIFY(m_SamplerDescriptor != null))
				return false;
		}
		PParticleSamplerDescriptor_Image_Default	desc = reinterpret_cast<CParticleSamplerDescriptor_Image_Default *>(m_SamplerDescriptor.Get());
		if (!PK_VERIFY(desc != null))
			return false;

#if 0 //TODO_V2
		desc->m_DefaultTCMode = texcoordMode;
#endif
		if (m_ImageSampler == null)
		{
			//if (desc->m_DefaultFilter == CImageSamplerInterface::FilterType_Linear)
			m_ImageSampler = PK_NEW(CImageSamplerBilinear);
#if 0 //TODO_V2
			else if (desc->m_DefaultFilter == CImageSamplerInterface::FilterType_Point)
				m_ImageSampler = PK_NEW(CImageSampler);
			else
				PK_ASSERT_NOT_REACHED();
#endif
		}
		if (!PK_VERIFY(m_ImageSampler != null))
			return false;

		const u32				width = texture->GetWidth();
		const u32				height = texture->GetHeight();
		const u32				sizeInBytes = texture->GetDataSize();// TextureDataSize(width, height, 1, texture->GetTextureSrcFormat());
		const CImage::EFormat	pkFormat = ToPk(texture->GetTextureSrcFormat());

		if (pkFormat == CImage::Format_Invalid)
		{
			AZ_Warning("PopcornFX", false, "Texture format not recognized by PopcornFX");
			texture->Release();
			return false;
		}

		if (sizeInBytes <= 0)
		{
			texture->Release();
			return false;
		}

		CImageMap	map;
		map.m_RawBuffer = CRefCountedMemoryBuffer::AllocAligned(sizeInBytes, 0x80);
		if (map.m_RawBuffer != null)
		{
			u8	*dst = map.m_RawBuffer->Data<u8>();
			texture->Readback(0, [&](void* data, uint32 rowPitch, uint32 slicePitch)
			{
				(void)rowPitch; (void)slicePitch;
				memcpy(dst, data, sizeInBytes);
				return true;
			});
			texture->Release();
			map.m_Dimensions = CUint3(width, height, 1);
		}

		desc->m_ImageDimensions = map.m_Dimensions.xy();

		CImageSurface	surface(map, pkFormat);

		if (m_ImageSampler->SetupFromSurface(surface))
		{
			// set image sampler
			desc->m_Sampler = m_ImageSampler;
		}
		else
		{
			surface.Convert(CImage::Format_BGRA8);
			if (m_ImageSampler->SetupFromSurface(surface))
			{
				// set image sampler
				desc->m_Sampler = m_ImageSampler;
			}
			else
				return false;
		}
		PopcornFXSamplerComponentEventsBus::Event(m_AttachedToEntityId, &PopcornFXSamplerComponentEventsBus::Events::OnSamplerReady, m_AttachedToEntityId);

		return true;
	}

	void	PopcornFXSamplerImage::_Clean()
	{
		if (m_SamplerDescriptor != null)
		{
			PParticleSamplerDescriptor_Image_Default	desc = reinterpret_cast<CParticleSamplerDescriptor_Image_Default *>(m_SamplerDescriptor.Get());
			if (PK_VERIFY(desc != null))
			{
				desc->m_Sampler = null;
			}
			m_SamplerDescriptor = null;
		}

		if (m_ImageSampler != null)
		{
			PK_DELETE(m_ImageSampler);
			m_ImageSampler = null;
		}
	}
#endif //LMBR_USE_PK

__LMBRPK_END
