#include "PrecompileHeader.h"
#include "AnimationEvent.h"

AnimationEvent::AnimationEvent() 
{
}

AnimationEvent::~AnimationEvent() 
{
}

void AnimationEvent::LoadAll(const AnimLoadParameter& _Parameter)
{

}

/*

GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Mesh");
		NewDir.Move("TestMesh");
		// 테스트 메쉬 폴더는 자동으로 로드합니다
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".FBX" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineFBXMesh::Load(Files[i].GetFullPath());
		}

*/

