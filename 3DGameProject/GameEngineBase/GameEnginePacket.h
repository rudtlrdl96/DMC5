#pragma once
#include "GameEngineSerializer.h"
#include "GameEngineDebug.h"

class GameEnginePacket : public std::enable_shared_from_this<GameEnginePacket>
{
public:
	GameEnginePacket();
	virtual ~GameEnginePacket() = 0;

	GameEnginePacket(const GameEnginePacket& _Other) = delete;
	GameEnginePacket(GameEnginePacket&& _Other) noexcept = delete;
	GameEnginePacket& operator=(const GameEnginePacket& _Other) = delete;
	GameEnginePacket& operator=(const GameEnginePacket&& _Other) noexcept = delete;

	template<typename EnumType>
	void SetPacketID(EnumType _PacketID)
	{
		PacketID = static_cast<int>(_PacketID);
	}

	void SetPacketID(unsigned int _PacketID)
	{
		PacketID = _PacketID;
	}



	void SerializePacket(GameEngineSerializer& _Ser)
	{
		//패킷을 버퍼에 직렬화하기 전 위치
		unsigned int WriteStartPos = _Ser.GetWriteOffSet();

		//이 패킷의 멤버변수 직렬화
		Serialize(_Ser);
		//패킷 사이즈 설정
		SerializeEnd(_Ser, WriteStartPos);
	}

	void DeSerializePacket(GameEngineSerializer& _Ser)
	{
		//역 직렬화
		DeSeralize(_Ser);
	}



	inline unsigned int GetPacketID() const
	{
		return PacketID;
	}

	inline unsigned int GetObjectID() const
	{
		return ObjectID;
	}

	inline void SetObjectID(unsigned int _ObjectID)
	{
		ObjectID = _ObjectID;
	}

	inline bool IsActiveRecv() const
	{
		return IsActiveRecvValue;
	}

	inline void SetActiveRecv()
	{
		IsActiveRecvValue = true;
	}

protected:
	virtual void Serialize(GameEngineSerializer& _Ser)
	{
		_Ser << PacketID;
		_Ser << Size;
		_Ser << ObjectID;
	}

	virtual void DeSeralize(GameEngineSerializer& _Ser)
	{
		_Ser >> PacketID;
		_Ser >> Size;
		_Ser >> ObjectID;
	}

private:
	//패킷 아이디
	unsigned int PacketID = -1; 
	//패킷 크기
	unsigned int Size = -1; 
	//오브젝트 아이디
	unsigned int ObjectID = -1; 

	/// <summary>
	/// Active컨트롤인 객체가 이 패킷을 처리하는지 여부, 이 값은 네트워크 직렬화가 이루어지지 않음
	/// </summary>
	bool IsActiveRecvValue = false;

	//패킷의 크기는 모든 맴버변수를 GameEngineSerializer에 집어 넣고 결정된다. 직렬화가 끝난 이후 이 패킷의 총 크기를 알기위한 함수
	void SerializeEnd(GameEngineSerializer& _Ser, unsigned int _PacketStartPos)
	{
		Size = _Ser.GetWriteOffSet() - _PacketStartPos;
		if (Size <= 0)
		{
			MsgAssert("0바이트 패킷이 감지되었습니다.");
		}

		// 이녀석의 4바이트 번째의 데이터를 size로 교체한다.
		unsigned char* Ptr = _Ser.GetDataPtr();
		memcpy_s(&Ptr[_PacketStartPos + 4], sizeof(int), &Size, sizeof(int));
	}
};

