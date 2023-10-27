#include "PrecompileHeader.h"
#include "GameEngineCaptureTexture.h"

std::map<std::string, std::shared_ptr<GameEngineRenderTarget>> GameEngineCaptureTexture::CaptureTarget;

std::shared_ptr<GameEngineRenderTarget> GameEngineCaptureTexture::CaptureTexture(const std::string_view& _CaptureName, const float4& _Scale, std::shared_ptr<GameEngineRenderTarget> _CaptureTarget, size_t _Index)
{
	std::string UpperName = GameEngineString::ToUpper(_CaptureName);

	std::shared_ptr<GameEngineRenderTarget> NewTarget = CaptureTarget[UpperName]; 
	
	if (nullptr == NewTarget)
	{
		NewTarget = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, _Scale, float4::ZERONULL);
		CaptureTarget[UpperName] = NewTarget;
	}
	
	NewTarget->Clear();
	NewTarget->Merge(_CaptureTarget, _Index);

	return NewTarget;
}

std::shared_ptr<GameEngineRenderTarget> GameEngineCaptureTexture::FindTexture(const std::string_view& _CaptureName)
{
	std::string UpperName = GameEngineString::ToUpper(_CaptureName);
	std::shared_ptr<GameEngineRenderTarget> FindTarget = CaptureTarget[UpperName];

	return FindTarget;
}

void GameEngineCaptureTexture::ReleaseTexture(const std::string_view& _CaptureName)
{
	std::string UpperName = GameEngineString::ToUpper(_CaptureName);
	std::shared_ptr<GameEngineRenderTarget> ReleaseTarget = CaptureTarget[UpperName];

	if (nullptr != ReleaseTarget)
	{
		ReleaseTarget->ReleaseTextures();
	}

	CaptureTarget.erase(UpperName);
}

void GameEngineCaptureTexture::ClearAllTexture()
{
	std::map<std::string, std::shared_ptr<GameEngineRenderTarget>>::iterator LoopIter = CaptureTarget.begin();
	std::map<std::string, std::shared_ptr<GameEngineRenderTarget>>::iterator EndIter = CaptureTarget.end();

	for (; LoopIter != EndIter; ++LoopIter)
	{
		if (nullptr != LoopIter->second)
		{
			LoopIter->second->ReleaseTextures();
		}
	}

	CaptureTarget.clear();
}

GameEngineCaptureTexture::GameEngineCaptureTexture()
{
}

GameEngineCaptureTexture::~GameEngineCaptureTexture()
{
}
