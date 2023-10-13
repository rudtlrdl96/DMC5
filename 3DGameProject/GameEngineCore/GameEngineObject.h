#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include "GameEngineObjectBase.h"
#include "GameEngineTransform.h"
#include <GameEngineCore/GameEngineNameObject.h>

// 설명 :
class GameEngineObject : 
	public GameEngineObjectBase, 
	public GameEngineNameObject,
	public std::enable_shared_from_this<GameEngineObject>
{
	friend class GameEngineComponent;
	friend class GameEngineActor;
	friend class GameEngineCore;
	friend class GameEngineTransform;
	friend class GameEngineLevel;

	friend class HierarchyGUI;

	static int NextActorID;
	static int GUI_SelectActorIndex;
	static GameEngineObject* ClickedObject;

public:
	// constrcuter destructer
	GameEngineObject();
	// 소멸자에 virtual붙이는 이유 => 자식소멸자가 제대로 호출되게 하기 위해서
	// 순수가상함수 쓰는 이유 => 자식에게 인터페이스를 강요하기 위해서
	// 순수가상함수 쓰는 이유 => 그 클래스를 객체화 할수 없게 만들기 위해서.
	// 소멸자를 순수가상함수를 만드는 이유? => 추상화 할만한게 딱히 없어서.
	virtual ~GameEngineObject() = 0;

	// delete Function
	GameEngineObject(const GameEngineObject& _Other) = delete;
	GameEngineObject(GameEngineObject&& _Other) noexcept = delete;
	GameEngineObject& operator=(const GameEngineObject& _Other) = delete;
	GameEngineObject& operator=(GameEngineObject&& _Other) noexcept = delete;

	GameEngineTransform* GetTransform()
	{
		return &Transform;
	}

	template<typename PtrType>
	std::shared_ptr<PtrType> DynamicThis()
	{
		return std::dynamic_pointer_cast<PtrType>(std::enable_shared_from_this<GameEngineObject>::shared_from_this());
	}

	virtual void AccLiveTime(float _LiveTime)
	{
		LiveTime += _LiveTime;
	}

	void ResetLiveTime()
	{
		LiveTime = 0.0f;
	}

	float GetLiveTime()
	{
		return LiveTime;
	}

	inline class GameEngineLevel* GetLevel() const
	{
		return Level;
	}

	bool IsDeath() override;
	bool IsUpdate() override;

	// 겹치지 않는 고유의 액터 ID를 반환합니다.
	inline int GetID() const
	{
		return ActorID;
	}

protected:
	virtual void Start() {}
	virtual void Update(float _DeltaTime) {}
	virtual void Render(float _DeltaTime) {}
	virtual void Destroy() {}
	virtual void Release();
	virtual void LevelChangeStart() {}
	virtual void LevelChangeEnd() {}

	void PushChild(std::shared_ptr<GameEngineObject> _Child)
	{
		Childs.push_back(_Child);
	}

private:
	float LiveTime = 0.0f;

	GameEngineTransform Transform;

	class GameEngineLevel* Level = nullptr;

	std::list<std::shared_ptr<GameEngineObject>> Childs;

	const int ActorID;

	bool PhysicsUpdate = true;

	/////////////////////  Gui 함수 /////////////////////

	GameEngineObject* DrawGUI();

	virtual void DrawEditor() {};

	////////////////////////////////////////////////////

	void AllAccTime(float _DeltaTime);
	void AllUpdate(float _DeltaTime);
	void AllRender(float _DeltaTime);
	void AllDestroy();
	void AllRelease();
	void AllLevelChangeStart();
	void AllLevelChangeEnd();
};
