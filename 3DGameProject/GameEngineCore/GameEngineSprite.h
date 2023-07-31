#pragma once
#include "GameEngineTexture.h"

class SpriteInfo
{
public:
	std::shared_ptr<GameEngineTexture> Texture;
	float4 CutData;
};
// �̰� ���鶧 �� �ؽ�ó�� �ε��� �ȵǾ������� �ؽ�ó�� �ε��� ���մϴ�.

// ���� : ��������Ʈ�� ������ 1�� �̻��� �̹��� ������ ����ִ�
class GameEngineSprite : public GameEngineResource<GameEngineSprite>
{
public:
	// constrcuter destructer
	GameEngineSprite();
	~GameEngineSprite();

	// delete Function
	GameEngineSprite(const GameEngineSprite& _Other) = delete;
	GameEngineSprite(GameEngineSprite&& _Other) noexcept = delete;
	GameEngineSprite& operator=(const GameEngineSprite& _Other) = delete;
	GameEngineSprite& operator=(GameEngineSprite&& _Other) noexcept = delete;


	static std::shared_ptr<GameEngineSprite> LoadFolder(const std::string_view& _Path)
	{
		GameEnginePath NewPath = std::string(_Path);
		return LoadFolder(NewPath.GetFileName(), _Path);
	}

	static std::shared_ptr<GameEngineSprite> LoadFolder(std::string _Spritename, const std::string_view& _Path)
	{
		std::shared_ptr<GameEngineSprite> NewTexture = GameEngineResource::Create(_Spritename);
		NewTexture->ResLoadFolder(_Path);
		return NewTexture;
	}


	static std::shared_ptr<GameEngineSprite> LoadSheet(const std::string_view& _Path, size_t _X, size_t _Y)
	{
		GameEnginePath NewPath = std::string(_Path);

		std::shared_ptr<GameEngineSprite> NewTexture = GameEngineResource::Create(NewPath.GetFileName());
		NewTexture->ResLoadSheet(_Path, _X, _Y);
		return NewTexture;
	}

	size_t GetSpriteCount() 
	{
		return Sprites.size();
	}

	const SpriteInfo& GetSpriteInfo(size_t _Index) 
	{
		if (_Index < 0)
		{
			MsgAssert("0������ ��������Ʈ �ε��� �Դϴ�.");
		}

		if (_Index >= Sprites.size())
		{
			MsgAssert("��������Ʈ�� �ε����� �����߽��ϴ�.");
		}

		return Sprites[_Index];
	}

	static std::shared_ptr<GameEngineSprite> UnLoad(const std::string_view& _Name)
	{
		std::shared_ptr<GameEngineSprite> FindSprite = GameEngineResource::Find(_Name);

		if (nullptr == FindSprite)
		{
			MsgAssert("�������� �ʴ� ��������Ʈ�� ��ε� �Ϸ��� �߽��ϴ�.");
		}

		FindSprite->Release();
		return FindSprite;
	}


	static std::shared_ptr<GameEngineSprite> ReLoad(const std::string_view& _Path)
	{
		GameEnginePath NewPath(_Path);
		return ReLoad(_Path, NewPath.GetFileName());
	}


	static std::shared_ptr<GameEngineSprite> ReLoad(const std::string_view& _Path, const std::string_view& _Name)
	{
		std::shared_ptr<GameEngineSprite> NewTexture = GameEngineResource<GameEngineSprite>::Find(_Name);

		if (nullptr == NewTexture)
		{
			return LoadFolder(_Name.data(), _Path);
		}

		NewTexture->ReLoad();
		return NewTexture;
	}


	void ReLoad();
	void Release();

protected:

private:
	void ResLoadFolder(const std::string_view& _Path);
	void ResLoadSheet(const std::string_view& _Path, size_t _X, size_t _Y);

	std::vector<SpriteInfo> Sprites;

};

