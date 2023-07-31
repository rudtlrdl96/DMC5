#pragma once
#include "GameEngineObject.h"
#include <GameEngineBase\GameEngineTimeEvent.h>
#include <string_view>
#include <map>
#include <GameEngineCore/GameEngineRenderTarget.h>

// ���� :
class GameEngineActor;
class GameEngineCamera;
class GameEngineRenderer;
class GameEngineCollision;
class GameEngineLevel : public GameEngineObject
{
	friend class GameEngineRenderer;
	friend class GameEngineCollision;
	friend class GameEngineTransform;
	friend class GameEngineCore;
	friend class GameEngineActor;
	friend class GameEngineTexture;

public:
	static void IsDebugSwitch() 
	{
		IsDebugRender = !IsDebugRender;
	}

	GameEngineTimeEvent TimeEvent;

	// constrcuter destructer
	GameEngineLevel();
	virtual ~GameEngineLevel();

	// delete Function
	GameEngineLevel(const GameEngineLevel& _Other) = delete;
	GameEngineLevel(GameEngineLevel&& _Other) noexcept = delete;
	GameEngineLevel& operator=(const GameEngineLevel& _Other) = delete;
	GameEngineLevel& operator=(GameEngineLevel&& _Other) noexcept = delete;

	template<typename ActorType>
	std::shared_ptr<ActorType> CreateActorToName(const std::string_view& _Name = "")
	{
		return CreateActor<ActorType>(0, _Name);
	}

	template<typename ActorType, typename EnumType>
	std::shared_ptr<ActorType> CreateActor(EnumType  _Order, const std::string_view& _Name = "")
	{
		return CreateActor<ActorType>(static_cast<int>(_Order), _Name);
	}

	template<typename ActorType >
	std::shared_ptr<ActorType> CreateActor(int _Order = 0, const std::string_view& _Name = "")
	{
		std::shared_ptr<GameEngineActor> NewActor = std::make_shared<ActorType>();

		std::string Name = _Name.data();

		if (_Name == "")
		{
			const type_info& Info = typeid(ActorType);
			Name = Info.name();
			Name.replace(0, 6, "");
		}

		ActorInit(NewActor, _Order, this);


		return std::dynamic_pointer_cast<ActorType>(NewActor);
	}

	std::shared_ptr<class GameEngineCamera> CreateNewCamera(int _Order);

	std::shared_ptr<class GameEngineCamera> GetMainCamera() 
	{
		return MainCamera;
	}

	std::shared_ptr<GameEngineLevel> GetSharedThis()
	{
		return DynamicThis<GameEngineLevel>();
	}

	std::shared_ptr<GameEngineCamera> GetCamera(int _CameraOrder);

	std::shared_ptr<GameEngineRenderTarget> GetLastTarget() 
	{
		return LastTarget;
	}

	// �Ϻη� ���̰� ���� �Լ�.
	template<typename EnumType>
	std::list<std::shared_ptr<GameEngineActor>> GetActorGroup(EnumType _Index)
	{
		return GetActorGroup(static_cast<int>(_Index));
	}

	std::list<std::shared_ptr<GameEngineActor>> GetActorGroup(int _Index) 
	{
		return Actors[_Index];
	}


protected:
	// ������ �ٲ� �����Ҷ�
	virtual void LevelChangeStart();
	virtual void LevelChangeEnd();

	virtual void Start() = 0;
	void Update(float _DeltaTime);
	void Render(float _DeltaTime);

	void AllActorDestroy();
private:
	static bool IsDebugRender;

	// ��� ī�޶��� ������ �� ���յ�.
	std::shared_ptr<GameEngineRenderTarget> LastTarget;

	//      �̸�           ���
	std::map<std::string, std::string> TexturePath;
	std::map<std::string, std::string> LoadEndPath;

	// �̹� ������ �ٱ׷��� Ŀ�ٶ� �ؽ�ó�� ���� ��ȭ�� �ִ°�.

	// ī�޶�
	std::map<int, std::shared_ptr<GameEngineCamera>> Cameras;
	std::shared_ptr<GameEngineCamera> MainCamera;

	void PushCameraRenderer(std::shared_ptr<GameEngineRenderer> _Renderer, int _CameraOrder);

	std::map<int, std::list<std::shared_ptr<GameEngineActor>>> Actors;

	std::map<int, std::list<std::shared_ptr<GameEngineCollision>>> Collisions;

	void PushCollision(std::shared_ptr<GameEngineCollision> _Collision);

	void ActorInit(std::shared_ptr<GameEngineActor> _Actor, int _Order, GameEngineLevel* _Level);

	void ActorUpdate(float _DeltaTime);
	void ActorRender(float _DeltaTime);
	void ActorRelease();
	void ActorLevelChangeStart();
	void ActorLevelChangeEnd();

	void LevelCameraInit();

	void DestroyCamera();


	void TextureUnLoad(GameEngineLevel* _NextLevel);

	void TextureReLoad(GameEngineLevel* _PrevLevel);

};

 