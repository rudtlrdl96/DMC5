#pragma once
#include "GameEngineResource.h"

// ���� :
class GameEngineRenderingPipeLine : public GameEngineResource<GameEngineRenderingPipeLine>
{
public:
	// constrcuter destructer
	GameEngineRenderingPipeLine();
	~GameEngineRenderingPipeLine();

	// delete Function
	GameEngineRenderingPipeLine(const GameEngineRenderingPipeLine& _Other) = delete;
	GameEngineRenderingPipeLine(GameEngineRenderingPipeLine&& _Other) noexcept = delete;
	GameEngineRenderingPipeLine& operator=(const GameEngineRenderingPipeLine& _Other) = delete;
	GameEngineRenderingPipeLine& operator=(GameEngineRenderingPipeLine&& _Other) noexcept = delete;

	static std::shared_ptr<class GameEngineRenderingPipeLine> Create(const std::string_view& _Name)
	{
		std::shared_ptr<class GameEngineRenderingPipeLine> NewRes = GameEngineResource<GameEngineRenderingPipeLine>::Create(_Name);
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

	std::shared_ptr<GameEngineRenderingPipeLine> Clone();

	bool IsClone() 
	{
		return IsCloneValue;
	}

protected:

private:
	bool IsCloneValue = false;
	// Directx11 ������ ������������ �ܰ迡�� 2���� ������ �ִµ�.
	// ������� �ܰ� => ���� �ɼ������� �Ѱܼ� ����� �ܰ�
	// ���α׷��� ���� �ܰ� => Ư�� ��Ģ�� ��Ű�� �������� ���� ���� ������� ©�� �ִ� �ܰ踦 �ǹ��Ѵ�.
	// ���̴� �迭�� �� ���α׷��� ���� �ܰ� �ܰ�� 
	// ���������� ������� �ܰ��� �θ���.

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