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
#include "Integration/ResourceHandlers/ImageResourceHandler.h"

#include <AzCore/Asset/AssetSerializer.h>

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
				;

			// edit context:
			if (AZ::EditContext* editContext = serializeContext->GetEditContext())
			{
				editContext->Class<PopcornFXSamplerImage>("PopcornFX Sampler Image", "")
					->DataElement(0, &PopcornFXSamplerImage::m_Texture, "Texture", "")
						->Attribute(AZ::Edit::Attributes::ChangeNotify, &PopcornFXSamplerImage::_OnDataChanged)
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
	}

#if !defined(LMBR_USE_PK)

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

	void	PopcornFXSamplerImage::OnTick(float deltaTime, AZ::ScriptTimePoint time)
	{
		(void)deltaTime; (void)time;

		if (m_LoadTexturePending)
			_LoadTexture();
	}

	void	PopcornFXSamplerImage::_LoadTexture()
	{
		if (!m_Texture.GetId().IsValid())
		{
			_Clean();
			// Disconnect from the tick bus, just in case
			if (!AZ::TickBus::Handler::BusIsConnected())
				AZ::TickBus::Handler::BusDisconnect();
			m_LoadTexturePending = false;
			return;
		}

		if (!m_Texture.IsReady())
		{
			m_Texture.QueueLoad();
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

		const AZ::RHI::ImageDescriptor	&imgDesc = m_Texture->GetImageDescriptor();
		if (!PK_VERIFY(_BuildDescriptor(imgDesc)))
		{
			AZ_Error("PopcornFX", false, "Build descriptor failed for Sampler Image");
		}
	}

	bool	PopcornFXSamplerImage::_BuildDescriptor(const AZ::RHI::ImageDescriptor &imgDesc)
	{
		PK_ASSERT(m_Texture.IsReady());
		if (m_SamplerDescriptor == null)
		{
			m_SamplerDescriptor = PK_NEW(CParticleSamplerDescriptor_Image_Default());
			if (!PK_VERIFY(m_SamplerDescriptor != null))
				return false;
		}
		PParticleSamplerDescriptor_Image_Default	desc = reinterpret_cast<CParticleSamplerDescriptor_Image_Default *>(m_SamplerDescriptor.Get());
		if (!PK_VERIFY(desc != null))
			return false;

		if (m_ImageSampler == null)
			m_ImageSampler = PK_NEW(CImageSamplerBilinear);
		if (!PK_VERIFY(m_ImageSampler != null))
			return false;

		const AZStd::array_view<uint8_t>	imgData = m_Texture->GetSubImageData(0, 0);
		const AZ::RHI::Format				imgFormat = imgDesc.m_format;
		const AZ::RHI::Size					imgSize = imgDesc.m_size;

		CImage::EFormat		pkImageFormat = CImage::Format_Invalid;
		u32					pkImageFlags = 0;
		ToPkImageFormatAndFlags(imgFormat, pkImageFormat, pkImageFlags);

		if (pkImageFormat == CImage::Format_Invalid)
		{
			AZ_Error("PopcornFX", false, "Texture format not recognized by PopcornFX");
			return false;
		}

		const u32	sizeInBytes = static_cast<u32>(imgData.size());
		if (sizeInBytes <= 0)
		{
			AZ_Error("PopcornFX", false, "Invalid texture image data");
			return false;
		}

		CImageMap	map;
		map.m_RawBuffer = CRefCountedMemoryBuffer::AllocAligned(sizeInBytes, 0x80);
		if (!PK_VERIFY(map.m_RawBuffer != null))
		{
			AZ_Error("PopcornFX", false, "Failed allocating '%d' bytes for PopcornFX texture", sizeInBytes);
			return false;
		}
		Mem::Copy(map.m_RawBuffer->Data<void>(), imgData.data(), imgData.size());
		map.m_Dimensions = CUint3(imgSize.m_width, imgSize.m_height, imgSize.m_depth);

		desc->m_ImageDimensions = map.m_Dimensions.xy();

		CImageSurface	surface(map, pkImageFormat);

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
