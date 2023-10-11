#pragma once
#include "GameEngineResource.h"
#include "GameEngineShader.h"

// Ό³Έν :
class GameEngineComputeShader : public GameEngineResource<GameEngineComputeShader>, public GameEngineShader
{
public:
	// constrcuter destructer
	GameEngineComputeShader();
	~GameEngineComputeShader();

	// delete Function
	GameEngineComputeShader(const GameEngineComputeShader& _Other) = delete;
	GameEngineComputeShader(GameEngineComputeShader&& _Other) noexcept = delete;
	GameEngineComputeShader& operator=(const GameEngineComputeShader& _Other) = delete;
	GameEngineComputeShader& operator=(GameEngineComputeShader&& _Other) noexcept = delete;

	static std::shared_ptr<GameEngineComputeShader> Load(const std::string_view& _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0)
	{
		GameEnginePath Path = GameEnginePath(std::string(_Path.data()));

		return Load(_Path, Path.GetFileName(), _EntryPoint, _VersionHigh, _VersionLow);
	}

	static std::shared_ptr<GameEngineComputeShader> Load(const std::string_view& _Path, const std::string_view& _Name, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0)
	{
		std::shared_ptr<GameEngineComputeShader> Res = GameEngineComputeShader::Create(_Name);
		Res->ShaderLoad(_Path, _EntryPoint, _VersionHigh, _VersionLow);
		return Res;
	}

	// void Execute();
	void Setting() override;

protected:
	// virtual void UpdateData() = 0;
	// virtual void Clear() = 0;


private:
	ID3D11ComputeShader* ShaderPtr = nullptr;

	void ShaderLoad(const std::string_view& _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);
};

