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

// 

// 대충
static float Gau[5][5] =
{
    { 1, 4, 6, 4, 1 },
    { 4, 16, 24, 16, 4 },
    { 6, 24, 36, 24, 6 },
    { 4, 16, 24, 16, 4 },
    { 1, 4, 6, 4, 1 }
};

Texture2D SmallBloomTex : register(t0);
SamplerState POINTSAMPLER : register(s0);

cbuffer BlurData : register(b2)
{
    float4 ScreenSize;
    float4 ScreenRatio;
}

float4 Blur_PS(OutPut _Value) : SV_Target0
{
    float2 PixelSize;
    
    PixelSize.x = ScreenRatio.x;
    PixelSize.y = ScreenRatio.y;
        
    float2 StartUV = _Value.UV.xy - (PixelSize * 2.0f);
    float2 CurUV = StartUV;
    
    float4 ResultColor = (float4) 0.0f;
 
    for (int y = 0; y < 5; ++y)
    {
        for (int x = 0; x < 5; ++x)
        {
            ResultColor += SmallBloomTex.Sample(POINTSAMPLER, saturate(CurUV.xy)) * Gau[y][x];
            CurUV.x += PixelSize.x;
        }
        
        CurUV.x = StartUV.x;
        CurUV.y += PixelSize.y;
    }
            
    ResultColor.rgb /= 256.0f;
    ResultColor.a = 1.0f;
    
    return ResultColor;
}