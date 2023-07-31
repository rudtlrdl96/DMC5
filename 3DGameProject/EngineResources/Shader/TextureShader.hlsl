// ���̴��� ¥�ԵǸ� ������ ��Ģ�� ���Ѿ� �Ѵ�.

// 0~ 16�� ���� 
// ������ ���ٰ� ���°� �ƴϿ���.
cbuffer TransformData : register(b0)
{
    float4 Scale;
    float4 Rotation;
    float4 Quaternion;
    float4 Position;

    float4 LocalScale;
    float4 LocalRotation;
    float4 LocalQuaternion;
    float4 LocalPosition;

    float4 WorldScale;
    float4 WorldRotation;
    float4 WorldQuaternion;
    float4 WorldPosition;

    float4x4 ScaleMatrix;
    float4x4 RotationMatrix;
    float4x4 PositionMatrix;
    float4x4 LocalWorldMatrix;
    float4x4 WorldMatrix;
    float4x4 View;
    float4x4 Projection;
    float4x4 ViewPort;
    float4x4 WorldViewProjectionMatrix;
}

// � ������ ������ ����ü�� ������ �մϴ�.
// ��� �������̰� ��� ���̰�
// �̸� �������
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
    float4 UV : TEXCOORD0;
    float4 ClipUV : TEXCOORD1;
};


cbuffer AtlasData : register(b1)
{
    // 0.0 0.5
    float2 FramePos;
    // 0.5 0.5 
    float2 FrameScale;
    // float4 AtlasUV;
}

cbuffer ClipData : register(b2)
{
    float4 Clip;
    // float4 AtlasUV;
}

cbuffer FlipData : register(b3)
{
    float4 Flip;
    // float4 AtlasUV;
}

// �������������

OutPut Texture_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
	
    _Value.Pos.w = 1.0f;
    OutPutValue.Pos = mul(_Value.Pos, WorldViewProjectionMatrix);
    // OutPutValue.UV = _Value.UV;
    
    // [][]
    // [][]
    
    // 0.5 0.0  0.5 0.5 
    
    // 0,0    1,0
    //
    //
    // 0,1    1,1
    
    float4 VtxUV = _Value.UV;
    
    // -1 0
    if (Flip.x != 0)
    {
        VtxUV.x = 1.0f - VtxUV.x;
    }
    
    if (Flip.y != 0)
    {
        VtxUV.y = 1.0f - VtxUV.y;
    }
    
    OutPutValue.UV.x = (VtxUV.x * FrameScale.x) + FramePos.x;
    OutPutValue.UV.y = (VtxUV.y * FrameScale.y) + FramePos.y;
    
    OutPutValue.ClipUV = _Value.UV;
    
    return OutPutValue;
}

cbuffer ColorOption : register(b0)
{
    float4 MulColor;
    float4 PlusColor;
}

Texture2D DiffuseTex : register(t0);
SamplerState SAMPLER : register(s0);

struct OutColor
{
    float4 Color0 : SV_Target0;
    float4 Color1 : SV_Target1;
    float4 Color2 : SV_Target2;
    float4 Color3 : SV_Target3;
};

float4 Texture_PS(OutPut _Value) : SV_Target0
{
    float4 Color = DiffuseTex.Sample(SAMPLER, _Value.UV.xy);
    
    if (Clip.z == 0)
    {
        if (_Value.ClipUV.x > Clip.x)
        {
            clip(-1);
        }
    }
    else
    {
        // 0~1
        // 0.7
        if (_Value.ClipUV.x < 1.0f - Clip.x)
        {
            clip(-1);
        }
    }
    
    if (Clip.w == 0)
    {
        if (_Value.ClipUV.y > Clip.y)
        {
            clip(-1);
        }
    }
    else
    {
        if (_Value.ClipUV.y < 1.0f - Clip.y)
        {
            clip(-1);
        }
    }
    
    Color *= MulColor;
    Color += PlusColor;
    
    return Color;
}