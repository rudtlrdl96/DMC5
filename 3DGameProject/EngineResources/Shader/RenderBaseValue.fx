cbuffer RenderBaseValue : register(b11)
{
    float4 BaseColor = { 0.0f, 0.0f, 0.0f, 1.0f };
    float DeltaTime = 0.0f;
    float SumDeltaTime = 0.0f;
    int IsAnimation = 0;
    int IsLight = 1;
    int IsNormal = 0;
    int IsSpecular = 0;
    float4 ScreenScale;
    float4 Mouse;
};