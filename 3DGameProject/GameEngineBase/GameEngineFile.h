#pragma once
#include "GameEngineDirectory.h"
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
class GameEngineFile : public GameEnginePath
{
public:
	// constrcuter destructer
	GameEngineFile();
	~GameEngineFile();
	GameEngineFile(std::filesystem::path _Path);
	GameEngineFile(const std::string_view& _Path);
	GameEngineFile(const GameEnginePath& _Path);

	// delete Function
	//GameEngineFile(const GameEngineFile& _Other) = delete;
	// GameEngineFile(GameEngineFile&& _Other) noexcept = delete;
	//GameEngineFile& operator=(const GameEngineFile& _Other) = delete;
	//GameEngineFile& operator=(GameEngineFile&& _Other) noexcept = delete;

	inline std::string GetFullPath() const
	{
		return Path.string();
	}

	void SaveBin(const GameEngineSerializer& _Data);
	void SaveText(const std::string_view& _View);

	void LoadBin(GameEngineSerializer& _Data);

	void LoadText(GameEngineSerializer& _Data);

	void ChangeExtension(std::string_view _NewExtension);

	std::string GetString();

	uintmax_t GetFileSize();

	GameEngineDirectory GetDirectory();

protected:

};

