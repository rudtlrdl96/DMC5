#include "PrecompileHeader.h"
#include "UIFBXRenderer.h"
UIFBXRenderer::UIFBXRenderer() 
{
}

UIFBXRenderer::~UIFBXRenderer() 
{
}
void UIFBXRenderer::SetFBXMesh(const std::string_view& _Name, const std::string_view& _Material)
{
	GameEngineFBXRenderer::SetFBXMesh(_Name, _Material);
	if (true == GetShaderResHelper().IsConstantBuffer("ClipData"))
	{
		GetShaderResHelper().SetConstantBufferLink("ClipData", ClipData);
	}
}

void UIFBXRenderer::Start()
{

	PushCameraRender(100);
}

