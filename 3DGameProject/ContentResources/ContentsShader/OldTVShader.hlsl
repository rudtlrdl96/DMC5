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

OutPut OldTV_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    
    // 화면 전체 범위
    return OutPutValue;
}

//float noise(float2 p)
//{
//    float s = texture(iChannel1, float2(1., 2. * cos(iTime)) * iTime * 8. + p * 1.).x;
//    s *= s;
//    return s;
//}

//float onOff(float a, float b, float c)
//{
//    return step(c, sin(iTime + a * cos(iTime * b)));
//}

//float ramp(float y, float start, float end)
//{
//    float inside = step(start, y) - step(end, y);
//    float fact = (y - start) / (end - start) * inside;
//    return (1. - fact) * inside;
	
//}

//float stripes(float2 uv)
//{
	
//    float noi = noise(uv * float2(0.5, 1.) + float2(1., 3.));
//    return ramp(mod(uv.y * 4. + iTime / 2. + sin(iTime + sin(iTime * 0.63)), 1.), 0.5, 0.6) * noi;
//}

//vec3 getVideo(float2 uv)
//{
//    float2 look = uv;
//    float window = 1. / (1. + 20. * (look.y - mod(iTime / 4., 1.)) * (look.y - mod(iTime / 4., 1.)));
//    look.x = look.x + sin(look.y * 10. + iTime) / 50. * onOff(4., 4., .3) * (1. + cos(iTime * 80.)) * window;
//    float vShift = 0.4 * onOff(2., 3., .9) * (sin(iTime) * sin(iTime * 20.) +
//										 (0.5 + 0.1 * sin(iTime * 200.) * cos(iTime)));
//    look.y = mod(look.y + vShift, 1.);
//    vec3 video = vec3(texture(iChannel0, look));
//    return video;
//}

//float2 screenDistort(float2 uv)
//{
//    uv -= float2(.5, .5);
//    uv = uv * 1.2 * (1. / 1.2 + 2. * uv.x * uv.x * uv.y * uv.y);
//    uv += float2(.5, .5);
//    return uv;
//}

//void mainImage(out float4 fragColor, in float2 fragCoord)
//{
//    float2 uv = fragCoord.xy / iResolution.xy;
//    uv = screenDistort(uv);
//    vec3 video = getVideo(uv);
//    float vigAmt = 3. + .3 * sin(iTime + 5. * cos(iTime * 5.));
//    float vignette = (1. - vigAmt * (uv.y - .5) * (uv.y - .5)) * (1. - vigAmt * (uv.x - .5) * (uv.x - .5));
	
//    video += stripes(uv);
//    video += noise(uv * 2.) / 2.;
//    video *= vignette;
//    video *= (12. + mod(uv.y * 30. + iTime, 1.)) / 13.;
	
//    fragColor = float4(video, 1.0);
//}

cbuffer RenderBaseValue : register(b11)
{
    float DeltaTime = 0.0f;
    float SumDeltaTime = 0.0f;
    int IsAnimation = 0;
    int IsLight = 1;
    int IsNormal = 0;
    float4 ScreenScale;
    float4 Mouse;
}


Texture2D DiffuseTex : register(t0);
SamplerState WRAP : register(s0);

float2 getDistortion(float2 uv, float d, float t)
{
    uv.x += cos(d) + t * 0.9;
    uv.y += sin(d + t * 0.75);
    return uv;
}

float4 getDistortedTexture(SamplerState iChannel, float2 uv)
{
    float4 rgb = DiffuseTex.Sample(iChannel, uv);
    return rgb;
}

void mainImage(out float4 fragColor, in float2 fragCoord)
{
    // float2 uv = fragCoord.xy / ScreenSize.xy;
    float2 uv = fragCoord.xy;
    float t = SumDeltaTime.x;
    float2 mid = float2(0.5, 0.5);
    // float2 focus = Mouse.xy / ScreenSize.xy;
    float2 focus = Mouse.xy;
    float d1 = distance(focus + sin(t * 0.25) * 0.5, uv);
    float d2 = distance(focus + cos(t), uv);
    float4 rgb = (getDistortedTexture(WRAP, getDistortion(uv, d1, t)) + getDistortedTexture(WRAP, getDistortion(uv, -d2, t))) * 0.5f;
    rgb.r /= d2;
    rgb.g += -0.5 + d1;
    rgb.b = -0.5 + (d1 + d2) / 2.0;
    fragColor = rgb;
}

float4 OldTV_PS(OutPut _Value) : SV_Target0
{
    // float2 ScreenSize1 = {1280.0f, 720.0f};
    
    float2 uv = _Value.UV.xy;
    //// uv.y = 1.0 - uv.y;
    uv.x += sin(uv.y * 10.0f + SumDeltaTime.x) / 10.0;
    
    //// uv.x += Time.x * 0.1f;
    float4 Color = DiffuseTex.Sample(WRAP, uv.xy);
    
    //float4 Color;
    
    //mainImage(Color, _Value.UV.xy);
    

    return Color;
}
