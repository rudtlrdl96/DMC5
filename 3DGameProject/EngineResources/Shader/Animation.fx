struct AniMat
{
    float4x4 Mat;
};

// ������۷� �ȵǳ�?
// ������۴� ���� �Ҵ��� �ȵȴ�.
// �뷮������ 700����Ʈ ���� �ۿ� �ȵȴ�.
// �ִϸ��̼��� �ϴµ� �ʿ��Ѱ� ��� 200~300
// 64

// 1�����ӿ� �ش��ϴ� ��纻�� ����Դϴ�.
StructuredBuffer<AniMat> ArrAniMationMatrix : register(t11);

// inout c++�� ġ�� ���۷���
// float4&
void Skinning(inout float4 _Pos, inout float4 _Weight, inout int4 _Index, StructuredBuffer<AniMat> _ArrMatrix)
{
    float4 CalPos = (float4) 0.0f;
    // 1�� ���߷��� �ൿ.
    // _Weight[3] = 1.0f - _Weight[0] - _Weight[1] - _Weight[2];
    _Pos.w = 1.0f;
    
    for (int i = 0; i < 4; ++i)
    {
        AniMat Mat = _ArrMatrix[_Index[i]];
        CalPos += _Weight[i] * mul(_Pos, Mat.Mat);
    }
    
    _Pos = CalPos;
    _Pos.w = 1.0f;

}

void SkinningNormal(inout float4 _Normal, inout float4 _Weight, inout int4 _Index, StructuredBuffer<AniMat> _ArrMatrix)
{
    float4 CalPos = (float4) 0.0f;
    // 1�� ���߷��� �ൿ.
    // _Weight[3] = 1.0f - _Weight[0] - _Weight[1] - _Weight[2];
    
    _Normal.w = 1.0f;
    
    for (int i = 0; i < 4; ++i)
    {
        AniMat Mat = _ArrMatrix[_Index[i]];
        CalPos += _Weight[i] * mul(_Normal, Mat.Mat);
    }
    
    _Normal = CalPos;
    _Normal.xyz = normalize(_Normal.xyz);
    _Normal.w = 0.0f;

}
