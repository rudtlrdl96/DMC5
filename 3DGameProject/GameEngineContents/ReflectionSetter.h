#pragma once
#include "FieldMapObject.h"
#include "ReflectionProbe.h"

class ReflectionSetter : public FieldMapObject
{
public:
	// construtor destructor
	ReflectionSetter();
	~ReflectionSetter();

	// delete Function
	ReflectionSetter(const ReflectionSetter& _Other) = delete;
	ReflectionSetter(ReflectionSetter&& _Other) noexcept = delete;
	ReflectionSetter& operator=(const ReflectionSetter& _Other) = delete;
	ReflectionSetter& operator=(ReflectionSetter&& _Other) noexcept = delete;

	//���� Actor�� Transform�� �����ؾ���
	void Init(const std::string_view& _CaptureTextureName, const float4& _Scale = float4(128, 128))
	{
		if (Probe == nullptr)
		{
			MsgAssert("ReflectionProbe�� nullptr�Դϴ�");
		}

		Probe->Init(_CaptureTextureName, _Scale);
	}

	inline std::shared_ptr<GameEngineTexture> GetReflectionCubeTexture() const
	{
		if (Probe == nullptr)
		{
			MsgAssert("ReflectionProbe�� nullptr�Դϴ�");
		}
		return Probe->GetReflectionCubeTexture();
	}


protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<ReflectionProbe> Probe = nullptr;
};

