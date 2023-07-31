#pragma once
#include "GameEngineEnum.h"
#include "GameEngineShaderResHelper.h"

// ���� :
class GameEngineShader
{
public:
	// constrcuter destructer
	GameEngineShader();
	~GameEngineShader();

	// delete Function
	GameEngineShader(const GameEngineShader& _Other) = delete;
	GameEngineShader(GameEngineShader&& _Other) noexcept = delete;
	GameEngineShader& operator=(const GameEngineShader& _Other) = delete;
	GameEngineShader& operator=(GameEngineShader&& _Other) noexcept = delete;

	void CreateVersion(const std::string_view& _ShaderType, UINT _VersionHigt = 5, UINT _VersionLow = 0);
	inline void SetEntryPoint(const std::string_view& _EntryPoint) 
	{
		EntryPoint = _EntryPoint;
	}

	inline const GameEngineShaderResHelper& GetShaderResHelper() const
	{
		return ResHelper;
	}

	inline ShaderType GetType() 
	{
		return Type;
	}

	static void AutoCompile(GameEngineFile& _File);

protected:
	// Hlsl ������� ���� �ƿ� �ϰ���
	// ���̴� �ڵ带 2�������ͷ� ���������� �װ� �׷���ī�尡 ���ο� �����
	// ���̴��� ���̳ʸ� �ڵ�
	ShaderType Type = ShaderType::None;
	ID3DBlob* BinaryCode = nullptr;
	std::string Version = "";
	std::string EntryPoint = "";

	// ���̴����� ����� ���ҽ��� �����ϴ� �Լ�
	void ShaderResCheck();

private:
	GameEngineShaderResHelper ResHelper;
};

