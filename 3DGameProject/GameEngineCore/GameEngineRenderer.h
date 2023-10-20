#pragma once
#include "GameEngineComponent.h"
#include "GameEngineShader.h"

enum class RenderPath
{
	None,
	Forward,
	Deferred,
	Alpha,
};

class GameEngineComputeUnit : public GameEngineObjectBase, public std::enable_shared_from_this<GameEngineComputeUnit>
{
public:
	GameEngineShaderResHelper ShaderResHelper;
	std::shared_ptr<class GameEngineComputeShader> ComputeShader = nullptr;

	// ��
	UINT                            m_iGroupX = 0;
	UINT                            m_iGroupY = 0;
	UINT                            m_iGroupZ = 0;

	// 128
	UINT                      m_iGroupPerThreadX = 1;
	UINT                      m_iGroupPerThreadY = 1;
	UINT                      m_iGroupPerThreadZ = 1;

	void SetComputeShader(const std::string_view& _Name);
	void Execute();
};

class GameEngineRenderUnit : public GameEngineObjectBase, public std::enable_shared_from_this<GameEngineRenderUnit>
{
public:
	bool IsShadow = false;
	bool IsStatic = false;

	GameEngineShaderResHelper ShaderResHelper;

	std::shared_ptr<class GameEngineMaterial> Material = nullptr;
	std::function<void(float)> RenderFunction = nullptr;

	GameEngineRenderUnit();
	void SetMesh(const std::string_view& _Name);
	void SetMesh(std::shared_ptr<class GameEngineMesh> _Mesh);
	void SetMaterial(const std::string_view& _Name, RenderPath _Path = RenderPath::None, bool _IsRender = true);

	void Render(float _DeltaTime);
	void SetRenderer(class GameEngineRenderer* _Renderer);

	class GameEngineRenderer* GetRenderer()
	{
		return ParentRenderer;
	}

	void Setting();
	void Draw();
	void Draw(UINT _IndexCount, UINT _IndexStart);

	void ShadowOn();
	void ShadowOff();
	void ShadowSetting();

private:
	class GameEngineRenderer* ParentRenderer = nullptr;
	std::shared_ptr<class GameEngineInputLayOut> InputLayOutPtr = nullptr;
	std::shared_ptr<class GameEngineMesh> Mesh = nullptr;
	std::shared_ptr<class GameEngineInputLayOut> ShadowInputLayOutPtr = nullptr;
};

class RenderBaseValue
{
public:
	float4 BaseColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	float4 AddColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	float4 MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	float DeltaTime = 0.0f;
	float SumDeltaTime = 0.0f;
	int IsAnimation = 0;
	int IsLight = 1;
	int IsNormal = 0;
	int IsSpecular = 0;
	float4 ScreenScale;
	float4 Mouse;
	float4 NoiseResolution;
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
	void SetMaterial(const std::string_view& _Name, int _index = 0);
	void SetMesh(const std::string_view& _Name, int _index = 0);

	std::shared_ptr<class GameEngineRenderUnit> CreateRenderUnit(std::string_view _Mesh, std::string_view _Material);

	// void SetMesh(const std::string_view& _Name, int _index = 0);

	// ��������Ʈ�� �����.
	std::shared_ptr<class GameEngineRenderUnit> CreateRenderUnit();

	std::shared_ptr<class GameEngineRenderUnit> CreateRenderUnitToIndex(unsigned int _Index);

	// ���⼭ ���ϵ� ������������ �����ϸ� �� ������������ ����ϴ� ��� �ֵ��� �ٲ�� �ȴ�.
	std::shared_ptr<class GameEngineMaterial> GetMaterial(int _index = 0);

	// �̰� ����ϰԵǸ� �� �������� ����Ʈ�� �ڽŸ��� Ŭ�� ������������ ������ �ȴ�.
	// std::shared_ptr<GameEngineMaterial> GetPipeLineClone(int _index = 0);

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

	RenderBaseValue& GetRenderBaseValueRef()
	{
		return BaseValue;
	}

	inline void LightOn()
	{
		BaseValue.IsLight = true;
	}

	inline void LightOff()
	{
		BaseValue.IsLight = false;
	}

	std::shared_ptr<GameEngineRenderUnit> GetUnit(unsigned int _Index = 0)
	{
		if (_Index >= Units.size())
		{
			return nullptr;
		}

		return Units[_Index];
	}

	virtual void ShadowOn(size_t _UnitIndex = -1)
	{
		if (_UnitIndex == -1)
		{
			for (size_t i = 0; i < Units.size(); i++)
			{
				Units[i]->ShadowOn();
			}
			return;
		}

		Units[_UnitIndex]->ShadowOn();
	}

	virtual void ShadowOff(size_t _UnitIndex = -1)
	{
		if (_UnitIndex == -1)
		{
			for (size_t i = 0; i < Units.size(); i++)
			{
				Units[i]->IsShadow = false;
			}
			return;
		}

		Units[_UnitIndex]->IsShadow = false;
	}

	inline float4 GetBaseColor() const
	{
		return BaseValue.BaseColor;
	}

	inline void SetBaseColor(const float4& _Color)
	{
		BaseValue.BaseColor = _Color;
	}

	inline void SetAddColor(const float4& _Color)
	{
		BaseValue.AddColor = _Color;
	}

	inline float4 GetAddColor() const
	{
		return BaseValue.AddColor;
	}

	inline void SetMulColor(const float4& _Color)
	{
		BaseValue.MulColor = _Color;
	}

	inline float4 GetMulColor() const
	{
		return BaseValue.MulColor;
	}



protected:
	void Start();

	void Render(float _Delta) override;

	void RenderBaseValueUpdate(float _Delta);

	void PushCameraRender(int _CameraOrder);

	RenderBaseValue BaseValue;

private:
	bool IsCameraCulling = false;

	float CalZ = 0.0f;

	GameEngineCamera* RenderCamera = nullptr;

	std::vector<std::shared_ptr<GameEngineRenderUnit>> Units;

	// Pipe��
	//// GameEngineShaderResHelper �� �������� ������ �� �Ǵ� ���� �˴ϴ�.
	//std::shared_ptr<class GameEngineMaterial> Pipe;
	//GameEngineShaderResHelper ShaderResHelper;


	void RenderTransformUpdate(GameEngineCamera* _Camera);
};

