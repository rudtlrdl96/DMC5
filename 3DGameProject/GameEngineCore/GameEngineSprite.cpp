#include "PrecompileHeader.h"
#include "GameEngineSprite.h"

GameEngineSprite::GameEngineSprite() 
{
}

GameEngineSprite::~GameEngineSprite() 
{
}

void GameEngineSprite::ResLoadFolder(const std::string_view& _Path)
{
	GameEngineDirectory Dir = _Path;

	if (false == Dir.GetPath().IsDirectory())
	{
		MsgAssert("디렉토리가 아닌 경로입니다." + std::string(_Path));
	}

 	std::vector<GameEngineFile> AllFile = Dir.GetAllFile({".png", ".jpg"});

	if (0 >= AllFile.size())
	{
		MsgAssert("이미지가 존재하지 않는 경로입니다." + std::string(_Path));
	}

	std::string FileName = Dir.GetPath().GetFileName();

	Sprites.resize(AllFile.size());

	for (size_t i = 0; i < AllFile.size(); i++)
	{
		GameEngineFile& File = AllFile[i];
		std::shared_ptr<GameEngineTexture> Texture =  GameEngineTexture::Load(File.GetFullPath());

		Sprites[i].Texture = Texture;
		Sprites[i].CutData.PosX = 0.0f;
		Sprites[i].CutData.PosY = 0.0f;
		Sprites[i].CutData.SizeX = 1.0f;
		Sprites[i].CutData.SizeY = 1.0f;
	}

	return;
}



void GameEngineSprite::ResLoadSheet(const std::string_view& _Path, size_t _X, size_t _Y)
{
	GameEnginePath NewPath(_Path);

	std::shared_ptr<GameEngineTexture> Texture = GameEngineTexture::Find(NewPath.GetFileName());
	if (nullptr == Texture)
	{
		Texture = GameEngineTexture::Load(_Path);
	}

	Sprites.resize(_Y * _X);

	float4 UVScale = { 1.0f/ static_cast<float>(_X), 1.0f / static_cast<float>(_Y) };

	float4 Start = float4::ZERO;

	// 2 2

	for (size_t y = 0; y < _Y; y++)
	{
		for (size_t x = 0; x < _X; x++)
		{
			size_t Index = (_X * y) + x;

			Sprites[Index].Texture = Texture;
			Sprites[Index].CutData.PosX = Start.x;
			Sprites[Index].CutData.PosY = Start.y;
			Sprites[Index].CutData.SizeX = UVScale.x;
			Sprites[Index].CutData.SizeY = UVScale.y;
			Start.x += UVScale.x;
		}

		Start.x = 0.0f;
		Start.y += UVScale.y;
	}

}

void GameEngineSprite::Release() 
{
	for (size_t i = 0; i < Sprites.size(); i++)
	{
		Sprites[i].Texture->Release();
	}
}
void GameEngineSprite::ReLoad() 
{
	for (size_t i = 0; i < Sprites.size(); i++)
	{
		Sprites[i].Texture->ReLoad();
	}
}