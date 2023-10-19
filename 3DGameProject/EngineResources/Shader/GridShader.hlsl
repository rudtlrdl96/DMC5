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
    Output NewOutPut = (Output) 0;
    
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

struct AlphaOutPut
{
    float4 ResultColor : SV_Target0;
};

AlphaOutPut MeshAniTexture_PS(Output _Input)
{
    AlphaOutPut Result;
    
    float CalX = abs(_Input.WPOSITION.x) % GridScale.x;
    float CalZ = abs(_Input.WPOSITION.z) % GridScale.z;
    
    if (GridScale.y >= CalX)
    {
        Result.ResultColor = float4(1.0f, 1.0f, 1.0f, GridScale.w);        
        return Result;
    }
    else if (GridScale.y >= CalZ)
    {
        Result.ResultColor = float4(1.0f, 1.0f, 1.0f, GridScale.w);
        return Result;
    }
    
    //float CalX1 = abs(_Input.WPOSITION.x) % GridScale.x * 10.0f;
    //float CalZ1 = abs(_Input.WPOSITION.x) % GridScale.z * 10.0f;
    
    //if (GridScale.y * 3.0f >= CalX)
    //{
    //    return float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}
    //else if (GridScale.y * 3.0f >= CalZ)
    //{
    //    return float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}
    
    clip(-1);
    
    Result.ResultColor = float4(0, 0, 0, 0);    
    return Result;
}

