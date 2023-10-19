#pragma once
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineLight.h>

// 설명 : 해당 랜더러는 Effect 전용 렌더러 입니다 UIFBXMesh또는 UIFBXAniMesh 머티리얼을 사용해야 합니다.
class UIMeshRenderer : public GameEngineFBXRenderer
{
public:
	class ClipData
	{
	public:
		float ClipStartX = 0.0f;
		float ClipEndX = 1.0f;
		float ClipStartY = 0.0f;
		float ClipEndY = 1.0f;
	};
public:
	UIMeshRenderer();
	~UIMeshRenderer();

	UIMeshRenderer(const UIMeshRenderer& _Other) = delete;
	UIMeshRenderer(UIMeshRenderer&& _Other) noexcept = delete;
	UIMeshRenderer& operator=(const UIMeshRenderer& _Other) = delete;
	UIMeshRenderer& operator=(UIMeshRenderer&& _Other) noexcept = delete;

	LightData LightDataValue;
	ClipData Clip;
	float4 HSVColor = { 0.5f, 0.5f, 0.5f, 0.0f };

	void LightDataUpdate();

	void SetFBXMesh(const std::string_view& _Name, const std::string_view& _Material) override
	{
		GameEngineFBXRenderer::SetFBXMesh(_Name, _Material);
		LightDataLink();
	}

	void SetFBXMesh(const std::string_view& _Name, const std::vector<std::vector<std::string>>& _Materials) override
	{
		GameEngineFBXRenderer::SetFBXMesh(_Name, _Materials);
		LightDataLink();
	}

	void SetFBXMesh(const std::string_view& _Name, const std::string_view& _Material, size_t MeshIndex) override
	{
		GameEngineFBXRenderer::SetFBXMesh(_Name, _Material, MeshIndex);
		LightDataLink();
	}

	void SetFBXMesh(const std::string_view& _Name, std::vector<std::string> _Material, size_t MeshIndex) override
	{
		GameEngineFBXRenderer::SetFBXMesh(_Name, _Material, MeshIndex);
		LightDataLink();
	}

	void HSVControl(float _saturation, float _brightness, float _contrast)
	{
		HSVColor.r = _saturation;
		HSVColor.g = _brightness;
		HSVColor.b = _contrast;
	}

protected:
	void Start() override;

private:

	void LightDataLink();

	void DrawEditor() override;

};