#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>


class ObjectUpdatePacket : public GameEnginePacket
{
public:
	ObjectUpdatePacket();
	~ObjectUpdatePacket();

	ObjectUpdatePacket(const ObjectUpdatePacket& _Other) = delete;
	ObjectUpdatePacket(ObjectUpdatePacket&& _Other) noexcept = delete;
	ObjectUpdatePacket& operator=(const ObjectUpdatePacket& _Other) = delete;
	ObjectUpdatePacket& operator=(const ObjectUpdatePacket&& _Other) noexcept = delete;

protected:

private:
	float4 Pos;
	int Animation;
};

