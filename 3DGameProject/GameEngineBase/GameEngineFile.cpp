#include "PrecompileHeader.h"
#include "GameEngineFile.h"
#include "GameEngineDebug.h"
#include "GameEngineString.h"

GameEngineFile::GameEngineFile()
{
}

GameEngineFile::~GameEngineFile()
{
}


GameEngineFile::GameEngineFile(std::filesystem::path _Path)
	: GameEnginePath(_Path)
{

}

GameEngineFile::GameEngineFile(const std::string_view& _Path)
	: GameEnginePath(_Path)
{

}

GameEngineFile::GameEngineFile(const GameEnginePath& _Path)
	: GameEnginePath(_Path)
{

}

void GameEngineFile::SaveBin(const GameEngineSerializer& _Data)
{
	FILE* FilePtr = nullptr;

	std::string PathString = GetFullPath();
	std::string Text = "wb";

	fopen_s(&FilePtr, PathString.c_str(), Text.c_str());

	if (nullptr == FilePtr)
	{
		MsgAssert("파일 오픈에 실패했습니다." + PathString);
	}
	//                         500               1
	fwrite(_Data.GetConstData(), _Data.GetWriteOffSet(), 1, FilePtr);

	if (nullptr != FilePtr)
	{
		fclose(FilePtr);
	}
}

void GameEngineFile::SaveText(const std::string_view& _View)
{
	FILE* FilePtr = nullptr;
	std::string PathString = GetFullPath();
	std::string Text = "wt";
	fopen_s(&FilePtr, PathString.c_str(), Text.c_str());

	if (nullptr == FilePtr)
	{
		MsgAssert("파일 오픈에 실패했습니다." + PathString);
	}

	//                         500               1
	fwrite(_View.data(), _View.size(), 1, FilePtr);

	if (nullptr != FilePtr)
	{
		fclose(FilePtr);
	}
}

void GameEngineFile::LoadBin(GameEngineSerializer& _Data)
{
	FILE* FilePtr = nullptr;

	std::string PathString = GetFullPath();
	std::string Text = "rb";

	fopen_s(&FilePtr, PathString.c_str(), Text.c_str());

	if (nullptr == FilePtr)
	{
		MsgAssert("파일 오픈에 실패했습니다." + PathString);
		return;
	}

	size_t FileSize = std::filesystem::file_size(Path);

	_Data.BufferResize(static_cast<unsigned int>(FileSize));

	fread_s(_Data.GetDataPtr(), _Data.GetBufferSize(), FileSize, 1, FilePtr);

	if (nullptr != FilePtr)
	{
		fclose(FilePtr);
	}
}

void GameEngineFile::LoadText(GameEngineSerializer& _Data)
{
	FILE* FilePtr = nullptr;

	std::string PathString = GetFullPath();
	std::string Text = "rt";

	fopen_s(&FilePtr, PathString.c_str(), Text.c_str());

	if (nullptr == FilePtr)
	{
		MsgAssert("파일 오픈에 실패했습니다." + PathString);
	}

	size_t FileSize = std::filesystem::file_size(Path);

	fread_s(_Data.GetDataPtr(), _Data.GetBufferSize(), FileSize, 1, FilePtr);
	_Data.ReadOffset = static_cast<unsigned>(_Data.GetBufferSize());

	if (nullptr != FilePtr)
	{
		fclose(FilePtr);
	}
}

std::string GameEngineFile::GetString()
{
	uintmax_t size = GetFileSize();
	GameEngineSerializer Ser;
	Ser.BufferResize(static_cast<unsigned int>(size + 1));
	LoadText(Ser);

	return Ser.GetString();
}

uintmax_t GameEngineFile::GetFileSize()
{
	return std::filesystem::file_size(Path);
}

GameEngineDirectory GameEngineFile::GetDirectory()
{
	GameEnginePath ReturnPath = Path;
	ReturnPath.MoveParent();
	return GameEngineDirectory(ReturnPath.Path);
}