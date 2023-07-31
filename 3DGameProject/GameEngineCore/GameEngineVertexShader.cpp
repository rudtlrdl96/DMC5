#include "PrecompileHeader.h"
#include "GameEngineVertexShader.h"

GameEngineVertexShader::GameEngineVertexShader() 
{
	Type = ShaderType::Vertex;
}

GameEngineVertexShader::~GameEngineVertexShader() 
{
	if (nullptr != ShaderPtr)
	{
		ShaderPtr->Release();
		ShaderPtr = nullptr;
	}
}

void GameEngineVertexShader::ShaderLoad(const std::string_view& _Path
	, const std::string_view& _EntryPoint
	, UINT _VersionHigh /*= 5*/
	, UINT _VersionLow /*= 0*/)
{
	CreateVersion("vs", _VersionHigh, _VersionLow);
	SetEntryPoint(_EntryPoint);

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif

	// ������� �Ҷ� �̿ɼ��� ������ �ֽ��ϴ�.
	// ������ۿ� float4x4 ũ���� �����͸� ������ 
	// ��ġ�� �������� ����.
	// ����� ������.
	
	// ���� 1 2 3 4

	// ���̴� 1 
	//        2
	//         3
	//         4

// | ��Ʈ���� �߿��� ���� �ϳ��� ��Ʈ�� 1�̾ 1�ǰ� ������.

// 0000000000000000000000000000000000
// 0000000000000000000000000000000001



//#define D3DCOMPILE_DEBUG                                (1 << 0) // 0000000000000000000000000000000001
//#define D3DCOMPILE_SKIP_VALIDATION                      (1 << 1) // 0000000000000000000000000000000010
//#define D3DCOMPILE_SKIP_OPTIMIZATION                    (1 << 2) // 0000000000000000000000000000000100
//#define D3DCOMPILE_PACK_MATRIX_ROW_MAJOR                (1 << 3) // 0000000000000000000000000000001000
//#define D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR             (1 << 4) // 0000000000000000000000000000010000
//#define D3DCOMPILE_PARTIAL_PRECISION                    (1 << 5)
//#define D3DCOMPILE_FORCE_VS_SOFTWARE_NO_OPT             (1 << 6)
//#define D3DCOMPILE_FORCE_PS_SOFTWARE_NO_OPT             (1 << 7)
//#define D3DCOMPILE_NO_PRESHADER                         (1 << 8)
//#define D3DCOMPILE_AVOID_FLOW_CONTROL                   (1 << 9)
//#define D3DCOMPILE_PREFER_FLOW_CONTROL                  (1 << 10)
//#define D3DCOMPILE_ENABLE_STRICTNESS                    (1 << 11)
//#define D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY       (1 << 12)
//#define D3DCOMPILE_IEEE_STRICTNESS                      (1 << 13)
//#define D3DCOMPILE_OPTIMIZATION_LEVEL0                  (1 << 14)


	Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
	// Flag & D3DCOMPILE_SKIP_VALIDATION
	// �ɼ��� �������� Ȯ���ϴ¹�
	// Flag |= D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;

	ID3DBlob* Error;

	std::wstring UniCodePath = GameEngineString::AnsiToUniCode(_Path);

	if (S_OK != D3DCompileFromFile
	(
		UniCodePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		EntryPoint.c_str(),
		Version.c_str(),
		Flag,
		0,
		&BinaryCode,
		&Error
	)
		)
	{

		// ������ �ؽ�Ʈ�� �˷��ݴϴ�.
		std::string ErrorString = reinterpret_cast<char*>(Error->GetBufferPointer());
		MsgAssert(ErrorString);
		return;
	}

	if (S_OK != GameEngineDevice::GetDevice()->CreateVertexShader
	(
		BinaryCode->GetBufferPointer(),
		BinaryCode->GetBufferSize(),
		nullptr,
		&ShaderPtr
	)
		)
	{
		MsgAssert("���ؽ� ���̴� �ڵ� ������ �����߽��ϴ�");
	}

	ShaderResCheck();
}

void GameEngineVertexShader::Setting()
{
	if (nullptr == ShaderPtr)
	{
		MsgAssert("���ؽ� ���̴��� �������� �ʽ��ϴ� ���ÿ� �����߽��ϴ�.");
	}

	GameEngineDevice::GetContext()->VSSetShader(ShaderPtr, nullptr, 0);
}