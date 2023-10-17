#pragma once
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineLight.h>

// ���� : �ش� �������� Effect ���� ������ �Դϴ� UIFBXMesh�Ǵ� UIFBXAniMesh ��Ƽ������ ����ؾ� �մϴ�.
class UIMeshRenderer : public GameEngineFBXRenderer
{
public:
	UIMeshRenderer();
	~UIMeshRenderer();

	UIMeshRenderer(const UIMeshRenderer& _Other) = delete;
	UIMeshRenderer(UIMeshRenderer&& _Other) noexcept = delete;
	UIMeshRenderer& operator=(const UIMeshRenderer& _Other) = delete;
	UIMeshRenderer& operator=(UIMeshRenderer&& _Other) noexcept = delete;

	LightData LightDataValue;

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

protected:
	void Start() override;

private:
	void LightDataLink();

};