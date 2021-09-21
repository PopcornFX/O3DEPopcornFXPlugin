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

#include "FileSystemController_LMBR.h"

#if defined(LMBR_USE_PK)

#include "Integration/PopcornFXIntegrationBus.h"

__LMBRPK_BEGIN

//----------------------------------------------------------------------------
//
// CFileSystemController_LMBR
//
//----------------------------------------------------------------------------

CFileStreamFS_LMBR::CFileStreamFS_LMBR(
	FileSystemController_LMBR	*controller,
	PopcornFX::PFilePack		pack,
	const CString				&path,
	IFileSystem::EAccessPolicy	mode,
	AZ::IO::HandleType			file)
	: CFileStream(controller, pack, path, mode)
	, m_File(file)
{
}

//----------------------------------------------------------------------------

CFileStreamFS_LMBR::~CFileStreamFS_LMBR()
{
	Close();
}

//----------------------------------------------------------------------------

u64	CFileStreamFS_LMBR::Read(void *targetBuffer, u64 byteCount)
{
	AZ::IO::FileIOBase	*fileIO = AZ::IO::FileIOBase::GetInstance();
	if (!PK_VERIFY(fileIO != null))
		return 0;

	u64	bytesRead = 0;
	if (fileIO->Read(m_File, targetBuffer, byteCount, false, &bytesRead))
		return bytesRead;
	return 0;
}

//----------------------------------------------------------------------------

u64	CFileStreamFS_LMBR::Write(const void *sourceBuffer, u64 byteCount)
{
	AZ::IO::FileIOBase	*fileIO = AZ::IO::FileIOBase::GetInstance();
	if (!PK_VERIFY(fileIO != null))
		return 0;

	u64	bytesWritten = 0;
	if (fileIO->Write(m_File, sourceBuffer, byteCount, &bytesWritten))
		return bytesWritten;
	return 0;
}

//----------------------------------------------------------------------------

bool	CFileStreamFS_LMBR::Seek(s64 offset, ESeekMode whence)
{
	AZ::IO::FileIOBase	*fileIO = AZ::IO::FileIOBase::GetInstance();
	if (!PK_VERIFY(fileIO != null))
		return false;

	return fileIO->Seek(m_File, offset, (AZ::IO::SeekType)whence);
}

//----------------------------------------------------------------------------

u64	CFileStreamFS_LMBR::Tell() const
{
	AZ::IO::FileIOBase	*fileIO = AZ::IO::FileIOBase::GetInstance();
	if (!PK_VERIFY(fileIO != null))
		return 0;

	AZ::u64	offset;
	if (fileIO->Tell(m_File, offset))
		return offset;
	return 0;
}

//----------------------------------------------------------------------------

bool	CFileStreamFS_LMBR::Eof() const
{
	AZ::IO::FileIOBase	*fileIO = AZ::IO::FileIOBase::GetInstance();
	if (!PK_VERIFY(fileIO != null))
		return true;

	return fileIO->Eof(m_File);
}

//----------------------------------------------------------------------------

bool	CFileStreamFS_LMBR::Flush()
{
	PK_ASSERT_NOT_REACHED();
	return true;
}

//----------------------------------------------------------------------------

void	CFileStreamFS_LMBR::Close()
{
	AZ::IO::FileIOBase	*fileIO = AZ::IO::FileIOBase::GetInstance();
	if (!PK_VERIFY(fileIO != null))
		return;

	if (m_File != AZ::IO::InvalidHandle)
	{
		if (!fileIO->Close(m_File))
		{
			AZ_Assert(fileIO, "Close file error.");
		}
		m_File = AZ::IO::InvalidHandle;
	}
}

//----------------------------------------------------------------------------

u64	CFileStreamFS_LMBR::SizeInBytes() const
{
	AZ::IO::FileIOBase	*fileIO = AZ::IO::FileIOBase::GetInstance();
	if (!PK_VERIFY(fileIO != null))
		return 0;

	AZ::u64 sizeBytes = 0;
	if (fileIO->Size(m_File, sizeBytes))
		return sizeBytes;
	return 0;
}

//----------------------------------------------------------------------------

void	CFileStreamFS_LMBR::Timestamps(PopcornFX::SFileTimes &timestamps)
{
	(void)timestamps;
	PK_ASSERT_NOT_REACHED();
}

//----------------------------------------------------------------------------

