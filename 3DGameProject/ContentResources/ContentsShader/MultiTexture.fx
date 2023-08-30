

float4 ColorMix(float4 _MainColor, float4 _MixColor)
{
    float4 ResultColor = _MainColor * _MixColor * 2.2f;    
    ResultColor = saturate(ResultColor);
    
    return ResultColor;
}