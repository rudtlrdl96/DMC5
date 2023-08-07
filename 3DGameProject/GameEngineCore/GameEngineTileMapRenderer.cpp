#include "PrecompileHeader.h"
#include "GameEngineTileMapRenderer.h"
#include "GameEngineSprite.h"
#include "GameEngineLevel.h"
#include "GameEngineCamera.h"

GameEngineTileMapRenderer::GameEngineTileMapRenderer()
{
}

GameEngineTileMapRenderer::~GameEngineTileMapRenderer()
{
}

void GameEngineTileMapRenderer::Start()
{
	GameEngineRenderer::Start();

	std::shared_ptr<GameEngineRenderUnit> Unit = CreateRenderUnit();

	Unit->SetMesh("Rect");
	Unit->SetPipeLine("TileMap");

	// SetPipeLine("2DTexture");

	AtlasData.x = 0.0f;
	AtlasData.y = 0.0f;
	AtlasData.z = 1.0f;
	AtlasData.w = 1.0f;

	ColorOptionValue.MulColor = float4::ONE;
	ColorOptionValue.PlusColor = float4::ZERONULL;

	GetShaderResHelper().SetConstantBufferLink("AtlasData", AtlasData);
	GetShaderResHelper().SetConstantBufferLink("ColorOption", ColorOptionValue);

}

void GameEngineTileMapRenderer::CreateTileMap(int _X, int _Y, float _ZPos, const float4& _TileSize, const float4& _RenderSize, TileMapMode _Mode)
{
	TileSize = _TileSize;
	TileSize.z = 1.0f;
	TileSizeH = TileSize.half();
	ZPos = _ZPos;

	if (_RenderSize == float4::ZERO)
	{
		RenderSize = TileSize;
	}
	else {
		RenderSize = _RenderSize;
	}

	MapCount.x = static_cast<float>(_X);
	MapCount.y = static_cast<float>(_Y);

	Tiles.resize(_Y);

	for (size_t y = 0; y < Tiles.size(); y++)
	{
		Tiles[y].resize(_X);
	}

	Mode = _Mode;
}

void GameEngineTileMapRenderer::Clear()
{
	Tiles.clear();
}

void GameEngineTileMapRenderer::SetTile(int _X, int _Y, const std::string_view& _SpriteName, size_t _Index)
{
	if (true == Tiles.empty())
	{
		MsgAssert("CreateTileMap을 먼저 호출해주셔야 합니다.");
	}

	// 인덱스 오버
	if (true == IsOver(_X, _Y))
	{
		MsgAssert("타일맵 크기를 초과해 접근하려 했습니다");
		return;
	}


	std::shared_ptr<GameEngineSprite> Sprite = GameEngineSprite::Find(_SpriteName);

	if (nullptr == Sprite)
	{
		MsgAssert(std::string(_SpriteName) + "존재하지 않는 스프라이트 입니다.");
	}

	Tiles[_Y][_X].Sprite = Sprite.get();
	Tiles[_Y][_X].Index = _Index;
}

bool GameEngineTileMapRenderer::IsOver(int _X, int _Y) const
{
	if (0 > _X || MapCount.ix() <= _X)
	{
		return true;
	}

	if (0 > _Y || MapCount.iy() <= _Y)
	{
		return true;
	}

	return false;
}

