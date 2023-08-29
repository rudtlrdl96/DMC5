struct Input
{
    float4 Pos : POSITION;
    float4 UV : TEXCOORD;
};

struct OutPut
{
    float4 Pos : SV_Position;
    float4 UV : TEXCOORD;
};

OutPut ZoomEffect_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    
    return OutPutValue;
}
    
cbuffer ZoomEffectBuffer : register(b0)
{
    float4 ZoomRatio;
}

Texture2D DiffuseTex : register(t0);
SamplerState WRAPSAMPLER : register(s0);

float map(float value, float originalMin, float originalMax, float newMin, float newMax)
{
    return (value - originalMin) / (originalMax - originalMin) * (newMax - newMin) + newMin;
}

float4 ZoomEffect_PS(OutPut _Value) : SV_Target0
{   
    // -1 ~ 1
    float2 ResultUV = (_Value.UV.xy * 2.0f) - float2(1.0f, 1.0f);
   
    ResultUV /= ZoomRatio.xy;
    
    ResultUV.x = map(ResultUV.x, -1.0f, 1.0f, 0.0f, 1.0f);
    ResultUV.y = map(ResultUV.y, -1.0f, 1.0f, 0.0f, 1.0f);
       
        
    float4 ResultColor = DiffuseTex.Sample(WRAPSAMPLER, ResultUV);

    return ResultColor;
}