static AZ::IO::OpenMode	ToCry(IFileSystem::EAccessPolicy mode)
{
	switch (mode)
	{
	case PopcornFX::IFileSystem::Access_Read:
		return AZ::IO::OpenMode::ModeRead;
	case PopcornFX::IFileSystem::Access_ReadWrite:
		return AZ::IO::OpenMode::ModeRead | AZ::IO::OpenMode::ModeWrite;
	case PopcornFX::IFileSystem::Access_WriteCreate:
		return AZ::IO::OpenMode::ModeWrite;
	case PopcornFX::IFileSystem::Access_ReadWriteCreate:
		return AZ::IO::OpenMode::ModeRead | AZ::IO::OpenMode::ModeWrite;
	default:
		break;
	}
	return AZ::IO::OpenMode::Invalid;
}

//----------------------------------------------------------------------------

CFileStreamFS_LMBR	*CFileStreamFS_LMBR::Open(FileSystemController_LMBR *controller, PFilePack pack, const CString &path, IFileSystem::EAccessPolicy mode)
{
	CFileStreamFS_LMBR		*stream = null;
	CString					rawPath = path;
	AZ::IO::FileIOBase		*fileIO = AZ::IO::FileIOBase::GetInstance();

	if (!PK_VERIFY(fileIO != null))
		return null;

	AZ::IO::HandleType		fileHandle = AZ::IO::InvalidHandle;
	const AZ::IO::Result	result = fileIO->Open(rawPath.Data(), ToCry(mode), fileHandle);

	if (result)
	{
		stream = PK_NEW(CFileStreamFS_LMBR(controller, pack, path, mode, fileHandle));
	}
	else
		CLog::Log(PK_WARN, "FileStreamFS_LMBR: Open: cannot open '%s'", rawPath.Data());
	return stream;
}

// ----------------------------------------------------------------------------
//
// FileSystemController_LMBR
//
//----------------------------------------------------------------------------

PopcornFX::PFileStream		FileSystemController_LMBR::OpenStream(const CString &path, IFileSystem::EAccessPolicy mode, bool pathNotVirtual)
{
	PK_ASSERT_MESSAGE(!path.Empty(), "null Path");
	PK_ASSERT(mode >= 0 && mode < IFileSystem::__MaxAccessPolicies);

	PFilePack	pack;
	CString		realPath = pathNotVirtual ? path : VirtualToPhysical(path, mode, &pack);
	if (!realPath.Empty())
	{
		return CFileStreamFS_LMBR::Open(this, pack, realPath, mode);
	}
	return null;
}

//----------------------------------------------------------------------------

bool			FileSystemController_LMBR::Exists(const CString &path, bool pathNotVirtual /*= false*/)
{
	if (path.Empty())
		return false;

	if (!pathNotVirtual)
	{
		return !VirtualToPhysical(path, IFileSystem::Access_Read).Empty();
	}

	AZ::IO::FileIOBase	*fileIO = AZ::IO::FileIOBase::GetInstance();
	if (!PK_VERIFY(fileIO != null))
		return false;

	if (!fileIO->Exists(path.Data()))
		CLog::Log(PK_WARN, "FileSystemController_LMBR Exists FAIL '%s' pathNotVirtual:%d", path.Data(), pathNotVirtual);
	else
		return true;
	return false;
}

//----------------------------------------------------------------------------

bool			FileSystemController_LMBR::FileDelete(const CString &path, bool pathNotVirtual /*= false*/)
{
	(void)path; (void)pathNotVirtual;
	PK_ASSERT_NOT_REACHED();
	return false;
}

bool			FileSystemController_LMBR::FileCopy(const CString &sourcePath, const CString &targetPath, bool pathsNotVirtual /*= false*/)
{
	(void)sourcePath; (void)targetPath; (void)pathsNotVirtual;
	PK_ASSERT_NOT_REACHED();
	return false;
}

void			FileSystemController_LMBR::GetDirectoryContents(char *dpath, char *virtualPath, u32 pathLength, PopcornFX::CFileDirectoryWalker *walker, const PopcornFX::CFilePack *pack)
{
	(void)dpath; (void)virtualPath; (void)pathLength; (void)walker; (void)pack;
	PK_ASSERT_NOT_REACHED();
	return;
}

bool			FileSystemController_LMBR::CreateDirectoryChainIFN(const CString &directoryPath, bool pathNotVirtual /*= false*/)
{
	(void)pathNotVirtual;
	AZ::IO::FileIOBase* fileIO = AZ::IO::FileIOBase::GetInstance();
	if (fileIO != null)
		return fileIO->CreatePath(directoryPath.Data());
	return false;
}

bool			FileSystemController_LMBR::DirectoryDelete(const PopcornFX::CString &path, bool pathNotVirtual /*= false*/)
{
	(void)path; (void)pathNotVirtual;
	PK_ASSERT_NOT_REACHED();
	return false;
}

__LMBRPK_END

#endif //LMBR_USE_PK
