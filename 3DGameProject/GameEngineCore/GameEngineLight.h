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
    S_16384,
};

// ���� :
class GameEngineLight : public GameEngineActor
{
    static const float4 PointViewDatas[6][2];
    static const float4 ShadowTextureScales[9];
    static const std::string ShadowTextureEnums[9];
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

    inline void ShadowOn()
    {
        IsShadowLight = true;
    }       
    
    inline void ShadowOff()
    {
        IsShadowLight = false;
    }   
    
    inline void DynamicShadowOn()
    {
        IsDynamicLight = true;
    }

    const LightData& GetLightData()
    {
        return LightDataValue;
    }
    
    // �׸��ڸ� ����ϴ� ����Ʈ���� ��ȯ�մϴ�.
    inline bool IsShadow() const
    {
        return IsShadowLight;
    }

    // ������ ī�޶� ���� �׸��ڸ� Bake �մϴ�.
    void BakeShadow(std::shared_ptr<GameEngineCamera> _BakeCam, int _BakeIndex = 0);

    // ���� ������ BakeTarget Index�� ��ȯ�մϴ�
    inline int GetBakeTargetIndex() const
    {
        return TargetBake;
    }

    // Bake Ÿ���� Index�� �����մϴ�
    inline void SetBakeTarget(int _Index)
    {
        TargetBake = _Index;
    }

    // Shadow Last Target�� ��ȯ�մϴ�.
    std::shared_ptr<class GameEngineRenderTarget> GetShadowTarget()
    {
        return ShadowTarget;
    }

    // �׸��� Bake Target�� ��ȯ�մϴ�.
    std::shared_ptr<class GameEngineRenderTarget> GetBakeTarget(int _Index)
    {
        return BakeShadowTarget[_Index];
    }

    void SetShadowTextureScale(ShadowTextureScale _Scale);

    inline float4 GetShadowTextureScale() const
    {
        float4(LightDataValue.ShadowTargetSizeX, LightDataValue.ShadowTargetSizeY);
    }

    // �׸��� ���� ������ �����մϴ�. [Directinal Light������ �����մϴ�.]
    inline void SetShadowScale(const float4& _Scale)
    {
        ShadowRange = _Scale;
    }
    
    // ���� �׸��� ���� ������ ��ȯ�մϴ�. [Directinal Light������ �����մϴ�.]
    float4 GetShadowScale() const
    {
        return ShadowRange;
    }

    // �� �Ÿ� ����
    void SetLightRange(float _Range);

    // ���� �� �Ÿ��� ��ȯ�մϴ�.
    inline float GetLightRange() const
    {
        return LightDataValue.LightRange;
    }

    // Lignt Angle�� �����մϴ� [SpotLight ������ �����մϴ�]
    inline void SetLightAngle(float _Angle)
    {
        LightDataValue.LightAngle = _Angle;
    }

    // ���� Light Angle�� ��ȯ�մϴ�. [SpotLight ������ �����մϴ�]
    inline float GetLightAngle() const
    {
        return LightDataValue.LightAngle;
    }

    // ���� Light�� Color�� ��ȯ�մϴ�.
    inline void SetLightColor(const float4& _Color)
    {
        LightDataValue.LightColor = _Color;
    }

    // ����Ʈ ��ü Power ����
    inline void SetLightPower(float _Power)
    {
        LightDataValue.LightPower = _Power;
    }

    // ���� Light�� Power�� ��ȯ�մϴ�
    inline float GetLightPower() const
    {
        return LightDataValue.LightPower;
    }

    // Diffuse Light�� Power�� �����մϴ�
    inline void SetDifLightPower(float _DifPower)
    {
        LightDataValue.DifLightPower = _DifPower;
    }

    // ���� Diffuse Light�� Power�� ��ȯ�մϴ�
    inline float GetDifLightPower() const
    {
        return LightDataValue.DifLightPower;
    }

    // Spacular Light�� Power�� �����մϴ�
    inline void SetSpcLightPower(float _SpcPower)
    {
        LightDataValue.SpcLightPower = _SpcPower;
    }

    // ���� Spacular Light�� Power�� ��ȯ�մϴ�
    inline float GetSpcLightPower() const
    {
        return LightDataValue.SpcLightPower;
    }

    // Ambient Light�� Power�� �����մϴ�
    inline void SetAmbLightPower(float _AmbPower)
    {
        LightDataValue.AmbLightPower = _AmbPower;
    }

    // ���� Ambient Light�� Power�� ��ȯ�մϴ�
    inline float GetAmbLightPower() const
    {
        return LightDataValue.AmbLightPower;
    }

    inline float GetCameraDistance() const
    {
        return CameraDistance;
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

    float CameraDistance = 0.0f;

    inline void SetLightType(LightType _Type)
    {
        LightDataValue.LightType = static_cast<int>(_Type);
    }

    void CreateShadowTarget(std::shared_ptr<class GameEngineRenderTarget> _Target, LightType _Type);

    void DrawEditor() override;

    void ShadowTargetTextureLoad(ShadowTextureScale _Scale = ShadowTextureScale::S_256);

    void ShadowTargetTextureRelease();

    void LightViewSetting(size_t _Index);

    void CalCameraDistance(GameEngineCamera* _Camera);
};