#pragma once
#include <set>
#include <GameEngineBase/GameEngineMath.h>
#include "GameEngineRenderer.h"

class NaviNode
{
public:
	float4 ArrTri[3];
	int ArrIndex[3];
	int VectorIndex;

	std::set<int> Link;
};

// 설명 :
class GameEngineNavi : public GameEngineRenderer
{
public:
	// constrcuter destructer
	GameEngineNavi();
	~GameEngineNavi();

	// delete Function
	GameEngineNavi(const GameEngineNavi& _Other) = delete;
	GameEngineNavi(GameEngineNavi&& _Other) noexcept = delete;
	GameEngineNavi& operator=(const GameEngineNavi& _Other) = delete;
	GameEngineNavi& operator=(GameEngineNavi&& _Other) noexcept = delete;

	// 안에서 내비매쉬를 만들어 내는 함수.
	void NaviLoad(std::string _FileName);

	// 갈수 있어 없어?
	bool IsMove(GameEngineObject* _FindObject, float4 NextPos);

protected:

private:
	std::vector<std::vector<std::shared_ptr<NaviNode>>> Nodes;

	//      플레이어나 몬스터들    밟고있는 삼각형.
	std::map<GameEngineObject*, std::shared_ptr<NaviNode>> NaviMeshs;
};

