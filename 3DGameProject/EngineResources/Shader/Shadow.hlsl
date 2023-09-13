
struct Output
{
    // viewport�� �������� ������� ���Ѵ�.
    float4 POSITION : SV_POSITION;
    // �׸��ڸ� �׸��� ���ؼ� �ʿ��Ѱ� 3���� ���������� �������� �Ϸᰡ�Ǽ�
    // 0~1������ ������ ����� �ְ� �ʿ��մϴ�.
    // view������ �������� �ɰǵ�.
    // 5������ �Ѱ���� �Ѵ�.
    float4 WVPPos : POSITION5;
};


float4 Shadow_PS(Output _Value) : SV_Target0
{
    // ���̰��� �ϼ���
    return float4(_Value.WVPPos.z / _Value.WVPPos.w, 0.0f, 0.0f, 1.0f);
}