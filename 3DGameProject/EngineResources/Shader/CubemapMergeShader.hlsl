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

OutPut CubemapMerge_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = normalize(_Value.UV);
    
    return OutPutValue;
}

TextureCube CubemapTex : register(t0);
SamplerState WRAPSAMPLER : register(s0);

float4 CubemapMerge_PS(OutPut _Value) : SV_Target0
{
    return CubemapTex.Sample(WRAPSAMPLER, _Value.UV.xyz);
}