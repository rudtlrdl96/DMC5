#pragma once
#include "GameEngineObject.h"
#include <string_view>
#include <map>

#include <GameEngineBase\GameEngineTimeEvent.h>

#include "GameEngineLight.h"
#include "GameEngineRenderTarget.h"
#include "PhysicsLevel.h"

// 설명 :
class GameEngineActor;
class GameEngineCamera;
class GameEngineRenderer;
class GameEngineCollision;
class GameEngineLevel : public GameEngineObject, public PhysicsLevel
{
	friend class GameEngineCamera;
	friend class GameEngineLight;
	friend class GameEngineRenderer;
	friend class GameEngineRenderUnit;
	friend class GameEngineCollision;
	friend class GameEngineTransform;
	friend class GameEngineCore;
	friend class GameEngineActor;
	friend class GameEngineTexture;
	friend class HierarchyGUI;

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

		ActorSetName(NewActor, Name);

		ActorInit(NewActor, _Order, this);

		return std::dynamic_pointer_cast<ActorType>(NewActor);
	}

	void ActorSetName(std::shared_ptr<GameEngineActor> _Actor, std::string _Name);

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

	std::shared_ptr<GameEngineRenderTarget> GetScreenShootTarget()
	{
		return ScreenShootTarget;
	}

	// 일부러 무겁게 만든 함수.
	template<typename EnumType>
	std::list<std::shared_ptr<GameEngineActor>> GetActorGroup(EnumType _Index)
	{
		return GetActorGroup(static_cast<int>(_Index));
	}

	std::list<std::shared_ptr<GameEngineActor>> GetActorGroup(int _Index) 
	{
		return Actors[_Index];
	}

	std::map<int, std::list<std::shared_ptr<GameEngineActor>>>& GetAllActor()
	{
		return Actors;
	}

	// DirectionalLight를 반환합니다.
	std::shared_ptr<GameEngineLight> GetDirectionalLight()
	{
		return DirectionalLight;
	}

	// Point Ligh를 생성합니다
	std::shared_ptr<GameEngineLight> CreatePointLight(const float4& _Pos, const float4& _ShadowScale, float _Range);

	// Spot Light를 생성합니다.
	std::shared_ptr<GameEngineLight> CreateSpotLight(const float4& _Pos, const float4& _ShadowScale, float _Range, float _Angle);

protected:
	// 레벨이 바뀌어서 시작할때
	virtual void LevelChangeStart();
	virtual void LevelChangeEnd();

	virtual void Start() = 0;
	void Update(float _DeltaTime) override;
	virtual void NetworkUpdate(float _DeltaTime) {};
	void Render(float _DeltaTime) override;

	void AllActorDestroy();

private:
	static bool IsDebugRender;

	// 모든 카메라의 내용이 다 종합된.
	std::shared_ptr<GameEngineRenderTarget> LastTarget;
	std::shared_ptr<GameEngineRenderTarget> ScreenShootTarget;

	//      이름           경로
	std::map<std::string, std::string> TexturePath;
	std::map<std::string, std::string> LoadEndPath;

	// DirectionalLight
	std::shared_ptr<GameEngineLight> DirectionalLight = nullptr;

	// 이미 뭔가가 다그려진 커다란 텍스처에 뭔가 변화를 주는것.

	// 카메라
	std::map<int, std::shared_ptr<GameEngineCamera>> Cameras;
	std::shared_ptr<GameEngineCamera> MainCamera;

	std::list<std::shared_ptr<GameEngineLight>> AllLight;

	LightDatas LightDataObject;

	void PushLight(std::shared_ptr<GameEngineLight> _Light);

	void PushCameraRenderer(std::shared_ptr<GameEngineRenderer> _Renderer, int _CameraOrder);

	std::map<int, std::list<std::shared_ptr<GameEngineActor>>> Actors;

	std::map<int, std::list<std::shared_ptr<GameEngineCollision>>> Collisions;

	void PushCollision(std::shared_ptr<GameEngineCollision> _Collision);

	void ActorInit(std::shared_ptr<GameEngineActor> _Actor, int _Order, GameEngineLevel* _Level);

	void ActorUpdate(float _DeltaTime);
	void ActorRelease();

	void ActorLevelChangeStart();
	void ActorLevelChangeEnd();

	void PostProcessLevelChangeStart();
	void PostProcessLevelChangeEnd();

	void DestroyCamera();

	void LevelCameraInit();
	void LevelLightInit();

	void TextureUnLoad(GameEngineLevel* _NextLevel);
	void TextureReLoad(GameEngineLevel* _PrevLevel);

	void RenderTargetTextureRelease();
	void RenderTargetTextureCreate();
};

 