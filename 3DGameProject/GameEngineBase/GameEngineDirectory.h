#pragma once
#include <string_view>
#include <vector>
#include "GameEnginePath.h"


// 설명 :
class GameEngineFile;
class GameEngineDirectory : public GameEnginePath
{
public:
	// constrcuter destructer
	GameEngineDirectory();
	~GameEngineDirectory();

	GameEngineDirectory(std::string Path);
	GameEngineDirectory(std::string_view Path);
	GameEngineDirectory(std::filesystem::path Path);

	// delete Function
	GameEngineDirectory(const GameEngineDirectory& _Other) = delete;
	GameEngineDirectory(GameEngineDirectory&& _Other) noexcept = delete;
	GameEngineDirectory& operator=(const GameEngineDirectory& _Other) = delete;
	GameEngineDirectory& operator=(GameEngineDirectory&& _Other) noexcept = delete;

	bool MoveParent();

	void MoveParentToDirectory(const std::string_view& _String);

	bool Move(const std::string_view& _String);

	GameEnginePath GetPlusFileName(const std::string_view& _String);

	// 하위는 돌지 않고 그 디렉토리에 해당하는 모든 파일만 리턴해주는 함수.
	// std::vector<GameEngineFile> GetAllFile(const std::string_view& _Ext = "");

	std::vector<GameEngineFile> GetAllFile(std::vector<std::string_view> _Ext);

protected:

private:

};

