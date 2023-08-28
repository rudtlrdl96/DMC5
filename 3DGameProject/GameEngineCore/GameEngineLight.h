#pragma once
#include "GameEngineActor.h"

// 빛을 받는 녀석과 빛을 받지 않는 녀석이 있을 것이기 때문에.

// 이 데이터 1개가 빛 1개입니다.

struct LightData
{
    friend class GameEngineLight;
    float4x4 LightViewMatrix;
    float4x4 LightViewInverseMatrix;
    float4x4 LightProjectionMatrix;
    float4x4 LightProjectionInverseMatrix;
    float4x4 LightViewProjectionMatrix;
    float4x4 CameraViewInverseMatrix;
    float4 LightColor = float4(1.0f, 1.0f, 1.0f, 0.0f);
    float4 AmbientLight = float4(0.2f, 0.2f, 0.2f, 1.0f);
    float4 LightPos;
    float4 LightDir;
    float4 LightRevDir;
    float4 ViewLightPos;
    float4 ViewLightDir;
    float4 ViewLightRevDir;
    float4 CameraPosition;
    float LightTargetSizeX;
    float LightTargetSizeY;
    float LightNear;
    float LightFar;
    float DifLightPower = 1.0f;
    float SpcLightPower = 1.0f;
    float AmbLightPower = 1.0f;
    float SpcPow = 50.0f;
    int LightType = 0;
    float PointLightRange = 100.0f;
    int Temp2;
    int Temp3;
};

struct LightDatas
{
    int LightCount = 0;
    LightData AllLight[64];
};

// 설명 :
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

protected:

private:
    LightData LightDataValue;

};

