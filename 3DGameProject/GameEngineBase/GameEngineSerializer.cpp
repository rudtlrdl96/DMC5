#include "PrecompileHeader.h"
#include "GameEngineSerializer.h"
#include "GameEngineDebug.h"


GameEngineSerializer::GameEngineSerializer()
	: WriteOffset(0)
	, ReadOffset(0)
{
	Data.resize(1024);
}

GameEngineSerializer::GameEngineSerializer(const char* _Data, unsigned int _Size)
	: WriteOffset(0)
	, ReadOffset(0)
{

	Data.resize(_Size);
	memcpy_s(&Data[0], _Size, _Data, _Size);
}

GameEngineSerializer::~GameEngineSerializer()
{
}

void GameEngineSerializer::Read(void* _Data, unsigned int _Size)
{
	memcpy_s(_Data, _Size, &Data[ReadOffset], _Size);
	ReadOffset += _Size;
}

void GameEngineSerializer::Write(const void* _Data, unsigned int _Size)
{
	/*if (WriteOffset + _Size >= Data.size())
	{
		Data.resize(Data.capacity() * 2);
	}*/

	while (Data.size() <= (WriteOffset + _Size))
	{
		Data.resize(Data.capacity() * 2);
	}

	memcpy_s(&Data[WriteOffset], _Size, _Data, _Size);
	WriteOffset += _Size;
}

void GameEngineSerializer::operator<<(const int _Value)
{
	Write(&_Value, sizeof(int));
}

void GameEngineSerializer::operator<<(const float _Value)
{
	Write(&_Value, sizeof(float));
}

void GameEngineSerializer::operator<<(const bool _Value)
{
	Write(&_Value, sizeof(bool));
}

void GameEngineSerializer::operator<<(const std::string& _Value)
{
	operator<<(static_cast<unsigned int>(_Value.size()));
	Write(reinterpret_cast<const void*>(&_Value[0]), static_cast<unsigned int>(_Value.size()));
}

void GameEngineSerializer::operator<<(const unsigned int _Value)
{
	Write(&_Value, sizeof(unsigned int));
}

void GameEngineSerializer::operator<<(const uint64_t& _Value)
{
	Write(&_Value, sizeof(uint64_t));
}

void GameEngineSerializer::operator<<(const float4& _Value)
{
	Write(&_Value, sizeof(float4));
}

void GameEngineSerializer::operator<<(const float4x4& _Value)
{
	Write(&_Value, sizeof(float4x4));
}



void GameEngineSerializer::operator>>(std::string& _Value)
{
	int Size;
	operator>>(Size);
	if (0 >= Size)
	{
		return;
	}
	_Value.resize(Size);
	Read(&_Value[0], Size);
}

void GameEngineSerializer::operator>>(int& _Value)
{
	Read(&_Value, sizeof(int));
}

void GameEngineSerializer::operator>>(unsigned int& _Value)
{
	Read(&_Value, sizeof(unsigned int));
}

void GameEngineSerializer::operator>>(float& _Value)
{
	Read(&_Value, sizeof(float));
}

void GameEngineSerializer::operator>>(bool& _Value)
{
	Read(&_Value, sizeof(bool));
}

void GameEngineSerializer::operator>>(float4& _Value)
{
	Read(&_Value, sizeof(float4));
}

void GameEngineSerializer::operator>>(float4x4& _Value)
{
	Read(&_Value, sizeof(float4x4));
}

void GameEngineSerializer::operator>>(uint64_t& _Value)
{
	Read(&_Value, sizeof(uint64_t));
}

void GameEngineSerializer::ClearReadData()
{
	//memcpy_s(&Data[0], WriteOffset, &Data[ReadOffset], WriteOffset - ReadOffset);
	memcpy_s(&Data[0], WriteOffset - ReadOffset, &Data[ReadOffset], WriteOffset - ReadOffset);
	WriteOffset -= ReadOffset;
	ReadOffset = 0;
}

void GameEngineSerializer::operator << (const GameEngineSerializer& _Ser)
{
	unsigned int MoveSize = (_Ser.WriteOffset - _Ser.ReadOffset);
	if (MoveSize == 0)
		return;

	Write(&_Ser.Data[_Ser.ReadOffset], MoveSize);
}