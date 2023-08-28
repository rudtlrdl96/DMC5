#include "RenderBaseValue.fx"

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

OutPut TAA_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    
    return OutPutValue;
}
    
float3 RGB2YCbCr(float3 rgb)
{
    float3 RGB2Y = { 0.29900, 0.58700, 0.11400 };
    float3 RGB2Cb = { -0.16874, -0.33126, 0.50000 };
    float3 RGB2Cr = { 0.50000, -0.41869, -0.081 };

    return float3(dot(rgb, RGB2Y), dot(rgb, RGB2Cb), dot(rgb, RGB2Cr));
}

float3 YCbCr2RGB(float3 ycc)
{
    float3 YCbCr2R = { 1.0, 0.00000, 1.40200 };
    float3 YCbCr2G = { 1.0, -0.34414, -0.71414 };
    float3 YCbCr2B = { 1.0, 1.77200, 1.40200 };

    return float3(dot(ycc, YCbCr2R), dot(ycc, YCbCr2G), dot(ycc, YCbCr2B));
}

Texture2D DiffuseTex : register(t0);
SamplerState WRAPSAMPLER : register(s0);

float4 TAA_PS(OutPut _Value) : SV_Target0
{        
    float4 ResultColor = (float4)0;
    
    float2 neighbor_offset[8] =
    {
        { -1, -1 },
        { -1, 1 },
        { 1, -1 },
        { 1, 1 },
        { 1, 0 },
        { 0, -1 },
        { 0, 1 },
        { -1, 0},
    };
    
    float4 center_color = DiffuseTex.Sample(WRAPSAMPLER, _Value.UV.xy);

    float4 neighbor_sum = center_color;

    for (int i = 0; i < 8; i++)
    {
        float4 neighbor = DiffuseTex.Sample(WRAPSAMPLER, _Value.UV.xy + neighbor_offset[i] * ScreenScale.xy * 0.001f);
        
        float3 color_diff = abs(neighbor.xyz - center_color.xyz);
        float3 ycc = RGB2YCbCr(color_diff.xyz);
        const float cbcr_threshhold = 0.32f;
        float cbcr_len = length(color_diff.yz);
        
        if (cbcr_threshhold < cbcr_len)
        {             
            ycc = (cbcr_threshhold / cbcr_len) * ycc;
            neighbor.rgb = center_color.rgb + YCbCr2RGB(ycc);
        }
        neighbor_sum += neighbor;
    }
    
    ResultColor = neighbor_sum / 5.0f;
    
    return ResultColor;
}

