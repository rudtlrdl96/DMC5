#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include "GameEngineRenderer.h"
#include <set>

class NaviNode : std::enable_shared_from_this<NaviNode>
{
public:
	float4 ArrTri[3];
	int ArrIndex[3];
	int VectorIndex;

	std::set<int> Link;
};

// ���� :
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

	// �ȿ��� ����Ž��� ����� ���� �Լ�.
	void NaviLoad(std::string _FileName);


	// ���� �־� ����?
	bool IsMove(GameEngineObject* _FindObject, float4 NextPos);


protected:

private:
	std::vector<std::shared_ptr<NaviNode>> Nodes;

	//      �÷��̾ ���͵�    ����ִ� �ﰢ��.
	std::map<GameEngineObject*, std::shared_ptr<NaviNode>> NaviMeshs;
};

