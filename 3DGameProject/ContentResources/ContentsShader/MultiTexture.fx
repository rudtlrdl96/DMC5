Texture2D MultiTexture[4] : register(t10);

cbuffer BultiTextureData : register(b10)
{
    int TextureCount;    
};

float4 ColorMix(float4 _MainTextureColor, float2 _UV, SamplerState _Sample)
{
    float4 ResultColor = _MainTextureColor;
    
    for (int i = 0; i < _MainTextureColor; ++i)
    {
        ResultColor *= MultiTexture[i].Sample(_Sample, _UV);
    }
    
    ResultColor *= TextureCount + 1;
    ResultColor = saturate(ResultColor);
    
    return ResultColor;
}