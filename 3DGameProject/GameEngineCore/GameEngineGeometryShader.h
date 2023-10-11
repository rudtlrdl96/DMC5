#pragma once
#include "GameEngineResource.h"
#include "GameEngineShader.h"

// Ό³Έν :
class GameEngineGeometryShader : public GameEngineResource<GameEngineGeometryShader>, public GameEngineShader
{
	friend GameEngineShader;
public:
	// constrcuter destructer
	GameEngineGeometryShader();
	~GameEngineGeometryShader();

	// delete Function
	GameEngineGeometryShader(const GameEngineGeometryShader& _Other) = delete;
	GameEngineGeometryShader(GameEngineGeometryShader&& _Other) noexcept = delete;
	GameEngineGeometryShader& operator=(const GameEngineGeometryShader& _Other) = delete;
	GameEngineGeometryShader& operator=(GameEngineGeometryShader&& _Other) noexcept = delete;

	static std::shared_ptr<GameEngineGeometryShader> Load(const std::string_view& _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0)
	{
		GameEnginePath Path = GameEnginePath(std::string(_Path.data()));

		return Load(_Path, Path.GetFileName(), _EntryPoint, _VersionHigh, _VersionLow);
	}

	static std::shared_ptr<GameEngineGeometryShader> Load(const std::string_view& _Path, const std::string_view& _Name, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0)
	{
		std::shared_ptr<GameEngineGeometryShader> Res = GameEngineGeometryShader::Create(_Name);
		Res->ShaderLoad(_Path, _EntryPoint, _VersionHigh, _VersionLow);
		return Res;
	}

	void Setting() override;

protected:

private:
	ID3D11GeometryShader* ShaderPtr = nullptr;
	void ShaderLoad(const std::string_view& _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);

};

