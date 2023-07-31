#pragma once
#include "GameEngineRenderer.h"
#include "GameEngineSprite.h"
#include <map>
#include "EngineContentRenderingStruct.h"

enum class TileMapMode
{
	Rect,
	Iso
};

class Tile
{
public:
	GameEngineSprite* Sprite;
	size_t Index = 0;
};

// Ό³Έν :
class GameEngineTileMapRenderer : public GameEngineRenderer
{
public:
	// constrcuter destructer
	GameEngineTileMapRenderer();
	~GameEngineTileMapRenderer();

	// delete Function
	GameEngineTileMapRenderer(const GameEngineTileMapRenderer& _Other) = delete;
	GameEngineTileMapRenderer(GameEngineTileMapRenderer&& _Other) noexcept = delete;
	GameEngineTileMapRenderer& operator=(const GameEngineTileMapRenderer& _Other) = delete;
	GameEngineTileMapRenderer& operator=(GameEngineTileMapRenderer&& _Other) noexcept = delete;

	ColorOption ColorOptionValue;

	void CreateTileMap(int _X, int _Y, float _ZPos, const float4& _TileSize, const float4& _RenderSize = float4::Zero, TileMapMode Mode = TileMapMode::Rect);

	void Clear();

	void SetTile(int _X, int _Y, const std::string_view& _SpriteName = "Error", size_t _Index = 0);

	void SetTile(const float4& _Pos, const std::string_view& _SpriteName = "Error", size_t _Index = 0);

	size_t GetTIleIndex(const float4& _Pos);

	float4 PosToTilePos(float4 _Pos);

	bool IsOver(int _X, int _Y) const;

	inline float4 GetCount() const
	{
		return MapCount;
	}

	inline void TilemapCullingOn()
	{
		IsTilemapCulling = true;
	}

	inline void TilemapCullingOff()
	{
		IsTilemapCulling = false;
	}

protected:
	void Render(float _Delta) override;

private:
	bool IsTilemapCulling = false;

	std::vector<std::vector<Tile>> Tiles;
	float4 MapCount;
	float4 AtlasData;
	float4 TileSize;
	float4 RenderSize;
	float4 TileSizeH;
	float ZPos = 0.0f;

	TileMapMode Mode = TileMapMode::Rect;

	void Start() override;
};

