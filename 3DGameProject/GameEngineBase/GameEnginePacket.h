#pragma once
#include "GameEngineSerializer.h"
#include "GameEngineDebug.h"

class GameEnginePacket : public std::enable_shared_from_this<GameEnginePacket>
{
public:
	GameEnginePacket();
	virtual ~GameEnginePacket();

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
		//�� ��Ŷ�� ������� ����ȭ
		Serialize(_Ser);
		//��Ŷ ������ ����
		SerializeEnd(_Ser);
	}

	void DeSerializePacket(GameEngineSerializer& _Ser)
	{
		//�� ����ȭ
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
	//��Ŷ ���̵�
	unsigned int PacketID = -1; 
	//��Ŷ ũ��
	unsigned int Size = -1; 
	//������Ʈ ���̵�
	unsigned int ObjectID = -1; 

	//��Ŷ�� ũ��� ��� �ɹ������� GameEngineSerializer�� ���� �ְ� �����ȴ�. ����ȭ�� ���� ���� �� ��Ŷ�� �� ũ�⸦ �˱����� �Լ�
	void SerializeEnd(GameEngineSerializer& _Ser)
	{
		Size = _Ser.GetWriteOffSet();
		if (Size <= 0)
		{
			MsgAssert("0����Ʈ ��Ŷ�� �����Ǿ����ϴ�.");
		}

		// �̳༮�� 4����Ʈ ��°�� �����͸� size�� ��ü�Ѵ�.
		unsigned char* Ptr = _Ser.GetDataPtr();
		memcpy_s(&Ptr[4], sizeof(int), &Size, sizeof(int));
	}
};