void GameEngineTileMapRenderer::Render(float _Delta)
{
	const TransformData& TransData = GetTransform()->GetTransDataRef();
	// 

	TransformData TileTransData = TransData;

	float4x4 Scale;
	float4x4 Pos;
	float4x4 Rot;
	float4 vPos;

	std::shared_ptr<GameEngineCamera> Camera = GetLevel()->GetMainCamera();

	for (size_t y = 0; y < Tiles.size(); y++)
	{
		for (size_t x = 0; x < Tiles[y].size(); x++)
		{
			GameEngineSprite* Sprite = Tiles[y][x].Sprite;
			const SpriteInfo& SpriteInfo = Sprite->GetSpriteInfo(Tiles[y][x].Index);

			// 트랜스폼 세팅
			{
				switch (Mode)
				{
				case TileMapMode::Rect:
					vPos = { TileSize.x * x, TileSize.y * y, 1.0f };
					break;
				case TileMapMode::Iso:
					vPos.x = (x * TileSizeH.x) - (y * TileSizeH.x);
					vPos.y = -(x * TileSizeH.y) - (y * TileSizeH.y);
					vPos.y -= TileSizeH.y;
					break;
				default:
					break;
				}

				vPos.z = ZPos;

				Scale.Scale(RenderSize);
				Pos.Pos(vPos);
				TileTransData.WorldViewProjectionMatrix = Scale * Pos * TransData.WorldMatrix;

				if (true == IsTilemapCulling)
				{
					TileTransData.WorldPosition = TileTransData.WorldViewProjectionMatrix.ArrVector[3];
					TileTransData.WorldScale = TileSize;

					if (false == Camera->IsView(TileTransData))
					{
						continue;
					}
				}

				//if (true == GetLevel()->IsCameraOver(WorldPos))
				//{
				//	continue;
				//}

				TileTransData.WorldViewProjectionMatrix = TileTransData.WorldViewProjectionMatrix * TransData.View * TransData.Projection;

				GetShaderResHelper().SetConstantBufferLink("TransformData", TileTransData);
			}

			// 텍스처 세팅
			{
				GetShaderResHelper().SetTexture("DiffuseTex", SpriteInfo.Texture);
				AtlasData = SpriteInfo.CutData;
			}

			GameEngineRenderer::Render(_Delta);
		}
	}

	GetShaderResHelper().SetConstantBufferLink("TransformData", GetTransform()->GetTransDataRef());

}


void GameEngineTileMapRenderer::SetTile(const float4& _Pos, const std::string_view& _SpriteName, size_t _Index)
{
	int X = -1;
	int Y = -1;


	switch (Mode)
	{
	case TileMapMode::Rect:
		X = static_cast<int>(_Pos.x / TileSize.x);
		Y = static_cast<int>(_Pos.y / TileSize.y);
		break;
	case TileMapMode::Iso:
		X = static_cast<int>((_Pos.x / TileSizeH.x + -_Pos.y / TileSizeH.y) / 2);
		Y = static_cast<int>((-_Pos.y / TileSizeH.y - (_Pos.x / TileSizeH.x)) / 2);
		break;
	default:
		break;
	}

	SetTile(X, Y, _SpriteName, _Index);
}

size_t GameEngineTileMapRenderer::GetTIleIndex(const float4& _Pos)
{
	int X = -1;
	int Y = -1;


	switch (Mode)
	{
	case TileMapMode::Rect:
		X = static_cast<int>(_Pos.x / TileSize.x);
		Y = static_cast<int>(_Pos.y / TileSize.y);
		break;
	case TileMapMode::Iso:
		X = static_cast<int>((_Pos.x / TileSizeH.x + -_Pos.y / TileSizeH.y) / 2);
		Y = static_cast<int>((-_Pos.y / TileSizeH.y - (_Pos.x / TileSizeH.x)) / 2);
		break;
	default:
		break;
	}
	if (true == Tiles.empty())
	{
		MsgAssert("CreateTileMap을 먼저 호출해주셔야 합니다.");
		return -1;
	}

	// 인덱스 오버
	if (true == IsOver(X, Y))
	{
		MsgAssert("타일맵 크기를 초과해 접근하려 했습니다");
		return -1;
	}
	if (nullptr == Tiles[X][Y].Sprite)
	{
		MsgAssert("타일맵이 존재하지 않습니다");
		return -1;
	}
	return Tiles[X][Y].Index;
}


float4 GameEngineTileMapRenderer::PosToTilePos(float4 _Pos)
{
	int X = -1;
	int Y = -1;


	switch (Mode)
	{
	case TileMapMode::Rect:
		X = static_cast<int>(_Pos.x / TileSize.x);
		Y = static_cast<int>(_Pos.y / TileSize.y);
		break;
	case TileMapMode::Iso:
		X = static_cast<int>((_Pos.x / TileSizeH.x + -_Pos.y / TileSizeH.y) / 2);
		Y = static_cast<int>((-_Pos.y / TileSizeH.y - (_Pos.x / TileSizeH.x)) / 2);
		break;
	default:
		break;
	}
	float4 ReturnPos;
	switch (Mode)
	{
	case TileMapMode::Rect:
		ReturnPos = { TileSize.x * X, TileSize.y * Y, 1.0f };
		break;
	case TileMapMode::Iso:
		ReturnPos.x = (X * TileSizeH.x) - (Y * TileSizeH.x);
		ReturnPos.y = -(X * TileSizeH.y) - (Y * TileSizeH.y);
		ReturnPos.y -= TileSizeH.y;
		break;
	default:
		break;
	}

	return ReturnPos;

}
