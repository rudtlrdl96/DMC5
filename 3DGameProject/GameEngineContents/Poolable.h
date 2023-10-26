#pragma once
#include "ContentsCore.h"

class GameEngineLevel;

template<typename ActorType>
class Poolable
{
public:
	/// <summary>
	/// ���ø��� �ش�Ǵ� ������Ʈ Ǯ�� �����մϴ�
	/// </summary>
	/// <param name="_Level">��ü�� �������� �����մϴ�</param>
	/// <param name="_Order">��ü�� �����Դϴ�</param>
	/// <param name="_Size">Ǯ�� ������ �� �������Դϴ�</param>
	/// <param name="_CallBack">Ǯ���� ������Ʈ�� ������ �� ó���� �ݹ��Դϴ�</param>
	static void CreatePool(
		GameEngineLevel* _Level, 
		int _Order = 0, size_t _Size = 5, 
		std::function<void(std::shared_ptr<ActorType>)> _CallBack = nullptr)
	{
		if (0 == _Size)
		{
			MsgAssert("Ǯ�� ����� 0���� �ʱ�ȭ�Ҽ��� �����ϴ�");
			return;
		}

		//ó�� Ǯ���ϴ� ����
		if (0 == Pool[_Level].capacity())
		{
			ContentsCore::PushGameEndCallBack(Poolable<ActorType>::Release);
			CreateCallBack = _CallBack;
		}

		Pool[_Level].reserve(_Size);
		for (size_t i = 0; i < _Size; ++i)
		{
			std::shared_ptr<ActorType> ActorPtr = nullptr;
			ActorPtr = Create(_Level, _Order);

			if (nullptr == CreateCallBack)
				continue;

			CreateCallBack(ActorPtr);
		}
	}

	/// <summary>
	/// Ǯ���� ������Ʈ�� �����ɴϴ�
	/// </summary>
	/// <param name="_Level">��ü�� �������� �����մϴ�</param>
	/// <param name="_Order">Ǯ�� ��ü�� ���� ��� ���Ӱ� ��������� �ش� ������ �����˴ϴ�</param>
	/// <param name="_Size">Ǯ�� ����������� ���ٸ� �ش� ������� Ǯ�� ����ϴ�</param>
	/// <param name="_CallBack">Ǯ�� ����������� ���� ��� Ǯ�� ����� �� �ݹ��� �����ŵ�ϴ�</param>
	/// <returns>��ȯ���� Ǯ���� ������ ��ü�Դϴ�</returns>
	static std::shared_ptr<ActorType> PopFromPool(
		GameEngineLevel* _Level, 
		int _Order = 0, size_t _Size = 5,
		std::function<void(std::shared_ptr<ActorType>)> _CallBack = nullptr)
	{
		//Ǯ�� ������Ʈ�� �������� ���� ���
		if (true == Pool[_Level].empty())
		{
			size_t Size = _Size;
			size_t Capacity = Pool[_Level].capacity();

			//������ Ǯ�� ������� ���� �ִ� ���
			if (0 != Capacity)
			{
				Size = Capacity;
			}

			CreatePool(_Level, _Order, Size, _CallBack);
		}

		std::shared_ptr<ActorType> ActorPtr = nullptr;
		ActorPtr = Pool[_Level].back();
		Pool[_Level].pop_back();

		ActorPtr->On();
		return ActorPtr;
	}

private:
	static std::map<GameEngineLevel*, std::vector<std::shared_ptr<ActorType>>> Pool;
	static std::function<void(std::shared_ptr<ActorType>)> CreateCallBack;

	static std::shared_ptr<ActorType> Create(GameEngineLevel* _Level, int _Order = 0)
	{
		std::shared_ptr<ActorType> ActorPtr = _Level->CreateActor<ActorType>(_Order);
		Pool[_Level].push_back(ActorPtr);

		ActorPtr->Off();
		return ActorPtr;
	}

	static void PushToPool(std::shared_ptr<ActorType> _Actor)
	{
		if (true == _Actor->IsDeath())
		{
			MsgAssert("�̹� Death�� ������Ʈ�� Ǯ�� �������� �߽��ϴ�");
			return;
		}

		_Actor->Off();

		std::shared_ptr<GameEngineLevel> Level = nullptr;
		Level = _Actor->GetLevel();
		Pool[Level.get()].push_back(_Actor);
	}

	static void Release()
	{
		auto StartIter = Pool.begin();
		auto EndIter = Pool.end();

		while (StartIter != EndIter)
		{
			StartIter->second.clear();
			++StartIter;
		}

		Pool.clear();
		CreateCallBack = nullptr;
	}

public:
	Poolable() {}
	virtual ~Poolable() = 0 {}

	Poolable(const Poolable& _Other) = delete;
	Poolable(Poolable&& _Other) noexcept = delete;
	Poolable& operator=(const Poolable& _Other) = delete;
	Poolable& operator=(const Poolable&& _Other) noexcept = delete;

	//�� ����� ������Ʈ�� �ٽ� Ǯ�� ���� �ֽ��ϴ�
	void ReturnToPool()
	{
		ActorType* ActorPtr = this;
		std::shared_ptr<ActorType> ActorSPtr = ActorPtr->DynamicThis();
		Poolable<ActorType>::PushToPool(ActorSPtr);
	}

protected:


private:

};

template<typename ActorType>
std::map<GameEngineLevel*, std::vector<std::shared_ptr<ActorType>>> Poolable<ActorType>::Pool;

template<typename ActorType>
std::function<void(std::shared_ptr<ActorType>)> Poolable<ActorType>::CreateCallBack = nullptr;

