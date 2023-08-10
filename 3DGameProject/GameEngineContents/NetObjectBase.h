#pragma once
#include <GameEngineBase/GameEngineNetObject.h>
#include <GameEngineCore/GameEngineActor.h>

class NetObjectBase : public GameEngineActor, public GameEngineNetObject
{
public:
	NetObjectBase();
	~NetObjectBase();

	NetObjectBase(const NetObjectBase& _Other) = delete;
	NetObjectBase(NetObjectBase&& _Other) noexcept = delete;
	NetObjectBase& operator=(const NetObjectBase& _Other) = delete;
	NetObjectBase& operator=(const NetObjectBase&& _Other) noexcept = delete;

protected:
	void Update(float _DeltaTime) override;
	virtual void MyUpdate(float _DeltaTime){}
	virtual void OtherUpdate(float _DeltaTime){}

private:

};

