#include "PrecompileHeader.h"
#include "GameEngineComputeShader.h"

GameEngineComputeShader::GameEngineComputeShader()
{
	Type = ShaderType::Compute;
}

GameEngineComputeShader::~GameEngineComputeShader()
{
	if (nullptr != ShaderPtr)
	{
		ShaderPtr->Release();
		ShaderPtr = nullptr;
	}
}

void GameEngineComputeShader::ShaderLoad(const std::string_view& _Path, const std::string_view& _EntryPoint, UINT _VersionHigh /*= 5*/, UINT _VersionLow /*= 0*/)
{
	CreateVersion("cs", _VersionHigh, _VersionLow);
	SetEntryPoint(_EntryPoint);

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif

	// ������� �Ҷ� �̿ɼ��� ������ �ֽ��ϴ�.
	Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

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

	if (S_OK != GameEngineDevice::GetDevice()->CreateComputeShader
	(
		BinaryCode->GetBufferPointer(),
		BinaryCode->GetBufferSize(),
		nullptr,
		&ShaderPtr
	)
		)
	{
		MsgAssert("������Ʈ�� ���̴� �ڵ� ������ �����߽��ϴ�");
	}

	ShaderResCheck();
}

void GameEngineComputeShader::Setting()
{
	if (nullptr == ShaderPtr)
	{
		MsgAssert("��ǻƮ ���̴��� �������� �ʽ��ϴ� ���ÿ� �����߽��ϴ�.");
	}

	GameEngineDevice::GetContext()->CSSetShader(ShaderPtr, nullptr, 0);
}
//
//void GameEngineComputeShader::Execute()
//{
//	// ���̴� ���ҽ� �並 �����ϰ�
//	// UpdateData();
//
//	// ��� ������Ʈ	
//	//CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
//	//pCB->SetData(&m_Param);
//	//pCB->UpdateData_CS();
//	//void CConstBuffer::UpdateData_CS()
//	//{
//	//	CONTEXT->CSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());
//	//}
//
//
//
//	Setting();
//
//	// ���� �����ض�.
//	GameEngineDevice::GetContext()->Dispatch(m_iGroupX, m_iGroupY, m_iGroupZ);
//
//	// ������۳� �̷��� �� ����.
//	// Clear();
//
//}