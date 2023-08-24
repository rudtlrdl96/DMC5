#include "PrecompileHeader.h"
#include "NetTestEnemy.h"

#include <GameEngineBase/GameEngineNet.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>

#include "NetworkManager.h"

#include "ObjectUpdatePacket.h"
#include "ServerWindow.h"
#include "ContentsEnum.h"

NetTestEnemy::NetTestEnemy()
{

}

NetTestEnemy::~NetTestEnemy()
{

}


void NetTestEnemy::Start()
{
	//서버와 통신할 때 이 엑터가 어떤 종류인지 정의하는 부분입니다.
	//ObjectUpdatePacket을 사용하기 위해서는 반드시 이 부분을 호출해주셔야 합니다
	SetNetObjectType(Net_ActorType::Nero);

	//이건 그냥 예시 렌더링
	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("House1.FBX", "NoneAlphaMesh");
}

void NetTestEnemy::Update_ProcessPacket()
{
	//패킷을 다 처리할 때 까지
	while (GameEngineNetObject::IsPacket())
	{
		//지금 처리할 패킷의 타입을 알아옵니다
		PacketEnum Type = GameEngineNetObject::GetFirstPacketType<PacketEnum>();

		switch (Type)
		{
			//업데이트 패킷의 경우엔
		case PacketEnum::ObjectUpdatePacket:
		{
			//패킷을 템플릿 포인터로 꺼내옵니다(Enum값과 포인터값을 맞게 해주셔야 하는 부분 유의부탁드려요)
			std::shared_ptr<ObjectUpdatePacket> ObjectUpdate = PopFirstPacket<ObjectUpdatePacket>();
			if (true == ObjectUpdate->IsDeath)
			{
				Death();
				break;
			}

			//패킷의 정보에 따라 자신의 값 수정
			GetTransform()->SetLocalPosition(ObjectUpdate->Position);
			GetTransform()->SetLocalRotation(ObjectUpdate->Rotation);
			ObjectUpdate->TimeScale;
			break;
		}
		default:
		{
			MsgAssert("처리하지 못하는 패킷이 플레이어로 날아왔습니다.");
			return;
		}
		}
	}
}

void NetTestEnemy::Update(float _DeltaTime)
{
	//몬스터의 경우 UserControll 일때만 FSM이 동작되어야 합니다.
	if (NetControllType::NetControll == GetControllType())
		return;


	//일반적인 Update부분입니다.
}

void NetTestEnemy::Update_SendPacket(float _DeltaTime)
{
	//NetworkManager를 통해서 업데이트 패킷을 보내면 됩니다.
	//그 외 패킷은 다른곳에서 보내도 상관없습니다.(아마도)
	NetworkManager::SendUpdatePacket(this, this, 1.f);

	//패킷을 보낼땐 모두 NetworkManager를 통해서 보낼 예정입니다.
	//추후 다양한 패킷 생성 예정
}
