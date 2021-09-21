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

#include <AzCore/IO/FileIO.h>
#include <pk_kernel/include/kr_file_controller.h>

__LMBRPK_BEGIN

class FileSystemController_LMBR;

class	CFileStreamFS_LMBR : public CFileStream
{
private:
	CFileStreamFS_LMBR(
		FileSystemController_LMBR	*controller,
		PFilePack					pack,
		const CString				&path,
		IFileSystem::EAccessPolicy	mode,
		AZ::IO::HandleType			file);

public:
	virtual ~CFileStreamFS_LMBR();

	static CFileStreamFS_LMBR	*Open(FileSystemController_LMBR *controller, PFilePack pack, const CString &path, IFileSystem::EAccessPolicy mode);

	virtual u64		Read(void *targetBuffer, u64 byteCount) override;
	virtual u64		Write(const void *sourceBuffer, u64 byteCount) override;
	virtual bool	Seek(s64 offset, ESeekMode whence) override;
	virtual u64		Tell() const override;
	virtual bool	Eof() const override;
	virtual bool	Flush() override;
	virtual void	Close() override;
	virtual u64		SizeInBytes() const override;
	virtual void	Timestamps(SFileTimes &timestamps) override;

private:
	AZ::IO::HandleType			m_File;
};
PK_DECLARE_REFPTRCLASS(FileStreamFS_LMBR);

class	FileSystemController_LMBR : public PopcornFX::CFileSystemBase
{
public:
	FileSystemController_LMBR() {}
	~FileSystemController_LMBR() {}

	virtual PopcornFX::PFileStream	OpenStream(const PopcornFX::CString &path, PopcornFX::IFileSystem::EAccessPolicy mode, bool pathNotVirtual) override;
	virtual bool					Exists(const PopcornFX::CString &path, bool pathNotVirtual = false) override;
	virtual bool					FileDelete(const PopcornFX::CString &path, bool pathNotVirtual = false) override;
	virtual bool					FileCopy(const PopcornFX::CString &sourcePath, const PopcornFX::CString &targetPath, bool pathsNotVirtual = false) override;
	virtual void					GetDirectoryContents(char *dpath, char *virtualPath, u32 pathLength, PopcornFX::CFileDirectoryWalker *walker, const PopcornFX::CFilePack *pack) override;
	virtual bool					CreateDirectoryChainIFN(const PopcornFX::CString &directoryPath, bool pathNotVirtual = false) override;
	virtual bool					DirectoryDelete(const PopcornFX::CString &path, bool pathNotVirtual = false);
};

__LMBRPK_END

#endif //LMBR_USE_PK
