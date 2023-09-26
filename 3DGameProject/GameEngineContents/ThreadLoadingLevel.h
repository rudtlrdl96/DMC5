#pragma once
#include "BaseLevel.h"

class ThreadLoadingLevel : public BaseLevel
{
public:
	static void ChangeLevel(const std::string_view& _NextLevelName)
	{
		NextLevelName = _NextLevelName;
		GameEngineCore::ChangeLevel("ThreadLoadingLevel");
	}



	ThreadLoadingLevel();
	~ThreadLoadingLevel();

	ThreadLoadingLevel(const ThreadLoadingLevel& _Other) = delete;
	ThreadLoadingLevel(ThreadLoadingLevel&& _Other) noexcept = delete;
	ThreadLoadingLevel& operator=(const ThreadLoadingLevel& _Other) = delete;
	ThreadLoadingLevel& operator=(const ThreadLoadingLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void LevelChangeStart() override;

private:
	static std::string NextLevelName;

	//<Level이름, 콜백>
	std::map<std::string, std::vector<std::function<void()>>> AllLoadCallBack;
	size_t LoadWorkCount = 0;
	std::atomic<size_t> ExcuteWorkCount = 0;




	template <typename LevelType, typename ResourceType>
	void PushLoadCallBack(const std::string_view& _Path)
	{
		const type_info& Info = typeid(LevelType);
		std::string LevelName = Info.name();
		LevelName.replace(0, 6, "");

		AllLoadCallBack[LevelName].push_back([=]()
		{
			GameEnginePath FilePath(_Path);
			std::string FileName = FilePath.GetFileName();
			if (nullptr != ResourceType::Find(FileName))
				return;

			GameEngineDirectory StartDir;
			StartDir.MoveParentToDirectory("ContentResources");
			StartDir.Move("ContentResources");
			FileName = StartDir.GetFullPath() + "\\" + FilePath.GetFullPath();

			GameEngineDirectory CheckDir(FileName);
			if (false == CheckDir.IsExists())
			{
				MsgAssert("ThreadLoadingLevel에서 유효하지 않은 경로로 이동하였습니다.\n" + FileName);
				return;
			}
			
			ResourceType::Load(CheckDir.GetFullPath());
		});
	}

	void ThreadLoadStart();
};

