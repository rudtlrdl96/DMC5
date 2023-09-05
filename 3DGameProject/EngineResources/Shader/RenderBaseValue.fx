cbuffer RenderBaseValue : register(b11)
{
    float4 BaseColor;
    float DeltaTime = 0.0f;
    float SumDeltaTime = 0.0f;
    int IsAnimation = 0;
    int IsLight = 0;
    int IsNormal = 0;
    int IsSpecular = 0;
    float4 ScreenScale;
    float4 Mouse;
    float4 ClipColor;
};