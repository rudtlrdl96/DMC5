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
static float Gau[9][9] =
{
    { 0.0000f, 0.0000f, 0.0000f, 0.0000f, 0.0001f, 0.0000f, 0.0000f, 0.0000f, 0.0000f},
    { 0.0000f, 0.0000f, 0.0002f, 0.0011f, 0.0018f, 0.0011f, 0.0002f, 0.0000f, 0.0000f},
    { 0.0000f, 0.0002f, 0.0029f, 0.0131f, 0.0215f, 0.0131f, 0.0029f, 0.0002f, 0.0000f},
    { 0.0000f, 0.0011f, 0.0131f, 0.0585f, 0.0965f, 0.0585f, 0.0131f, 0.0011f, 0.0000f},
    { 0.0001f, 0.0018f, 0.0215f, 0.0965f, 0.1592f, 0.0965f, 0.0215f, 0.0018f, 0.0001f},
    { 0.0000f, 0.0011f, 0.0131f, 0.0585f, 0.0965f, 0.0585f, 0.0131f, 0.0011f, 0.0000f},
    { 0.0000f, 0.0002f, 0.0029f, 0.0131f, 0.0215f, 0.0131f, 0.0029f, 0.0002f, 0.0000f},
    { 0.0000f, 0.0000f, 0.0002f, 0.0011f, 0.0018f, 0.0011f, 0.0002f, 0.0000f, 0.0000f},
    { 0.0000f, 0.0000f, 0.0000f, 0.0000f, 0.0001f, 0.0000f, 0.0000f, 0.0000f, 0.0000f},
};

Texture2D SmallBloomTex : register(t0);
SamplerState POINTSAMPLER : register(s0);

cbuffer BlurData : register(b2)
{
    float4 ScreenSize;
    float4 ScreenRatio;
}

float4 Blur_PS(OutPut _Value) : SV_Target1
{
    float2 PixelSize = float2(1.0f / (ScreenSize.x * ScreenRatio.x), 1.0f / (ScreenSize.y * ScreenRatio.x)) * 2.0f;
    // 현재 UV
    float2 PixelUvCenter = _Value.UV.xy;
    float2 StartUV = _Value.UV.xy - (PixelSize * 4.0f);
    float2 CurUV = StartUV;
    
    float4 ResultColor = (float4) 0.0f;
 
    // 쉐이더 for문 쓰는건 정말 주의해야 합니다.
    // 상수가 아닐시 에러가 뜰때가 있다.
    for (int y = 0; y < 9; ++y)
    {
        for (int x = 0; x < 9; ++x)
        {
            ResultColor += SmallBloomTex.Sample(POINTSAMPLER, CurUV.xy) * Gau[y][x];
            // ResultColor *= float4(0.0f, 1.0f, 0.0f, 1.0f);
            CurUV.x += PixelSize.x;
        }
        
        CurUV.x = StartUV.x;
        CurUV.y += PixelSize.y;
    }
    
    
    ResultColor.a = 1.0f;
    
    return ResultColor;
}