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

OutPut Blur_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    
    return OutPutValue;
}

static float Gau[5][5] =
{
    { 1, 4, 6, 4, 1 },
    { 4, 16, 24, 16, 4 },
    { 6, 24, 36, 24, 6 },
    { 4, 16, 24, 16, 4 },
    { 1, 4, 6, 4, 1 }
};

Texture2D DiffuseTex : register(t0);
SamplerState POINTSAMPLER : register(s0);

float4 Blur_PS(OutPut _Value) : SV_Target0
{
    float2 PixelSize = float2(1.0f / 1280.0f, 1.0f / 720);
    float2 PixelUvCenter = _Value.UV.xy;
    float2 StartUV = _Value.UV.xy + (-PixelSize * 2.0f);
    float2 CurUV = StartUV;
    float4 ResultColor = (float4) 0.0f;
 
    // 쉐이더 for문 쓰는건 정말 주의, 상수가 아닐시 에러가 뜰때가 있다.
    for (int y = 0; y < 5; ++y)
    {
        for (int x = 0; x < 5; ++x)
        {
            ResultColor += DiffuseTex.Sample(POINTSAMPLER, CurUV.xy) * Gau[y][x];
            // ResultColor *= float4(0.0f, 1.0f, 0.0f, 1.0f);
            CurUV.x += PixelSize.x;
        }
        
        CurUV.x = StartUV.x;
        CurUV.y += PixelSize.y;
    }
    
    ResultColor /= 256.0f;
    
    if (ResultColor.a <= 0.0f)
    {
        clip(-1);
    }
    
    return ResultColor;
}