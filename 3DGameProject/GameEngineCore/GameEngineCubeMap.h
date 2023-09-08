#pragma once

// Ό³Έν :
class GameEngineCubeMap
{
public:
	// constrcuter destructer
	GameEngineCubeMap();
	~GameEngineCubeMap();

	// delete Function
	GameEngineCubeMap(const GameEngineCubeMap& _Other) = delete;
	GameEngineCubeMap(GameEngineCubeMap&& _Other) noexcept = delete;
	GameEngineCubeMap& operator=(const GameEngineCubeMap& _Other) = delete;
	GameEngineCubeMap& operator=(GameEngineCubeMap&& _Other) noexcept = delete;

	//HRESULT D3DXCreateCubeTextureFromFileInMemory(
	//	_In_  LPDIRECT3DDEVICE9      pDevice,
	//	_In_  LPCVOID                pSrcData,
	//	_In_  UINT                   SrcDataSize,
	//	_Out_ LPDIRECT3DCUBETEXTURE9* ppCubeTexture
	//);

protected:

private:

};

