#include "PrecompileHeader.h"
#include "GameEngineNavi.h"

GameEngineNavi::GameEngineNavi()
{
}

GameEngineNavi::~GameEngineNavi()
{
}


void GameEngineNavi::NaviLoad(std::string _FileName)
{

	// 삼각형이 다 들어갔다고 치고.
	// 
	for (size_t i = 0; i < Nodes.size(); i++)
	{
		std::shared_ptr<NaviNode> Node = Nodes[i];

		for (size_t j = i + 1; j < Nodes.size(); j++)
		{
			std::shared_ptr<NaviNode> OtherNode = Nodes[j];
			for (size_t indexCount = 0; indexCount < 3; indexCount++)
			{
				// 포지션으로 비교하거나 인덱스로 비교해라.
				int NodeIndex = Node->ArrIndex[i];
				for (size_t OtherIndexCount = 0; OtherIndexCount < 3; OtherIndexCount++)
				{
					int OtherNodeIndex = OtherNode->ArrIndex[i];

					if (NodeIndex == OtherNodeIndex)
					{
						OtherNode->Link.insert(Node->VectorIndex);
						Node->Link.insert(OtherNode->VectorIndex);
					}
				}
			}


		}
	}
}