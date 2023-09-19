#pragma once
#include "GameEngineActor.h"

// ���� �޴� �༮�� ���� ���� �ʴ� �༮�� ���� ���̱� ������.

// �� ������ 1���� �� 1���Դϴ�.

enum class LightType
{
    Directional,
    Point,
    Spot
};

struct LightData
{
    friend class GameEngineLight;
    float4x4 LightViewMatrix;
    float4x4 LightViewInverseMatrix;
    float4x4 LightProjectionMatrix;
    float4x4 LightProjectionInverseMatrix;
    float4x4 LightViewProjectionMatrix;
    float4x4 CameraViewInverseMatrix;
    float4 LightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 AmbientLight = float4(0.2f, 0.2f, 0.2f, 1.0f);
    float4 LightPos;
    float4 LightDir;
    float4 LightRevDir;
    float4 ViewLightPos;
    float4 ViewLightDir;
    float4 ViewLightRevDir;
    float4 CameraPosition;
    float4 CameraWorldPosition;
    float ShadowTargetSizeX;
    float ShadowTargetSizeY;
    float LightNear;
    float LightFar;
    float DifLightPower = 1.0f;
    float SpcLightPower = 1.0f;
    float AmbLightPower = 1.0f;
    float SpcPow = 1.0f;
    int LightType = 0;
    float LightRange = 100.0f;
    float LightAngle = 10.0f;
    float LightPower = 1.0f;
    int Temp2;
    int Temp3;
};

struct LightDatas
{
    int LightCount = 0;
    LightData AllLight[64];
};

// ���� :
class GameEngineLight : public GameEngineActor
{
public:
    friend class GameEngineLevel;

    // constrcuter destructer
    GameEngineLight();
    ~GameEngineLight();

    // delete Function
    GameEngineLight(const GameEngineLight& _Other) = delete;
    GameEngineLight(GameEngineLight&& _Other) noexcept = delete;
    GameEngineLight& operator=(const GameEngineLight& _Other) = delete;
    GameEngineLight& operator=(GameEngineLight&& _Other) noexcept = delete;

    void Start() override;
    void Update(float _DeltaTime) override;
    void LightUpdate(GameEngineCamera* _Camera, float _DeltaTime);

    const LightData& GetLightData()
    {
        return LightDataValue;
    }
    
    std::shared_ptr<class GameEngineRenderTarget> GetShadowTarget()
    {
        return ShadowTarget;
    }

    std::shared_ptr<class GameEngineRenderTarget> GetBakeTarget(int _Index)
    {
        return BakeShadowTarget[_Index];
    }

    bool IsDebugDraw = false;

    LightData LightDataValue;

    void ShadowTargetTextureLoad(const float4 _ShadowScale = float4::ZERO);
    void ShadowTargetTextureRelease();

    inline bool IsShadow() const
    {
        return IsShadowLight;
    }

    void BakeShadow(std::shared_ptr<GameEngineCamera> _BakeCam, int _BakeIndex = 0);

    inline int GetBakeTargetIndex() const
    {
        return TargetBake;
    }

    inline void SetBakeTarget(int _Index)
    {
        TargetBake = _Index;
    }

    void SetShadowRange(const float4& _Value)
    {
        ShadowRange = _Value;
    }

    float4 GetShadowRange() const
    {
        return ShadowRange;
    }

    float4 ShadowRange;

    std::shared_ptr<class GameEngineRenderTarget> ShadowTarget = nullptr;

    std::map<int, std::shared_ptr<class GameEngineRenderTarget>> BakeShadowTarget;
    int TargetBake = 0;

    bool IsShadowLight = false;

    inline void SetLightType(LightType _Type)
    {
        LightDataValue.LightType = static_cast<int>(_Type);
    }
protected:

private:


    void DrawEditor() override;

};

