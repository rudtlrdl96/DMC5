
struct Output
{
    // viewport가 곱해져서 사용하지 못한다.
    float4 POSITION : SV_POSITION;
    // 그림자를 그리기 위해서 필요한건 3차원 공간에서의 투영까지 완료가되서
    // 0~1사이의 값으로 변경된 애가 필요합니다.
    // view까지만 곱해져도 될건데.
    // 5번으로 넘겨줘야 한다.
    float4 WVPPos : POSITION5;
};


float4 Shadow_PS(Output _Value) : SV_Target0
{
    // 깊이값이 완성된
    return float4(_Value.WVPPos.z / _Value.WVPPos.w, 0.0f, 0.0f, 1.0f);
}