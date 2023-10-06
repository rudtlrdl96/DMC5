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

OutPut DistortionEffect_VS(Input _Value)
{
	OutPut OutPutValue = (OutPut) 0;
	OutPutValue.Pos = _Value.Pos;
	OutPutValue.UV = _Value.UV;
    
	return OutPutValue;
}
    
cbuffer ColorEffectBuffer : register(b0)
{
	float4 ColorStart;
	float4 ColorEnd;
	float4 ColorTime;
}

Texture2D DiffuseTex : register(t0);
SamplerState WRAPSAMPLER : register(s0);

float4 DistortionEffect_PS(OutPut _Value) : SV_Target0
{
	float4 ResultColor = DiffuseTex.Sample(WRAPSAMPLER, _Value.UV.xy);
    
	float CurA = ResultColor.a;
	ResultColor += lerp(ColorStart, ColorEnd, ColorTime);
	ResultColor.a = CurA;

	return ResultColor;
}
