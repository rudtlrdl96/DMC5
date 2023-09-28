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
    float4 ScreenScale;
    float4 Mouse;
};

#define PIE 3.141592653589793238462643383279502884197169399375105820974944f
#define PIE2 (PIE * 2.0f)
#define DegToRad (PIE / 180)
#define RadToDeg (180 / PIE)