#pragma once
#include <GameEngineCore/GameEngineFBXRenderer.h>
// 설명 :
class UIFBXRenderer :public GameEngineFBXRenderer
{
public:
	// constrcuter destructer
	UIFBXRenderer();
	~UIFBXRenderer();

	// delete Function
	UIFBXRenderer(const UIFBXRenderer& _Other) = delete;
	UIFBXRenderer(UIFBXRenderer&& _Other) noexcept = delete;
	UIFBXRenderer& operator=(const UIFBXRenderer& _Other) = delete;
	UIFBXRenderer& operator=(UIFBXRenderer&& _Other) noexcept = delete;
	void SetFBXMesh(const std::string_view& _Name, const std::string_view& _Material) override;
	void SetClipData(float4 _Clip)
	{
		ClipData.x = _Clip.x;
		ClipData.y = _Clip.y;
		ClipData.z = _Clip.z;
		ClipData.w = _Clip.w;

	}
protected:
	void Start() override;
private:
	//x는 startx
	//y는 endx
	//z는 starty
	//w는 endy
	float4 ClipData = { 0.0f,1.0f,0.0f,1.0f };

};

