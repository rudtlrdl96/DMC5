#pragma once

#include <d3d11_1.h>

#if defined(NTDDI_WIN10_FE) || defined(__MINGW32__)
#include <ocidl.h>
#else
#include <OCIdl.h>
#endif

#include <functional>

// Ό³Έν :
class GameEngineScreenShoot
{
public:
	// constrcuter destructer
	GameEngineScreenShoot();
	~GameEngineScreenShoot();

	// delete Function
	GameEngineScreenShoot(const GameEngineScreenShoot& _Other) = delete;
	GameEngineScreenShoot(GameEngineScreenShoot&& _Other) noexcept = delete;
	GameEngineScreenShoot& operator=(const GameEngineScreenShoot& _Other) = delete;
	GameEngineScreenShoot& operator=(GameEngineScreenShoot&& _Other) noexcept = delete;

	static HRESULT ScreenShoot();
	static HRESULT RenderTargetShoot(std::shared_ptr<GameEngineRenderTarget> _CaptureTarget, const std::string_view& _Path, const std::string_view& _TextureName);

protected:

private:
	static HRESULT __cdecl SaveDDSTextureToFile
	(
		_In_ ID3D11DeviceContext* pContext,
		_In_ ID3D11Resource* pSource,
		_In_z_ const wchar_t* fileName
	) noexcept;

	static HRESULT __cdecl SaveWICTextureToFile
	(
		_In_ ID3D11DeviceContext* pContext,
		_In_ ID3D11Resource* pSource,
		_In_ REFGUID guidContainerFormat,
		_In_z_ const wchar_t* fileName,
		_In_opt_ const GUID* targetFormat = nullptr,
		_In_opt_ std::function<void __cdecl(IPropertyBag2*)> setCustomProps = nullptr,
		_In_ bool forceSRGB = false
	);

};