#pragma once
#include "GameEngineComponent.h"
#include "GameEngineShader.h"

class GameEngineRenderUnit
	: std::enable_shared_from_this<GameEngineRenderUnit>
{
public:
	GameEngineShaderResHelper ShaderResHelper;
	std::shared_ptr<class GameEngineRenderingPipeLine> Pipe;

	GameEngineRenderUnit();
	void SetMesh(const std::string_view& _Name);
	void SetMesh(std::shared_ptr<class GameEngineMesh> _Mesh);
	void SetPipeLine(const std::string_view& _Name);
	void Render(float _DeltaTime);
	void SetRenderer(GameEngineRenderer* _Renderer);

private:
	GameEngineRenderer* ParentRenderer = nullptr;
	std::shared_ptr<class GameEngineInputLayOut> InputLayOutPtr;
	std::shared_ptr<class GameEngineMesh> Mesh;
};


class RenderBaseValue
{
public:
	float DeltaTime = 0.0f;
	float SumDeltaTime = 0.0f;
	int IsAnimation = 0;
	int IsNormal = 0;
	float4 ScreenScale;
	float4 Mouse;
};

// ���� :
class GameEngineRenderer : public GameEngineComponent
{
	friend class GameEngineCamera;
	friend class GameEngineRenderUnit;

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

	// void SetMesh(const std::string_view& _Name, int _index = 0);

	// ��������Ʈ�� �����.
	std::shared_ptr<GameEngineRenderUnit> CreateRenderUnit();

	// ���⼭ ���ϵ� ������������ �����ϸ� �� ������������ ����ϴ� ��� �ֵ��� �ٲ�� �ȴ�.
	std::shared_ptr<GameEngineRenderingPipeLine> GetPipeLine(int _index = 0);

	// �̰� ����ϰԵǸ� �� �������� ����Ʈ�� �ڽŸ��� Ŭ�� ������������ ������ �ȴ�.
	// std::shared_ptr<GameEngineRenderingPipeLine> GetPipeLineClone(int _index = 0);

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

	void RenderBaseValueUpdate(float _Delta);

	void PushCameraRender(int _CameraOrder);

	RenderBaseValue BaseValue;

private:
	bool IsCameraCulling = false;

	float CalZ = 0.0f;

	GameEngineCamera* RenderCamera;

	std::vector<std::shared_ptr<GameEngineRenderUnit>> Units;

	// Pipe��
	//// GameEngineShaderResHelper �� �������� ������ �� �Ǵ� ���� �˴ϴ�.
	//std::shared_ptr<class GameEngineRenderingPipeLine> Pipe;
	//GameEngineShaderResHelper ShaderResHelper;


	void RenderTransformUpdate(GameEngineCamera* _Camera);
};

