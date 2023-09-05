#pragma once
#include <vector>
#include <string>
#include <string_view>
#include "GameEngineMath.h"

// 데이터를 바이트 단위 char 배열로 만들어주는 녀석
// 직렬화란 클래스를 만들면 c++로 예를 들면 가상함수포인터나 
// 포인터같은 저장해봐야 쓸모없는 데이터들을 다 제외하고 필요한 데이터만
// 바이트 단위로 변경하는것을 의미합니다.


class GameEngineSerializObject
{
public:
	virtual void Write(class GameEngineSerializer& _File) = 0;
	virtual void Read(class GameEngineSerializer& _File) = 0;
};


// 설명 :
class GameEngineSerializer
{
	friend class GameEngineFile;

public:
	// constrcuter destructer
	GameEngineSerializer();
	GameEngineSerializer(const char* _Data, unsigned int _Size);
	~GameEngineSerializer();

	// delete Function
	//GameEngineSerializer(const GameEngineSerializer& _Other) = delete;
	//GameEngineSerializer(GameEngineSerializer&& _Other) noexcept = delete;
	//GameEngineSerializer& operator=(const GameEngineSerializer& _Other) = delete;
	//GameEngineSerializer& operator=(GameEngineSerializer&& _Other) noexcept = delete;

	void BufferResize(unsigned int _Size)
	{
		Data.resize(_Size);
	}

	size_t GetBufferSize() const
	{
		return Data.size();
	}

	unsigned int GetReadOffSet() const
	{
		return ReadOffset;
	}

	void ReadOffsetReset()
	{
		ReadOffset = 0;
	}

	unsigned int GetWriteOffSet() const
	{
		return WriteOffset;
	}

	void WriteOffsetReset()
	{
		WriteOffset = 0;
	}

	void Reset()
	{
		WriteOffset = 0;
		ReadOffset = 0;
	}

	const std::vector<unsigned char>& GetData() {
		return Data;
	}

	unsigned char* GetDataPtr() {
		return &Data[0];
	}

	const char* GetConstCharPtr() {
		return reinterpret_cast<const char*>(&Data[0]);
	}

	const char* GetDataConstPtr() {
		return reinterpret_cast<const char*>(&Data[0]);
	}


	template<typename Type>
	Type GetDataPtrConvert() {
		return reinterpret_cast<Type>(&Data[0]);
	}

	void Write(const void* Data, unsigned int _Size);

	void Read(void* Data, unsigned int _Size);

	void ClearReadData();

	template<typename Type>
	void operator<<(const Type& _Data)
	{
		Write(reinterpret_cast<const void*>(&_Data), sizeof(Type));
	}


	template<typename Type>
	void Write(const Type& _Data)
	{
		Write(reinterpret_cast<const void*>(&_Data), sizeof(Type));
	}


	template<typename Type>
	void Write(std::vector<Type>& _Data)
	{
		unsigned int Size = static_cast<unsigned int>(_Data.size());
		operator<<(Size);

		if (Size <= 0)
		{
			return;
		}

		for (size_t i = 0; i < _Data.size(); i++)
		{
			if (false == std::is_base_of<GameEngineSerializObject, Type>::value)
			{
				Write(_Data[i]);
			}
			else
			{
				GameEngineSerializObject* Ser = reinterpret_cast<GameEngineSerializObject*>(&_Data[i]);
				Ser->Write(*this);
			}
		}
	}


	void operator<<(const std::string& _Value);
	void operator<<(const int _Value);
	void operator<<(const unsigned int _Value);
	void operator<<(const uint64_t& _Value);
	void operator<<(const float _Value);
	void operator<<(const bool _Value);
	void operator<<(const float4& _Value);
	void operator<<(const float4x4& _Value);

	template<typename Type>
	void operator>>(Type& _Data)
	{
		Read(reinterpret_cast<void*>(&_Data), sizeof(Type));
	}

	template<typename Type>
	void Read(Type& _Data)
	{
		Read(reinterpret_cast<void*>(&_Data), sizeof(Type));
	}


	template<typename T>
	void WriteEnum(const T _Value)
	{
		Write(reinterpret_cast<const void*>(&_Value), static_cast<unsigned int>(sizeof(T)));
	}

	template<typename T>
	void WriteUserData(const T _Value)
	{
		Write(reinterpret_cast<const void*>(&_Value), static_cast<unsigned int>(sizeof(T)));
	}

