//--------------------------------------------------------------------------------------
// File: ScreenGrab11.h
//
// Function for capturing a 2D texture and saving it to a file (aka a 'screenshot'
// when used on a Direct3D 11 Render Target).
//
// Note these functions are useful as a light-weight runtime screen grabber. For
// full-featured texture capture, DDS writer, and texture processing pipeline,
// see the 'Texconv' sample and the 'DirectXTex' library.
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248926
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------
#pragma once

#include <d3d11_1.h>

#if defined(NTDDI_WIN10_FE) || defined(__MINGW32__)
#include <ocidl.h>
#else
#include <OCIdl.h>
#endif

#include <functional>

//#include <GameEngineCore/ThirdParty/DirectXTex/inc/DirectXTex.h>

//using namespace Microsoft::WRL;
//
//ComPtr<ID3D11Texture2D> backBuffer;
//HRESULT hr = swapChain->GetBuffer(0,
//	__uuidof(ID3D11Texture2D),
//	reinterpret_cast<LPVOID*>(backBuffer.GetAddressOf()));
//if (SUCCEEDED(hr))
//{
//	hr = SaveWICTextureToFile(immContext.Get(), backBuffer.Get(),
//		GUID_ContainerFormatJpeg, L"SCREENSHOT.JPG");
//}
//DX::ThrowIfFailed(hr);

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

	static HRESULT ScreenShoot(ID3D11Resource* _Resource);

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

protected:

private:

};