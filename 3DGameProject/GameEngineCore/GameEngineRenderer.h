#pragma once
#include "GameEngineComponent.h"
#include "GameEngineShader.h"

class GameEngineRenderUnit 
	: std::enable_shared_from_this<GameEngineRenderUnit>
{
public:
	GameEngineRenderUnit();

public:
	std::shared_ptr<class GameEngineInputLayOut> InputLayOutPtr;
	std::shared_ptr<class GameEngineMesh> Mesh;
	std::shared_ptr<class GameEngineRenderingPipeLine> Pipe;
	GameEngineShaderResHelper ShaderResHelper;

	// 

public:
	void SetMesh(const std::string_view& _Name);
	void SetPipeLine(const std::string_view& _Name);
	void Render(float _DeltaTime);
};


class RenderBaseValue 
{
public:
	float4 Time;
	float4 ScreenScale;
	float4 Mouse;
};

// ���� :
class GameEngineRenderer : public GameEngineComponent
{
	friend class GameEngineCamera;

public:
	// constrcuter destructer
	GameEngineRenderer();
	~GameEngineRenderer();

	// delete Function
	GameEngineRenderer(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer(GameEngineRenderer&& _Other) noexcept = delete;
	GameEngineRenderer& operator=(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer& operator=(GameEngineRenderer&& _Other) noexcept = delete;

	// ������������ ���õǾ� �ְ�
	// � ���̴��� ����ߴٴ� �� �˾ƾ� �ϰ�
	// �� ���̴����� � �ؽ�ó�� ����߰�
	// � ���÷� � ������۸� ����ߴ����� �˾ƾ� �Ѵ�.
	void SetPipeLine(const std::string_view& _Name, int _index = 0);

	void SetMesh(const std::string_view& _Name, int _index = 0);

	// ���⼭ ���ϵ� ������������ �����ϸ� �� ������������ ����ϴ� ��� �ֵ��� �ٲ�� �ȴ�.
	std::shared_ptr<GameEngineRenderingPipeLine> GetPipeLine(int _index = 0);

	// �̰� ����ϰԵǸ� �� �������� ����Ʈ�� �ڽŸ��� Ŭ�� ������������ ������ �ȴ�.
	std::shared_ptr<GameEngineRenderingPipeLine> GetPipeLineClone(int _index = 0);

	inline GameEngineShaderResHelper& GetShaderResHelper(int _index = 0) 
	{
		return Units[_index]->ShaderResHelper;
	}

	void CameraCullingOn() 
	{
		IsCameraCulling = true;
	}

	void CalSortZ(class GameEngineCamera* _Camera);

	GameEngineCamera* GetCamera() 
	{
		return RenderCamera;
	}

	// ������Ʈ���� �Ұ��̱� ������ �׳� �ϰڽ��ϴ�. 
	// ���� ���߿� ī�޶� �ٲٰų� �Ѵٸ� �̻��� ���� �߻��Ҽ� �ִ�.

protected:
	void Start();

	void Render(float _Delta) override;

	void PushCameraRender(int _CameraOrder);

private:
	bool IsCameraCulling = false;

	float CalZ = 0.0f;

	GameEngineCamera* RenderCamera;

	std::vector<std::shared_ptr<GameEngineRenderUnit>> Units;

	// Pipe��
	//// GameEngineShaderResHelper �� �������� ������ �� �Ǵ� ���� �˴ϴ�.
	//std::shared_ptr<class GameEngineRenderingPipeLine> Pipe;
	//GameEngineShaderResHelper ShaderResHelper;

	RenderBaseValue BaseValue;

	void RenderTransformUpdate(GameEngineCamera* _Camera);
};

