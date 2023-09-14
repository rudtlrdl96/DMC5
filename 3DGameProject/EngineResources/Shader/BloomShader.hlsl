struct Input
{
    float4 Pos : POSITION;
    float4 UV : TEXCOORD;
};

struct OutPut
{
    // 레스터라이저야 이 포지션이
    // w나눈 다음  뷰포트 곱하고 픽셀 건져낼때 쓸포지션 정보를 내가 보낸거야.
    float4 Pos : SV_Position;
    float4 UV : TEXCOORD;
};

OutPut Bloom_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    return OutPutValue;
}

Texture2D LightTarget : register(t0);
SamplerState WRAPSAMPLER : register(s0);

float4 Bloom_PS(OutPut _Value) : SV_Target0
{
    float4 Color = LightTarget.Sample(WRAPSAMPLER, _Value.UV.xy);
    float LightPower = max(max(Color.r, Color.g), Color.b);
    
    if(0.7f >= LightPower)
    {
        return Color;
    }
    
    return Color * (1.0f + (LightPower - 0.7));
}