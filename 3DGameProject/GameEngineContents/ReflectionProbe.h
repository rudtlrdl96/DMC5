#pragma once
#include <GameEngineCore/GameEngineComponent.h>
#include <GameEngineCore/GameEngineRenderTarget.h>

class ReflectionProbe : public GameEngineComponent
{
private:
	class CubeCaptureData
	{
	public:
		float CutStartX;
		float CutEndX;
		float CutStartY;
		float CutEndY;
	};	
	
	static bool RenderTargetInit;
	static GameEngineRenderUnit CubemapMergeTarget;
	static CubeCaptureData CutData;

public:
	ReflectionProbe();
	~ReflectionProbe();

	ReflectionProbe(const ReflectionProbe& _Other) = delete;
	ReflectionProbe(ReflectionProbe&& _Other) noexcept = delete;
	ReflectionProbe& operator=(const ReflectionProbe& _Other) = delete;
	ReflectionProbe& operator=(ReflectionProbe&& _Other) noexcept = delete;

	// 캡쳐 텍스쳐의 이름이 겹치면 안됩니다. 반드시 Init을 호출해야 합니다.
	// Init 함수를 호출하기 전에 반드시 맵이 모두 생성되어야 하며 맵 컬링 기능이
	// Off 되어야 합니다 리플렉션 위치를 지정 후 호출하길 바랍니다.
	void Init(const std::string_view& _CaptureTextureName, const float4& _Scale = float4(128, 128));
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	
private:
	bool IsInitCheck = false;

	void CubemapMerge(const CubeCaptureData& _Cut, std::shared_ptr<GameEngineRenderTarget> _Target, std::shared_ptr<GameEngineRenderTarget> _Merge);

};

