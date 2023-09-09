#pragma once
#include "GameEngineConstantBuffer.h"
#include "GameEngineTexture.h"
#include "GameEngineSampler.h"

// 모든 텍스처건 상수버퍼건 
class GameEngineShaderResources
{
public:
	std::string Name;
	class GameEngineShader* ParentShader;
	int BindPoint = -1; // b0 t0 같은 몇번째 슬롯에 세팅되어야 하는지에 대한 정보.

public:
	virtual void Setting() = 0;
	virtual void Reset() {};
};

class GameEngineConstantBufferSetter : public GameEngineShaderResources
{
public:
	std::shared_ptr<GameEngineConstantBuffer> Res;
	const void* CPUData;
	UINT CPUDataSize;

	void Setting() override;
};

class GameEngineTextureSetter : public GameEngineShaderResources
{
public:
	std::shared_ptr<GameEngineTexture> Res;
	void Setting() override;
	void Reset() override;
};

class GameEngineSamplerSetter : public GameEngineShaderResources
{
public:
	std::shared_ptr<GameEngineSampler> Res;
	void Setting() override;
};

class GameEngineStructuredBufferSetter : public GameEngineShaderResources
{
public:
	// 상수버퍼와 완전히 동일하게 동일하게 생각하면 됩니다.
	std::shared_ptr <class GameEngineStructuredBuffer> Res;
	const void* SetData;
	size_t Size;
	size_t Count;
	std::vector<char> OriginalData;

	void Setting() override;

	// void Resize(size_t _Count);

	int GetDataSize();

	// void PushData(const void* Data, int _Count);

	template<typename DataType>
	void Push(DataType& _Data, int _Count)
	{
		int LeftSize = sizeof(DataType);
		int RightSize = GetDataSize();

		if (LeftSize != RightSize)
		{
			MsgAssert("구조화버퍼에 넣으려는 데이터 사이즈가 다릅니다.");
		}

		PushData(&_Data, _Count);
	}

};

class GameEngineShaderResHelper
{
private:
	std::multimap<std::string, GameEngineConstantBufferSetter> ConstantBufferSetters;
	std::multimap<std::string, GameEngineTextureSetter> TextureSetters;
	std::multimap<std::string, GameEngineSamplerSetter> SamplerSetters;
	std::multimap<std::string, GameEngineStructuredBufferSetter> StructuredBufferSetters;

public:
	GameEngineStructuredBufferSetter* GetStructuredBufferSetter(const std::string_view& _View);

	GameEngineTextureSetter* GetTextureSetter(const std::string_view& _View);

	std::vector<GameEngineTextureSetter*> GetTextureSetters(const std::string_view& _View);

	void CreateTextureSetter(const GameEngineTextureSetter& _Setter)
	{
		TextureSetters.insert(std::make_pair(_Setter.Name, _Setter));
	}

	void CreateSamplerSetter(const GameEngineSamplerSetter& _Setter)
	{
		SamplerSetters.insert(std::make_pair(_Setter.Name, _Setter));
	}

	void CreateStructuredBufferSetter(const GameEngineStructuredBufferSetter& _Setter)
	{
		StructuredBufferSetters.insert(std::make_pair(_Setter.Name, _Setter));
	}

	void CreateConstantBufferSetter(const GameEngineConstantBufferSetter& _Setter)
	{
		ConstantBufferSetters.insert(std::make_pair(_Setter.Name, _Setter));
	}

	bool IsStructuredBuffer(const std::string_view& _Name);

	bool IsConstantBuffer(const std::string_view& _Name);

	bool IsTexture(const std::string& _Name);

	template<typename DataType>
	void SetConstantBufferLink(const std::string_view& _Name, const DataType& _Data)
	{
		SetConstantBufferLink(_Name, reinterpret_cast<const void*>(&_Data), sizeof(DataType));
	}

	void SetConstantBufferLink(const std::string_view& _Name, const void* _Data, UINT _Size);

	void SetTexture(const std::string_view& _SettingName, const std::string_view& _ImageName);

	void SetTexture(const std::string_view& _SettingName, std::shared_ptr<GameEngineTexture> _Texture);

	void Copy(const GameEngineShaderResHelper& _ResHelper);

	void Setting();

	void AllResourcesReset();

	void AllResourcesRelease();

};
