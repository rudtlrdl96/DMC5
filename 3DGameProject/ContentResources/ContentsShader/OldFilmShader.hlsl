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

OutPut OldFilm_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    
    // 0         1
    // 0.25   0.75
    OutPutValue.UV.x = OutPutValue.UV.x / 2.0f + 0.25f;
    OutPutValue.UV.y = OutPutValue.UV.y / 2.0f + 0.25f;
    
    // 화면 전체 범위
    return OutPutValue;
}

cbuffer OldFilmData : register(b1)
{
    // 상수버퍼는 
    float4 OldFilmValue;
}

Texture2D DiffuseTex : register(t0);
SamplerState WRAPSAMPLER : register(s0);

float4 OldFilm_PS(OutPut _Value) : SV_Target0
{
    float4 Color = DiffuseTex.Sample(WRAPSAMPLER, _Value.UV.xy);
    
    // 0.2 => 0.8
    // 1 => 0
    
    Color.x = (1.0f - Color.x) * OldFilmValue.x;
    //if (Color.x != 0)
    //{
    //    Color.xyzw = 1.0f;
    //}
    Color.yzw = Color.x;
    
    return Color;
}