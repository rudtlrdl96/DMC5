#pragma once
#include "GameEngineComponent.h"
#include "GameEngineShader.h"

enum class RenderPath
{
	None,
	Forward,
	Deferred,
	Alpha,
	Coustom01,
	Coustom02,
	Coustom03,
	Debug,
};

class GameEngineRenderUnit : public GameEngineObjectBase, public std::enable_shared_from_this<GameEngineRenderUnit>
{
public:
	bool IsShadow = false;

	GameEngineShaderResHelper ShaderResHelper;
	std::shared_ptr<class GameEngineMaterial> Material;

	std::function<void(float)> RenderFunction;

	GameEngineRenderUnit();
	void SetMesh(const std::string_view& _Name);
	void SetMesh(std::shared_ptr<class GameEngineMesh> _Mesh);
	void SetMaterial(const std::string_view& _Name, RenderPath _Path = RenderPath::None);
	void Render(float _DeltaTime);
	void SetRenderer(class GameEngineRenderer* _Renderer);

	class GameEngineRenderer* GetRenderer()
	{
		return ParentRenderer;
	}

	void Setting();
	void Draw();

private:
	class GameEngineRenderer* ParentRenderer = nullptr;
	std::shared_ptr<class GameEngineInputLayOut> InputLayOutPtr;
	std::shared_ptr<class GameEngineMesh> Mesh;
};


class RenderBaseValue
{
public:
	float4 BaseColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	float DeltaTime = 0.0f;
	float SumDeltaTime = 0.0f;
	int IsAnimation = 0;
	int IsLight = 1;
	int IsNormal = 0;
	int IsSpecular = 0;
	float4 ScreenScale;
	float4 Mouse;
};

// 설명 :
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

	// 파이프라인이 세팅되어 있고
	// 어떤 쉐이더를 사용했다는 걸 알아야 하고
	// 그 쉐이더에서 어떤 텍스처를 사용했고
	// 어떤 샘플러 어떤 상수버퍼를 사용했는지를 알아야 한다.
	void SetMaterial(const std::string_view& _Name, int _index = 0);
	void SetMesh(const std::string_view& _Name, int _index = 0);

	std::shared_ptr<GameEngineRenderUnit> CreateRenderUnit(std::string_view _Mesh, std::string_view _Material);

	// void SetMesh(const std::string_view& _Name, int _index = 0);

	// 랜더유니트를 만든다.
	std::shared_ptr<GameEngineRenderUnit> CreateRenderUnit();

	std::shared_ptr<GameEngineRenderUnit> CreateRenderUnitToIndex(unsigned int _Index);

	// 여기서 리턴된 파이프라인을 수정하면 이 파이프라인을 사용하는 모든 애들이 바뀌게 된다.
	std::shared_ptr<GameEngineMaterial> GetMaterial(int _index = 0);

	// 이걸 사용하게되면 이 랜더러의 유니트는 자신만의 클론 파이프라인을 가지게 된다.
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

	// 업데이트에서 할것이기 때문에 그냥 하겠습니다. 
	// 랜더 도중에 카메라를 바꾸거나 한다면 이상한 일이 발생할수 있다.

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

	void ShadowOn(size_t _UnitIndex = -1)
	{
		if (_UnitIndex == -1)
		{
			for (size_t i = 0; i < Units.size(); i++)
			{
				Units[i]->IsShadow = true;
			}
			return;
		}

		Units[_UnitIndex]->IsShadow = true;
	}

	void ShadowOff(size_t _UnitIndex = -1)
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

	// Pipe와
	//// GameEngineShaderResHelper 가 합쳐져야 랜더링 이 되는 식이 됩니다.
	//std::shared_ptr<class GameEngineMaterial> Pipe;
	//GameEngineShaderResHelper ShaderResHelper;


	void RenderTransformUpdate(GameEngineCamera* _Camera);
};

