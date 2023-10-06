#pragma once
#include "ContentsCore.h"

class GameEngineLevel;

template<typename ActorType>
class Poolable
{
public:
	/// <summary>
	/// 템플릿에 해당되는 오브젝트 풀을 생성합니다
	/// </summary>
	/// <param name="_Level">객체를 레벨별로 관리합니다</param>
	/// <param name="_Order">객체의 오더입니다</param>
	/// <param name="_Size">풀을 생성할 때 사이즈입니다</param>
	/// <param name="_CallBack">풀에서 오브젝트를 생성한 뒤 처리할 콜백입니다</param>
	static void CreatePool(
		GameEngineLevel* _Level, 
		int _Order = 0, size_t _Size = 5, 
		std::function<void(std::shared_ptr<ActorType>)> _CallBack = nullptr)
	{
		if (0 == _Size)
		{
			MsgAssert("풀의 사이즈를 0으로 초기화할수는 없습니다");
			return;
		}

		//처음 풀링하는 순간
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
	/// 풀에서 오브젝트를 꺼내옵니다
	/// </summary>
	/// <param name="_Level">객체를 레벨별로 관리합니다</param>
	/// <param name="_Order">풀에 객체가 없는 경우 새롭게 만들어지고 해당 오더로 지정됩니다</param>
	/// <param name="_Size">풀을 만들어준적이 없다면 해당 사이즈로 풀을 만듭니다</param>
	/// <param name="_CallBack">풀을 만들어준적이 없는 경우 풀을 만들고 이 콜백을 실행시킵니다</param>
	/// <returns>반환값은 풀에서 꺼내온 객체입니다</returns>
	static std::shared_ptr<ActorType> PopFromPool(
		GameEngineLevel* _Level, 
		int _Order = 0, size_t _Size = 5,
		std::function<void(std::shared_ptr<ActorType>)> _CallBack = nullptr)
	{
		//풀에 오브젝트가 존재하지 않은 경우
		if (true == Pool[_Level].empty())
		{
			size_t Size = _Size;
			size_t Capacity = Pool[_Level].capacity();

			//이전에 풀을 만들어준 적이 있는 경우
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
			MsgAssert("이미 Death된 오브젝트를 풀에 넣으려고 했습니다");
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
	}

public:
	Poolable() {}
	virtual ~Poolable() = 0 {}

	Poolable(const Poolable& _Other) = delete;
	Poolable(Poolable&& _Other) noexcept = delete;
	Poolable& operator=(const Poolable& _Other) = delete;
	Poolable& operator=(const Poolable&& _Other) noexcept = delete;

	//다 사용한 오브젝트를 다시 풀에 집어 넣습니다
	void ReturnToPool()
	{
		ActorType* ActorPtr = this;
		std::shared_ptr<ActorType> ActorSPtr = ActorPtr->DynamicThis();
		Poolable<ActorType>::PushToPool(ActorSPtr);
	}

protected:


private:
	//이런 콜백 방식 또는 추상함수 방식으로 하자
	std::function<void()> PopCallBack = nullptr;
	std::function<void()> PushCallBack = nullptr;

};

template<typename ActorType>
std::map<GameEngineLevel*, std::vector<std::shared_ptr<ActorType>>> Poolable<ActorType>::Pool;

template<typename ActorType>
std::function<void(std::shared_ptr<ActorType>)> Poolable<ActorType>::CreateCallBack = nullptr;

