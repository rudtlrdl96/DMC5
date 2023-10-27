#pragma once
#include <string>
#include <string_view>
#include <map>
#include <list>
#include <memory>

#include <GameEngineCore/GameEngineNameObject.h>
#include <GameEngineBase/GameEngineString.h>
#include <GameEngineBase/GameEngineDebug.h>

#include "GameEngineDevice.h"

class GameEngineResourceBase
{
public:
	GameEngineResourceBase(){}
	virtual ~GameEngineResourceBase() = 0 {}

	virtual bool Initialize() { return false; }
};

// 설명 :
template<typename ResourcesType>
class GameEngineResource : public GameEngineNameObject, public GameEngineResourceBase
{
public:
	friend class GameEngineCore;

	// constrcuter destructer
	GameEngineResource() {}
	virtual ~GameEngineResource() = 0 {}

	// delete Function
	GameEngineResource(const GameEngineResource& _Other) = delete;
	GameEngineResource(GameEngineResource&& _Other) noexcept = delete;
	GameEngineResource& operator=(const GameEngineResource& _Other) = delete;
	GameEngineResource& operator=(GameEngineResource&& _Other) noexcept = delete;

	std::string_view GetPath()
	{
		return Path.c_str();
	}

	std::string GetPathToString()
	{
		return Path.c_str();
	}

	void SetPath(const std::string_view& _Value)
	{
		Path = _Value;
	}

	static std::shared_ptr<ResourcesType> Find(const std::string_view& _Name)
	{
		std::string UpperName = GameEngineString::ToUpper(_Name);

		std::lock_guard<std::mutex> Lock(NameLock);
		if (NamedResources.end() == NamedResources.find(UpperName.c_str()))
		{
			return nullptr;
		}

		return NamedResources[UpperName];
	}

	static void Remove(const std::string_view& _Name)
	{
		std::string UpperName = GameEngineString::ToUpper(_Name);

		if (NamedResources.end() == NamedResources.find(UpperName.c_str()))
		{
			return;
		}

		NamedResources.erase(NamedResources.find(UpperName.c_str()));

		return;
	}

	virtual void Setting() {}

	static void ResourcesClear()
	{
		NamedResources.clear();
		UnNamedRes.clear();
	}

protected:
	static std::shared_ptr<ResourcesType> CreateUnNamed()
	{
		std::shared_ptr<ResourcesType> NewRes = std::make_shared<ResourcesType>();
		std::lock_guard<std::mutex> Lock(UnNamedLock);
		UnNamedRes.push_back(NewRes);

		return NewRes;
	}

	static std::shared_ptr<ResourcesType> Create(const std::string_view& _Name)
	{
		std::string UpperName = GameEngineString::ToUpper(_Name);

		if (nullptr != Find(UpperName))
		{
			MsgAssert("이미 존재하는 이름의 리소스를 또 만들려고 했습니다.");

			return nullptr;
		}

		std::shared_ptr<ResourcesType> NewRes = std::make_shared<ResourcesType>();
		NewRes->SetName(UpperName);

		std::lock_guard<std::mutex> Lock(NameLock);
		NamedResources.insert(std::map<std::string, std::shared_ptr<ResourcesType>>::value_type(UpperName, NewRes));

		return NewRes;
	}

	std::string Path;

private:
	static std::map<std::string, std::shared_ptr<ResourcesType>> NamedResources;
	static std::list<std::shared_ptr<ResourcesType>> UnNamedRes;
	static std::mutex NameLock;
	static std::mutex UnNamedLock;

};

// 템플릿 클래스에서 데이터 영역의 초기화를 하는 방법들
template<typename ResourcesType>
std::map<std::string, std::shared_ptr<ResourcesType>> GameEngineResource<ResourcesType>::NamedResources;

template<typename ResourcesType>
std::mutex GameEngineResource<ResourcesType>::NameLock;

template<typename ResourcesType>
std::list<std::shared_ptr<ResourcesType>> GameEngineResource<ResourcesType>::UnNamedRes;

template<typename ResourcesType>
std::mutex GameEngineResource<ResourcesType>::UnNamedLock;
