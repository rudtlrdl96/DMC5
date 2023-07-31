#pragma once
#include "GameEngineEnum.h"
#include "GameEngineShaderResHelper.h"

// 설명 :
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
	// Hlsl 사람들이 점점 아예 니가쳐
	// 쉐이더 코드를 2진데이터로 변형했을때 그걸 그래픽카드가 내부에 숨기고
	// 쉐이더의 바이너리 코드
	ShaderType Type = ShaderType::None;
	ID3DBlob* BinaryCode = nullptr;
	std::string Version = "";
	std::string EntryPoint = "";

	// 쉐이더에서 사용한 리소스를 조사하는 함수
	void ShaderResCheck();

private:
	GameEngineShaderResHelper ResHelper;
};

