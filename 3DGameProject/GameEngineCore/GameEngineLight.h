#pragma once
#include "GameEngineActor.h"

// 빛을 받는 녀석과 빛을 받지 않는 녀석이 있을 것이기 때문에.

// 이 데이터 1개가 빛 1개입니다.

enum class LightType
{
    Directional,
    Point,
    Spot
};

struct LightViewData
{
    float4x4 LightViewMatrix;
    float4x4 LightViewInverseMatrix;
    float4x4 LightProjectionMatrix;
    float4x4 LightProjectionInverseMatrix;
    float4x4 LightViewProjectionMatrix;
};

struct LightData
{
    friend class GameEngineLight;
    float4x4 LightViewMatrix;
    float4x4 LightViewInverseMatrix;
    float4x4 LightProjectionMatrix;
    float4x4 LightProjectionInverseMatrix;
    float4x4 LightViewProjectionMatrix;
    float4x4 CameraView;
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

enum class ShadowTextureScale
{
    S_64,
    S_128,
    S_256,
    S_512,
    S_1024,
    S_2048,
    S_4096,
    S_8192,
};

// 설명 :
class GameEngineLight : public GameEngineActor
{
    static const float4 PointViewDatas[6][2];
    static const float4 ShadowTextureScales[8];
    static const std::string ShadowTextureEnums[8];
public:
    friend class GameEngineLevel;
    friend class GameEngineCamera;

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

    bool IsDebugDraw = false;

    inline void DynamicShadowOn()
    {
        IsDynamicLight = true;
    }

    const LightData& GetLightData()
    {
        return LightDataValue;
    }
    
    // 그림자를 사용하는 라이트인지 반환합니다.
    inline bool IsShadow() const
    {
        return IsShadowLight;
    }

    // 설정된 카메라를 기준 그림자를 Bake 합니다.
    void BakeShadow(std::shared_ptr<GameEngineCamera> _BakeCam, int _BakeIndex = 0);

    // 현재 설정된 BakeTarget Index를 반환합니다
    inline int GetBakeTargetIndex() const
    {
        return TargetBake;
    }

    // Bake 타켓의 Index를 설정합니다
    inline void SetBakeTarget(int _Index)
    {
        TargetBake = _Index;
    }

    // Shadow Last Target을 반환합니다.
    std::shared_ptr<class GameEngineRenderTarget> GetShadowTarget()
    {
        return ShadowTarget;
    }

    // 그림자 Bake Target을 반환합니다.
    std::shared_ptr<class GameEngineRenderTarget> GetBakeTarget(int _Index)
    {
        return BakeShadowTarget[_Index];
    }

    void SetShadowTextureScale(ShadowTextureScale _Scale);

    inline float4 GetShadowTextureScale() const
    {
        float4(LightDataValue.ShadowTargetSizeX, LightDataValue.ShadowTargetSizeY);
    }

    // 그림자 적용 범위를 설정합니다. [Diffuse Light에서만 동작합니다.]
    inline void SetShadowScale(const float4& _Scale)
    {
        ShadowRange = _Scale;
    }
    
    // 현재 그림자 적용 범위를 반환합니다. [Diffuse Light에서만 동작합니다.]
    float4 GetShadowScale() const
    {
        return ShadowRange;
    }

    // 빛 거리 변경
    void SetLightRange(float _Range);

    // 현재 빛 거리를 반환합니다.
    inline float GetLightRange() const
    {
        return LightDataValue.LightRange;
    }

    // Lignt Angle을 설정합니다 [SpotLight 에서만 동작합니다]
    inline void SetLightAngle(float _Angle)
    {
        LightDataValue.LightAngle = _Angle;
    }

    // 현재 Light Angle을 반환합니다. [SpotLight 에서만 동작합니다]
    inline float GetLightAngle() const
    {
        return LightDataValue.LightAngle;
    }

    // 현재 Light의 Color를 반환합니다.
    inline void SetLightColor(const float4& _Color)
    {
        LightDataValue.LightColor = _Color;
    }

    // 라이트 전체 Power 변경
    inline void SetLightPower(float _Power)
    {
        LightDataValue.LightPower = _Power;
    }

    // 현재 Light의 Power를 반환합니다
    inline float GetLightPower() const
    {
        return LightDataValue.LightPower;
    }

    // Diffuse Light의 Power를 설정합니다
    inline void SetDifLightPower(float _DifPower)
    {
        LightDataValue.DifLightPower = _DifPower;
    }

    // 현재 Diffuse Light의 Power를 반환합니다
    inline float GetDifLightPower() const
    {
        return LightDataValue.DifLightPower;
    }

    // Spacular Light의 Power를 설정합니다
    inline void SetSpcLightPower(float _SpcPower)
    {
        LightDataValue.SpcLightPower = _SpcPower;
    }

    // 현재 Spacular Light의 Power를 반환합니다
    inline float GetSpcLightPower() const
    {
        return LightDataValue.SpcLightPower;
    }

    // Ambient Light의 Power를 설정합니다
    inline void SetAmbLightPower(float _AmbPower)
    {
        LightDataValue.AmbLightPower = _AmbPower;
    }

    // 현재 Ambient Light의 Power를 반환합니다
    inline float GetAmbLightPower() const
    {
        return LightDataValue.AmbLightPower;
    }
protected:

private:
    float4 ShadowRange;
    ShadowTextureScale ShadowTextureScaleValue = ShadowTextureScale::S_256;

    std::shared_ptr<class GameEngineRenderTarget> ShadowTarget = nullptr;
    std::vector<LightViewData> ViewDatas;

    LightData LightDataValue;

    std::map<int, std::shared_ptr<class GameEngineRenderTarget>> BakeShadowTarget;
    int TargetBake = 0;

    bool IsShadowLight = false;
    bool IsDynamicLight = false;

    inline void SetLightType(LightType _Type)
    {
        LightDataValue.LightType = static_cast<int>(_Type);
    }

    void CreateShadowTarget(std::shared_ptr<class GameEngineRenderTarget> _Target, LightType _Type);

    void DrawEditor() override;

    void ShadowTargetTextureLoad(ShadowTextureScale _Scale = ShadowTextureScale::S_256);

    void ShadowTargetTextureRelease();

    void LightViewSetting(size_t _Index);
};