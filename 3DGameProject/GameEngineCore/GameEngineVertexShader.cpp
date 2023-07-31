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

	// 상수버퍼 할때 이옵션이 관련이 있습니다.
	// 상수버퍼에 float4x4 크기의 데이터를 넣으면 
	// 전치된 느낌으로 들어간다.
	// 행렬을 넣을때.
	
	// 내가 1 2 3 4

	// 쉐이더 1 
	//        2
	//         3
	//         4

// | 비트연산 중에서 둘중 하나만 비트가 1이어도 1되게 만들어라.

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
	// 옵션이 켜졌는지 확인하는법
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

		// 에러를 텍스트로 알려줍니다.
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
		MsgAssert("버텍스 쉐이더 핸들 생성에 실패했습니다");
	}

	ShaderResCheck();
}

void GameEngineVertexShader::Setting()
{
	if (nullptr == ShaderPtr)
	{
		MsgAssert("버텍스 쉐이더가 존재하지 않습니다 세팅에 실패했습니다.");
	}

	GameEngineDevice::GetContext()->VSSetShader(ShaderPtr, nullptr, 0);
}