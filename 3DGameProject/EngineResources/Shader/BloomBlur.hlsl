struct Input
{
    float4 Pos : POSITION;
    float4 UV : TEXCOORD;
};

struct OutPut
{
    // �����Ͷ������� �� ��������
    // w���� ����  ����Ʈ ���ϰ� �ȼ� �������� �������� ������ ���� �����ž�.
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

// ����
static float Gau[7][7] =
{
    { 0.000f, 0.000f, 0.001f, 0.001f, 0.001f, 0.000f, 0.000f },
    { 0.000f, 0.002f, 0.012f, 0.002f, 0.012f, 0.002f, 0.000f },
    { 0.001f, 0.012f, 0.068f, 0.109f, 0.068f, 0.012f, 0.001f },
    { 0.001f, 0.020f, 0.109f, 0.172f, 0.109f, 0.020f, 0.001f },
    { 0.001f, 0.012f, 0.068f, 0.109f, 0.068f, 0.012f, 0.001f },
    { 0.000f, 0.002f, 0.012f, 0.020f, 0.012f, 0.002f, 0.000f },
    { 0.000f, 0.000f, 0.001f, 0.001f, 0.001f, 0.000f, 0.000f },
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
    float2 PixelSize = float2(1.0f / (ScreenSize.x * ScreenRatio.x), 1.0f / (ScreenSize.y * ScreenRatio.x));
    // ���� UV
    float2 PixelUvCenter = _Value.UV.xy;
    float2 StartUV = _Value.UV.xy + (-PixelSize * 3.0f);
    float2 CurUV = StartUV;
    
    float4 ResultColor = (float4) 0.0f;
 
    // ���̴� for�� ���°� ���� �����ؾ� �մϴ�.
    // ����� �ƴҽ� ������ �㶧�� �ִ�.
    for (int y = 0; y < 7; ++y)
    {
        for (int x = 0; x < 7; ++x)
        {
            ResultColor += SmallBloomTex.Sample(POINTSAMPLER, CurUV.xy) * Gau[y][x];
            // ResultColor *= float4(0.0f, 1.0f, 0.0f, 1.0f);
            CurUV.x += PixelSize.x;
        }
        
        CurUV.x = StartUV.x;
        CurUV.y += PixelSize.y;
    }
    
    
    ResultColor.a = ResultColor.x;
    
    return ResultColor;
}