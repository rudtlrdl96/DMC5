#pragma once
#include <GameEngineCore/GameEngineComponent.h>
#include <GameEngineCore/GameEngineRenderTarget.h>

class ReflectionProbe : public GameEngineComponent
{
private:	
	static std::shared_ptr<GameEngineRenderTarget> CaptureTarget;

public:
	ReflectionProbe();
	~ReflectionProbe();

	ReflectionProbe(const ReflectionProbe& _Other) = delete;
	ReflectionProbe(ReflectionProbe&& _Other) noexcept = delete;
	ReflectionProbe& operator=(const ReflectionProbe& _Other) = delete;
	ReflectionProbe& operator=(ReflectionProbe&& _Other) noexcept = delete;

	// ĸ�� �ؽ����� �̸��� ��ġ�� �ȵ˴ϴ�. �ݵ�� Init�� ȣ���ؾ� �մϴ�.
	// Init �Լ��� ȣ���ϱ� ���� �ݵ�� ���� ��� �����Ǿ�� �ϸ� �� �ø� �����
	// Off �Ǿ�� �մϴ� ���÷��� ��ġ�� ���� �� ȣ���ϱ� �ٶ��ϴ�.
	void Init(const std::string_view& _CaptureTextureName, const float4& _Scale = float4(128, 128));
	
	inline std::shared_ptr<GameEngineTexture> GetReflectionCubeTexture() const
	{
		return ReflectionCubeTexture;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	
private:
	std::shared_ptr<GameEngineTexture> ReflectionCubeTexture = nullptr;
};

