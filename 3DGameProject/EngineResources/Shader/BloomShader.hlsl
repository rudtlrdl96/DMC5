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

Texture2D LightTarget : register(t0);
SamplerState WRAPSAMPLER : register(s0);

float4 Bloom_PS(OutPut _Value) : SV_Target0
{
    float4 Color = LightTarget.Sample(WRAPSAMPLER, _Value.UV.xy);
    
    if (Color.x <= 0.7f)
    {
        clip(-1);
    }
    
    Color.xyz *= 2.0f;
    
    return Color;
}