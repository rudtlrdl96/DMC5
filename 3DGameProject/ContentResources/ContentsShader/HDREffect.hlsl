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

OutPut HDREffect_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    
    return OutPutValue;
}

Texture2D DiffuseTex : register(t0);
SamplerState WRAPSAMPLER : register(s0);

float4 HDREffect_PS(OutPut _Value) : SV_Target0
{
    const float Gamma = 2.2f;
    
    float3 ResultColor = DiffuseTex.Sample(WRAPSAMPLER, _Value.UV.xy).rgb;
    
    ResultColor = float3(1.0, 1.0f, 1.0f) - exp(-ResultColor * 1.0f);    
    ResultColor = pow(ResultColor, float3(1.0 / Gamma, 1.0 / Gamma, 1.0f / Gamma));
    
    return float4(ResultColor.rgb, 1.0f);
}
