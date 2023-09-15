#pragma once
#include <GameEngineCore/GameEngineFBXRenderer.h>

class ClipData
{
public:
	float ClipStartX = 0;
	float ClipEndX = 1;
	float ClipStartY = 0;
	float ClipEndY = 1;
};

// Ό³Έν :
class EffectFBXRenderer : public GameEngineFBXRenderer
{
public:
	// constrcuter destructer
	EffectFBXRenderer();
	~EffectFBXRenderer();

	// delete Function
	EffectFBXRenderer(const EffectFBXRenderer& _Other) = delete;
	EffectFBXRenderer(EffectFBXRenderer&& _Other) noexcept = delete;
	EffectFBXRenderer& operator=(const EffectFBXRenderer& _Other) = delete;
	EffectFBXRenderer& operator=(EffectFBXRenderer&& _Other) noexcept = delete;

	ClipData Clip = ClipData();

	void SetFBXMesh(const std::string& _Name, std::string _Material) override;
	void SetFBXMesh(const std::string& _Name, const std::vector<std::vector<std::string>>& _Materials) override;

	void SetFBXMesh(const std::string& _Name, std::string _Material, size_t MeshIndex)  override;
	void SetFBXMesh(const std::string& _Name, std::vector<std::string> _Material, size_t MeshIndex) override;

protected:
	void Start() override;

private:
	void ClipSetting();	
	void DrawEditor() override;
};

