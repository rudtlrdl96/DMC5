#include "PrecompileHeader.h"
#include "GameEngineSerializer.h"
#include "GameEngineDebug.h"

GameEngineSerializer::GameEngineSerializer() 
{
}

GameEngineSerializer::~GameEngineSerializer() 
{
}

void GameEngineSerializer::BufferResize(size_t _Size)
{
	Datas.resize(_Size);
}

// 
// [][][][][][][][][][]

void GameEngineSerializer::Write(const int& _Value)
{
	Write(&_Value, sizeof(int));
}

void GameEngineSerializer::Write(const std::string_view& _Value)
{
	int Size = static_cast<int>(_Value.size());
	Write(&Size, sizeof(int));
	Write(_Value.data(), Size);
}


void GameEngineSerializer::Write(const void* _Ptr, size_t _Size)
{
	//  1024             1023   + 4
	if (Datas.size() <= Offset + _Size)
	{
		Datas.resize(Datas.size() + 1024);
	}

	//       여기에 복사해라   복사할 위치의 여유크기는
	memcpy_s(&Datas[Offset], Datas.size() - Offset, _Ptr, _Size);

	Offset += _Size;
}


void GameEngineSerializer::Read(int& _Value)
{
	Read(&_Value, sizeof(int));
}

void GameEngineSerializer::Read(std::string& _Value)
{
	int Size = 0;
	Read(&Size, sizeof(int));
	_Value.clear();
	_Value.resize(Size);
	Read(_Value.data(), Size);
}

void GameEngineSerializer::Read(void* _Ptr, size_t _Size)
{
	//  1024             1023   + 4
	if (Datas.size() < Offset + _Size)
	{
		MsgAssert("데이터의 한도이상으로 읽으려고 했습니다.");
	}

	memcpy_s(_Ptr, _Size, &Datas[Offset], _Size);

	Offset += _Size;
}

std::string GameEngineSerializer::GetString()
{
	std::string AllString;

	AllString.resize(Datas.size());

	memcpy_s(&AllString[0], Datas.size(), &Datas[0], Datas.size());

	return AllString;
}