#pragma once
#include "GameEngineResource.h"

// 설명 :
class GameEngineMaterial : public GameEngineResource<GameEngineMaterial>
{
public:
	// constrcuter destructer
	GameEngineMaterial();
	~GameEngineMaterial();

	// delete Function
	GameEngineMaterial(const GameEngineMaterial& _Other) = delete;
	GameEngineMaterial(GameEngineMaterial&& _Other) noexcept = delete;
	GameEngineMaterial& operator=(const GameEngineMaterial& _Other) = delete;
	GameEngineMaterial& operator=(GameEngineMaterial&& _Other) noexcept = delete;

	static std::shared_ptr<class GameEngineMaterial> Create(const std::string_view& _Name)
	{
		std::shared_ptr<class GameEngineMaterial> NewRes = GameEngineResource<GameEngineMaterial>::Create(_Name);
		return NewRes;
	}

	inline  std::shared_ptr<class GameEngineVertexShader> GetVertexShader()
	{
		return VertexShaderPtr;
	}

	inline  std::shared_ptr<class GameEnginePixelShader> GetPixelShader()
	{
		return PixelShaderPtr;
	}

	//void SetVertexBuffer(const std::string_view& _Value);
	//void SetIndexBuffer(const std::string_view& _Value);

	void SetVertexShader(const std::string_view& _Value);
	void SetRasterizer(const std::string_view& _Value);
	void SetPixelShader(const std::string_view& _Value);
	void SetBlendState(const std::string_view& _Value);
	void SetDepthState(const std::string_view& _Value);

	inline void SetFILL_MODE(D3D11_FILL_MODE _Value)
	{
		FILL_MODE = _Value;
	}

	void RenderingPipeLineSetting();
	void Render();

	std::shared_ptr<GameEngineMaterial> Clone();

	bool IsClone()
	{
		return IsCloneValue;
	}

protected:

private:
	bool IsCloneValue = false;
	// Directx11 랜더링 파이프라인의 단계에는 2가지 구분이 있는데.
	// 고정기능 단계 => 내가 옵션정도만 넘겨서 만드는 단계
	// 프로그래밍 가능 단계 => 특정 규칙만 지키면 나머지는 이제 내가 마음대로 짤수 있는 단계를 의미한다.
	// 쉐이더 계열은 다 프로그래밍 가능 단계 단계와 
	// 나머지들은 고정기능 단계라고 부른다.

	// void InputAssembler1();
	void VertexShader();
	// void InputAssembler2();
	void HullShader();
	void Tessellator();
	void DomainShader();
	void GeometryShaeder();
	void Rasterizer();
	void PixelShader();
	void OutputMerger();


	D3D11_FILL_MODE FILL_MODE = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	// std::shared_ptr<class GameEngineInputLayOut> InputLayOutPtr;
	std::shared_ptr<class GameEngineVertexBuffer> VertexBufferPtr;
	std::shared_ptr<class GameEngineIndexBuffer> IndexBufferPtr;
	std::shared_ptr<class GameEngineVertexShader> VertexShaderPtr;
	std::shared_ptr<class GameEngineRasterizer> RasterizerPtr;
	std::shared_ptr<class GameEnginePixelShader> PixelShaderPtr;
	std::shared_ptr<class GameEngineBlend> BlendStatePtr;
	std::shared_ptr<class GameEngineDepthState> DepthStatePtr;

};