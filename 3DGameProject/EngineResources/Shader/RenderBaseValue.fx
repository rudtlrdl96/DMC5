cbuffer RenderBaseValue : register(b11)
{
    float4 BaseColor;
    float4 AddColor;
    float4 MulColor;
    float DeltaTime;
    float SumDeltaTime;
    int IsAnimation;
    int IsLight;
    int IsNormal;
    int IsSpecular;
    int IsBlurMask;
    float4 ScreenScale;
    float4 Mouse;
    float4 NoiseResolution;
};

