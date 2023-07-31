#pragma once
#include "GameEnginePath.h"
#include "GameEngineSerializer.h"

// �����̶�� ���� Ȯ���ڵ� �ǹ̾���
// ����Ʈ ����� ���̴�.
// c��Ÿ���� ����������� ����Ұ��Դϴ�.

enum class SAVEMODE
{
	Text,
	Bin,
};

// ���� :
class GameEngineFile
{
public:
	// constrcuter destructer
	GameEngineFile();
	~GameEngineFile();
	GameEngineFile(std::filesystem::path _Path);

	// delete Function
	//GameEngineFile(const GameEngineFile& _Other) = delete;
	// GameEngineFile(GameEngineFile&& _Other) noexcept = delete;
	//GameEngineFile& operator=(const GameEngineFile& _Other) = delete;
	//GameEngineFile& operator=(GameEngineFile&& _Other) noexcept = delete;

	std::string GetFullPath() 
	{
		return Path.Path.string();
	}

	void SaveBin(const GameEngineSerializer& _Data);
	void SaveText(const std::string_view& _View);

	void LoadBin(GameEngineSerializer& _Data);

	void LoadText(GameEngineSerializer& _Data);

	std::string GetString();

	uintmax_t GetFileSize();

protected:

private:
	GameEnginePath Path;
};

