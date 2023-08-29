#include "Transform.fx"
#include "Animation.fx"
#include "Light.fx"
#include "RenderBaseValue.fx"

struct Input
{
    float4 POSITION : POSITION;
};

struct Output
{
    float4 POSITION : SV_POSITION;
    float4 WPOSITION : POSITION;
};

Output MeshAniTexture_VS(float4 POSITION : POSITION)
{
    Output NewOutPut = (Output)0;
    
    float4 InputPos = POSITION;
    InputPos.w = 1.0f;
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.WPOSITION = mul(InputPos, WorldMatrix);
    return NewOutPut;
}

cbuffer GridData : register(b0)
{
    float4 GridScale;
};

float4 MeshAniTexture_PS(Output _Input) : SV_Target0
{
    float CalX = abs(_Input.WPOSITION.x) % GridScale.x;
    float CalZ = abs(_Input.WPOSITION.z) % GridScale.z;
    
    if (GridScale.y >= CalX)
    {
        return float4(1.0f, 1.0f, 1.0f, GridScale.w);
    }
    else if (GridScale.y >= CalZ)
    {
        return float4(1.0f, 1.0f, 1.0f, GridScale.w);
    }
    
    float CalX1 = abs(_Input.WPOSITION.x) % GridScale.x * 10.0f;
    float CalZ1 = abs(_Input.WPOSITION.x) % GridScale.z * 10.0f;
    
    if (GridScale.y * 10.0f >= CalX)
    {
        return float4(1.0f, 1.0f, 1.0f, 1.0f - GridScale.w);
    }
    else if (GridScale.y * 10.0f >= CalZ)
    {
        return float4(1.0f, 1.0f, 1.0f, 1.0f - GridScale.w);
    }
    
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}

