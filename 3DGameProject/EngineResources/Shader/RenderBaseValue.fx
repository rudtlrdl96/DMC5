cbuffer RenderBaseValue : register(b11)
{
    float4 BaseColor;
    float DeltaTime;
    float SumDeltaTime;
    int IsAnimation;
    int IsNormal;
    float4 ScreenScale;
    float4 Mouse;
};