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

OutPut BWColorEffect_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    
    return OutPutValue;
}

Texture2D DiffuseTex : register(t0);
SamplerState WRAPSAMPLER : register(s0);

float4 BWColorEffect_PS(OutPut _Value) : SV_Target0
{
    float4 ResultColor = DiffuseTex.Sample(WRAPSAMPLER, _Value.UV.xy);
    
    ResultColor.xyz = (ResultColor.x + ResultColor.y + ResultColor.z) / 3.0f;
    
    return ResultColor;
}
