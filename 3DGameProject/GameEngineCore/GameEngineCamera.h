#pragma once
#include "GameEngineEnum.h"
#include "GameEngineActor.h"
#include "GameEngineRenderer.h"
#include <list>
#include <map>

enum class SortType
{
	None,
	ZSort,
	YSort,
};

// ���� :
class GameEngineRenderer;
class GameEngineRenderTarget;
class GameEngineCamera : public GameEngineActor
{
	friend class GameEngineLevel;
	friend class GameEngineRenderer;
	friend class GameEngineRenderUnit;

public:
	// constrcuter destructer
	GameEngineCamera();
	~GameEngineCamera();

	// delete Function
	GameEngineCamera(const GameEngineCamera& _Other) = delete;
	GameEngineCamera(GameEngineCamera&& _Other) noexcept = delete;
	GameEngineCamera& operator=(const GameEngineCamera& _Other) = delete;
	GameEngineCamera& operator=(GameEngineCamera&& _Other) noexcept = delete;

	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;
	void CameraTransformUpdate();

	void ViewPortSetting();

	void Setting();

	bool IsView(const TransformData& _TransData);

	std::shared_ptr<GameEngineRenderTarget> GetCamTarget()
	{
		return CamTarget;
	}

	std::shared_ptr<GameEngineRenderTarget> GetCamForwardTarget()
	{
		return CamForwardTarget;
	}

	std::shared_ptr<GameEngineRenderTarget> GetCamDeferrdTarget()
	{
		return CamDeferrdTarget;
	}

	std::shared_ptr<GameEngineRenderTarget> GetDeferredLightTarget()
	{
		return DeferredLightTarget;
	}

	std::shared_ptr<GameEngineRenderTarget> GetCamAllRenderTarget()
	{
		return AllRenderTarget;
	}

	std::shared_ptr<GameEngineRenderTarget> GetCubeRenderTarget()
	{
		return CubeRenderTarget;
	}

	inline float4x4 GetView()
	{
		return View;
	}

	inline float4x4 GetProjection()
	{
		return Projection;
	}

	inline float4x4 GetViewPort()
	{
		return ViewPort;
	}

	void SetProjectionType(CameraType _Type)
	{
		ProjectionType = _Type;
	}

	inline bool IsFreeCamera()
	{
		return FreeCamera;
	}

	template<typename EnumType>
	void SetSortType(EnumType _Index, SortType _Sort)
	{
		SetSortType(static_cast<int>(_Index), _Sort);
	}

	void SetSortType(int _Index, SortType _Sort)
	{
		SortValues[_Index] = _Sort;
	}

	inline void SetZoomRatio(float _Value)
	{
		ZoomRatio = _Value;
	}

	inline void AddZoomRatio(float _Value)
	{
		ZoomRatio -= _Value;
	}

	inline float GetZoomRatio() const
	{
		return ZoomRatio;
	}

	void CaptureCubemap(const float4& _Pos, const float4& _Rot, const float4& _CaptureScale = float4(128, 128));

protected:
	void Start() override;

private:
	std::map<RenderPath, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>> Units;

	std::map<int, std::list<std::shared_ptr<GameEngineRenderer>>> Renderers;
	std::map<int, SortType> SortValues;

	DirectX::BoundingOrientedBox Box;

	bool FreeCamera = false;
	bool IsLoad = false;

	float ZoomRatio = 1.0f;

	float4x4 View;
	float4x4 Projection;
	float4x4 ViewPort;

	TransformData OldData;

	CameraType ProjectionType = CameraType::None;

	D3D11_VIEWPORT ViewPortData;

	float Width = 0.0f;
	float Height = 0.0f;

	float FOV = 90.0f;
	float Near = 0.1f;
	float Far = 100000.0f;

	void PushRenderer(std::shared_ptr<class GameEngineRenderer> _Render);
	void PushRenderUnit(std::shared_ptr<GameEngineRenderUnit> _Unit, RenderPath _Path = RenderPath::None);

	void Release();

	std::shared_ptr<class GameEngineRenderTarget> CamTarget;
	std::shared_ptr<class GameEngineRenderTarget> CamForwardTarget;
	std::shared_ptr<class GameEngineRenderTarget> CamDeferrdTarget;
	std::shared_ptr<class GameEngineRenderTarget> CamAlphaTarget;
	std::shared_ptr<class GameEngineRenderTarget> AllRenderTarget;
	std::shared_ptr<class GameEngineRenderTarget> CubeRenderTarget;

	// ������� ������� �ޱ� ���� Ÿ��.
	std::shared_ptr<class GameEngineRenderTarget> DeferredLightTarget;

	GameEngineRenderUnit CalLightUnit;
	GameEngineRenderUnit DefferdMergeUnit;

	void FreeCameraSwitch();

	void RenderTargetTextureLoad();
	void RenderTargetTextureRelease();
};

