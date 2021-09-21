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

#include "JsonPackPath.h"

#if defined(LMBR_USE_PK)

#include <AzCore/JSON/document.h>
#include <AzCore/JSON/stringbuffer.h>
#include <AzFramework/IO/LocalFileIO.h>

#include <AzCore/JSON/document.h>
#include <AzCore/JSON/stringbuffer.h>
#include <AzCore/JSON/prettywriter.h>

#if defined(POPCORNFX_EDITOR)
#define JSON_PACK_PATH "@devassets@/popcornfx_pack.json"
#else
#define JSON_PACK_PATH "@assets@/popcornfx_pack.json"
#endif

__LMBRPK_BEGIN

bool	LoadPackPathFromJson(AZStd::string &outPackPath, AZStd::string &outLibraryPath)
{
	const AZStd::string	filepath = JSON_PACK_PATH;
	AZ::IO::FileIOBase*	fileReader = AZ::IO::FileIOBase::GetInstance();
	AZ::IO::HandleType	fileHandle = AZ::IO::InvalidHandle;
	if (fileReader->Open(filepath.c_str(), AZ::IO::OpenMode::ModeRead | AZ::IO::OpenMode::ModeBinary, fileHandle))
	{
		AZ::u64 fileSize = 0;
		fileReader->Size(fileHandle, fileSize);

		AZStd::string fileBuffer;
		fileBuffer.resize(fileSize);

		if (fileReader->Read(fileHandle, fileBuffer.data(), fileSize, true))
		{
			rapidjson::Document document;
			document.Parse(fileBuffer.data());
			if (!document.HasParseError())
			{
				bool	success = true;

				rapidjson::Value::MemberIterator packPathIterator = document.FindMember("PackPath");
                success &= packPathIterator != document.MemberEnd();
				if (success)
					outPackPath = packPathIterator->value.GetString();

				rapidjson::Value::MemberIterator libraryPathIterator = document.FindMember("LibraryPath");
                success &= libraryPathIterator != document.MemberEnd();
				if (success)
					outLibraryPath = libraryPathIterator->value.GetString();

				fileReader->Close(fileHandle);

				return success;
			}
		}
		fileReader->Close(fileHandle);
	}
	return false;
}

bool	SavePackPathToJson(const AZStd::string &packPath, const AZStd::string &libraryPath)
{
	const AZStd::string	filepath = JSON_PACK_PATH;
	AZ::IO::FileIOBase	*fileIo = AZ::IO::FileIOBase::GetInstance();
	AZ::IO::HandleType	fileHandle = AZ::IO::InvalidHandle;
	if (fileIo->Open(filepath.c_str(), AZ::IO::OpenMode::ModeWrite | AZ::IO::OpenMode::ModeText, fileHandle))
	{
		rapidjson::Document json;
		rapidjson::Value& root = json.SetObject();

		rapidjson::Value packPathItem(rapidjson::kStringType);
		packPathItem.SetString(packPath.c_str(), json.GetAllocator());
		root.AddMember("PackPath", packPathItem, json.GetAllocator());

		rapidjson::Value libraryPathItem(rapidjson::kStringType);
		libraryPathItem.SetString(libraryPath.c_str(), json.GetAllocator());
		root.AddMember("LibraryPath", libraryPathItem, json.GetAllocator());

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		json.Accept(writer);

		AZ::u64 bytesWritten = 0;
		if (!fileIo->Write(fileHandle, buffer.GetString(), buffer.GetSize(), &bytesWritten))
		{
			AZ_Error("PopcornFX", false, "Failed to write popcornfx pack file: %s", filepath.c_str());
			return false;
		}
		fileIo->Close(fileHandle);
		return true;
	}
	return false;
}

__LMBRPK_END
//----------------------------------------------------------------------------

#endif //LMBR_USE_PK

