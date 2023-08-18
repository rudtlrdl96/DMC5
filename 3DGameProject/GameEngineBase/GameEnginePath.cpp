#include "PrecompileHeader.h"
#include "GameEnginePath.h"

#include <Windows.h>
#include <io.h>
#include <GameEngineBase/GameEngineDebug.h>

#include "GameEngineDirectory.h"

std::string GameEnginePath::GetFileFullPath(const std::string_view& _RootDirectory, const std::vector<std::string>& _Moves, const std::string_view& _FileName)
{
	GameEngineDirectory DirPath;

	DirPath.MoveParentToDirectory(_RootDirectory);
	DirPath.Move(_RootDirectory);

	for (size_t i = 0; i < _Moves.size(); i++)
	{
		DirPath.Move(_Moves[i]);
	}

	return DirPath.GetPlusFileName(_FileName).GetFullPath();
}

GameEnginePath::GameEnginePath()
	: Path(std::filesystem::current_path())
{
}

GameEnginePath::GameEnginePath(std::filesystem::path _Path)
	: Path(_Path)
{
}

GameEnginePath::GameEnginePath(const std::string& _Path)
	: Path(_Path)
{

}

GameEnginePath::GameEnginePath(GameEnginePath&& _Other)
	: Path(_Other.Path)
{

}

GameEnginePath::~GameEnginePath()
{
}

GameEnginePath::GameEnginePath(const GameEnginePath& _Other)
	: Path(_Other.Path)
{

}

std::string GameEnginePath::GetFileName(const std::string_view& _Path)
{
	std::filesystem::path NewPath = _Path;

	return NewPath.filename().string();
}


std::string GameEnginePath::GetFolderPath(const std::string_view& _Path)
{
	std::string FileName = GetFileName(_Path);
	std::string FullPath = _Path.data();

	FullPath = FullPath.replace(FullPath.size() - FileName.size(), FileName.size(), "");
	return FullPath;
}

std::string GameEnginePath::GetFileName() const
{
	return Path.filename().string();
}

std::string GameEnginePath::GetExtension() const
{
	return Path.extension().string();
}


std::string GameEnginePath::GetFullPath() const
{
	return Path.string();
}

void GameEnginePath::MoveParent()
{
	Path = Path.parent_path();
}

void GameEnginePath::MoveParentToChildPath(const std::string_view& _String)
{
	while (false == IsRoot())
	{
		if (true == IsExistsToPlusString(_String))
		{
			return;
		}

		MoveParent();
	}

	MsgAssert("이런 경로를 자식으로 가진 부모는 존재하지 않습니다.");
}

bool GameEnginePath::Move(const std::string_view& _Path)
{
	Path += _Path;

	if (false == IsExists())
	{
		MsgAssert("존재하지 않는 경로로 이동하려고 했습니다.");
		return false;
	}

	return true;
}

bool GameEnginePath::IsRoot()
{
	return Path.root_path() == Path;
}

bool GameEnginePath::IsDirectory() const
{
	return std::filesystem::is_directory(Path);
}

bool GameEnginePath::IsExists()
{
	return std::filesystem::exists(Path);
}

bool GameEnginePath::IsExistsToPlusString(const std::string_view& _String)
{
	std::string Str = GetFullPath() + _String.data();
	return 0 == _access(Str.c_str(), 0);
}

void GameEnginePath::SetPath(const std::string_view& _Path)
{
	Path = _Path.data();
}