	template<typename Value>
	void operator<<(std::vector<Value>& _Data)
	{
		unsigned int Size = static_cast<unsigned int>(_Data.size());
		operator<<(Size);

		if (Size <= 0)
		{
			return;
		}

		for (size_t i = 0; i < _Data.size(); i++)
		{
			if (false == std::is_base_of<GameEngineSerializObject, Value>::value)
			{
				Write(_Data[i]);
			}
			else
			{
				GameEngineSerializObject* Ser = reinterpret_cast<GameEngineSerializObject*>(&_Data[i]);
				Ser->Write(*this);
			}
		}
	}


	template<typename Key, typename  Value>
	void operator<<(std::map<Key, Value>& _Data)
	{
		unsigned int Size = static_cast<unsigned int>(_Data.size());
		Write(Size);

		if (Size <= 0)
		{
			return;
		}

		for (std::pair<Key, Value> Pair : _Data)
		{
			if (false == std::is_base_of<GameEngineSerializObject, Key>::value)
			{
				Write(Pair.first);
			}
			else
			{
				GameEngineSerializObject* Ser = reinterpret_cast<GameEngineSerializObject*>(&Pair.first);
				Ser->Write(*this);
			}

			if (false == std::is_base_of<GameEngineSerializObject, Value>::value)
			{
				Write(Pair.second);
			}
			else
			{
				GameEngineSerializObject* Ser = reinterpret_cast<GameEngineSerializObject*>(&Pair.second);
				Ser->Write(*this);
			}
		}
	}

	void operator>>(std::string& _Value);
	void operator>>(int& _Value);
	void operator>>(unsigned int& _Value);
	void operator>>(uint64_t& _Value);
	void operator>>(float& _Value);
	void operator>>(bool& _Value);
	void operator>>(float4& _Value);
	void operator>>(float4x4& _Value);


	template<typename Type>
	void Read(std::vector<Type>& _Data)
	{
		unsigned int Size = 0;
		operator>>(Size);

		if (Size <= 0)
		{
			return;
		}

		_Data.resize(Size);

		for (size_t i = 0; i < Size; i++)
		{
			if (false == std::is_base_of<GameEngineSerializObject, Type>::value)
			{
				Read(_Data[i]);
			}
			else
			{
				GameEngineSerializObject* Ser = reinterpret_cast<GameEngineSerializObject*>(&_Data[i]);
				Ser->Read(*this);
			}
		}
	}


	template<typename T>
	void ReadEnum(T& _Value)
	{
		Read(reinterpret_cast<void*>(&_Value), static_cast<unsigned int>(sizeof(T)));
	}

	template<typename T>
	void ReadUserData(T& _Value)
	{
		Read(reinterpret_cast<void*>(&_Value), static_cast<unsigned int>(sizeof(T)));
	}


	template<typename Value>
	void operator>>(std::vector<Value>& _Data)
	{
		unsigned int Size = 0;
		operator>>(Size);

		if (Size <= 0)
		{
			return;
		}

		_Data.resize(Size);

		for (unsigned int i = 0; i < Size; i++)
		{
			if (false == std::is_base_of<GameEngineSerializObject, Value>::value)
			{
				Read(_Data[i]);
			}
			else
			{
				GameEngineSerializObject* Ser = reinterpret_cast<GameEngineSerializObject*>(&_Data[i]);
				Ser->Read(*this);
			}

		}
	}

	template<typename Key, typename  Value>
	void operator>>(std::map<Key, Value>& _Data)
	{
		unsigned int Size = 0;
		Read(Size);

		if (Size <= 0)
		{
			return;
		}

		for (unsigned int i = 0; i < Size; ++i)
		{
			std::pair<Key, Value> Pair;

			if (false == std::is_base_of<GameEngineSerializObject, Key>::value)
			{
				Read(Pair.first);
			}
			else
			{
				GameEngineSerializObject* Ser = reinterpret_cast<GameEngineSerializObject*>(&Pair.first);
				Ser->Read(*this);
			}


			if (false == std::is_base_of<GameEngineSerializObject, Value>::value)
			{
				Read(Pair.second);
			}
			else
			{
				GameEngineSerializObject* Ser = reinterpret_cast<GameEngineSerializObject*>(&Pair.second);
				Ser->Read(*this);
			}

			_Data.insert(Pair);
		}
	}

	const unsigned char* GetConstData() const
	{
		return &Data[0];
	}

	std::string GetString()
	{
		const char* Return = reinterpret_cast<const char*>(&Data[0]);

		return Return;
	}

	void operator << (const GameEngineSerializer& _Ser);

protected:

private:
	unsigned int WriteOffset = 0;
	unsigned int ReadOffset = 0;
	std::vector<unsigned char> Data;

};

