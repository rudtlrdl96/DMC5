#pragma once

#include "GameEngineResource.h"
#include "GameEngineDirectBuffer.h"

enum class StructuredBufferType
{
	SRV_ONLY, // => 쉐이더 리소스로만 사용하겠다.
	UAV_INC, // => 컴퓨트 쉐이더에 넣어서 사용할때.
};

// 설명 :
class GameEngineStructuredBuffer : public GameEngineResource<GameEngineStructuredBuffer>, public GameEngineDirectBuffer
{
public:
	static std::shared_ptr < GameEngineStructuredBuffer> Find(const std::string& _Name, int _ByteSize)
	{
		std::string UpperName = GameEngineString::ToUpper(_Name);

		std::map < std::string, std::map<int, std::shared_ptr < GameEngineStructuredBuffer>>>::iterator NameIter = StructuredBufferRes.find(UpperName);

		if (StructuredBufferRes.end() == NameIter)
		{
			return nullptr;
		}

		std::map<int, std::shared_ptr<GameEngineStructuredBuffer>>& SizeMap = NameIter->second;

		std::map<int, std::shared_ptr<GameEngineStructuredBuffer>>::iterator SizeIter = SizeMap.find(_ByteSize);

		if (SizeIter == SizeMap.end())
		{
			return nullptr;
		}

		return SizeIter->second;
	}

	static std::shared_ptr < GameEngineStructuredBuffer> Create(
		const std::string& _Name,
		D3D11_SHADER_BUFFER_DESC _Desc,
		int Count
	)
	{
		std::shared_ptr<GameEngineStructuredBuffer> NewBuffer = CreateResName(_Name, _Desc.Size);

		NewBuffer->CreateResize(_Desc, Count);

		return NewBuffer;
	}


	static std::shared_ptr < GameEngineStructuredBuffer> Create(
		D3D11_SHADER_BUFFER_DESC _Desc,
		int Count
	)
	{
		std::shared_ptr < GameEngineStructuredBuffer> Res = CreateUnNamed();
		Res->CreateResize(_Desc, Count);
		return Res;
	}



	static std::shared_ptr < GameEngineStructuredBuffer> CreateAndFind(
		const std::string& _Name,
		D3D11_SHADER_BUFFER_DESC _Desc,
		int Count
	)
	{
		std::shared_ptr < GameEngineStructuredBuffer> FindBuffer = Find(_Name, _Desc.Size);

		if (nullptr != FindBuffer)
		{
			return FindBuffer;
		}

		std::shared_ptr < GameEngineStructuredBuffer> NewBuffer = CreateResName(_Name, _Desc.Size);

		NewBuffer->CreateResize(_Desc, Count);

		return NewBuffer;
	}


	static void ResourcesDestroy()
	{
		StructuredBufferRes.clear();
		//for (auto& NameRes : StructuredBufferRes)
		//{
		//	for (auto& SizeRes : NameRes.second)
		//	{
		//		delete SizeRes.second;
		//	}
		//}
	}

	// constrcuter destructer
	GameEngineStructuredBuffer();
	~GameEngineStructuredBuffer();

	// delete Function
	GameEngineStructuredBuffer(const GameEngineStructuredBuffer& _Other) = delete;
	GameEngineStructuredBuffer(GameEngineStructuredBuffer&& _Other) noexcept = delete;
	GameEngineStructuredBuffer& operator=(const GameEngineStructuredBuffer& _Other) = delete;
	GameEngineStructuredBuffer& operator=(GameEngineStructuredBuffer&& _Other) noexcept = delete;

	void VSReset(int _BindPoint);
	void PSReset(int _BindPoint);

	void ChangeData(const void* _Data, size_t _Size);

	void VSSetting(int _BindPoint);

	void PSSetting(int _BindPoint);

	void CreateResize(const D3D11_SHADER_BUFFER_DESC& _Desc, int Count, StructuredBufferType _Type = StructuredBufferType::SRV_ONLY, void* _StartData = nullptr, bool _CPUAccess = false);

	void CreateResize(size_t DataSize, size_t Count, StructuredBufferType _Type = StructuredBufferType::SRV_ONLY, void* _StartData = nullptr, bool _CPUAccess = false);

	void CreateResize(size_t Count, StructuredBufferType _Type = StructuredBufferType::SRV_ONLY, void* _StartData = nullptr, bool _CPUAccess = false);

	void CreateResize(const D3D11_BUFFER_DESC& _Data, StructuredBufferType _Type = StructuredBufferType::SRV_ONLY, void* _StartData = nullptr, bool _CPUAccess = false);

	inline int GetDataSize()
	{
		return DataSize;
	}

	void Release();

protected:
	static std::shared_ptr < GameEngineStructuredBuffer> CreateResName(const std::string& _Name, int _ByteSize)
	{
		std::string Name = GameEngineString::ToUpper(_Name);

		std::shared_ptr < GameEngineStructuredBuffer> FindBuffer = Find(_Name, _ByteSize);

		if (nullptr != FindBuffer)
		{
			return FindBuffer;
		}

		std::shared_ptr < GameEngineStructuredBuffer> Res = GameEngineResource<GameEngineStructuredBuffer>::Create(Name);
		StructuredBufferRes[Name][_ByteSize] = Res;

		return Res;
	}


private:
	static std::map<std::string, std::map<int, std::shared_ptr < GameEngineStructuredBuffer>>> StructuredBufferRes;

	D3D11_SHADER_BUFFER_DESC ShaderDesc;
	D3D11_MAPPED_SUBRESOURCE SettingResources;
	ID3D11ShaderResourceView* ShaderResourceView = nullptr;
	// 컴퓨트 쉐이더에 넣어줄때 사용하는 view
	ID3D11UnorderedAccessView* UnorderedAccessView = nullptr;

	ID3D11Buffer* WriteBuffer = nullptr;
	ID3D11Buffer* ReadBuffer = nullptr;

	int DataSize = 0;
	int DataCount = 0;
	bool IsInit = false;
};

