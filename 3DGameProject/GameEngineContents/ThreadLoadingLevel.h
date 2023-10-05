#pragma once
#include "BaseLevel.h"

class ThreadLoadingLevel : public BaseLevel
{
public:
	static ThreadLoadingLevel* GetInst()
	{
		return Inst;
	}

	//�� �Լ��� ���� �ε� ������ Ȱ��ȭ ��ŵ�ϴ�
	static void ChangeLevel(const std::string_view& _NextLevelName)
	{
		NextLevelName = _NextLevelName;
		GameEngineCore::ChangeLevel("ThreadLoadingLevel");
	}

	//�� �Լ��� ���� �ε� ������ Ȱ��ȭ ��ŵ�ϴ�
	template <typename LevelType>
	static void ChangeLevel()
	{
		const type_info& Info = typeid(LevelType);
		std::string LevelName = Info.name();
		LevelName.replace(0, 6, "");

		ChangeLevel(LevelName);
	}

	//���￹��
	std::shared_ptr<class GameEngineActor> ThreadTestActor = nullptr;

	ThreadLoadingLevel();
	~ThreadLoadingLevel();

	ThreadLoadingLevel(const ThreadLoadingLevel& _Other) = delete;
	ThreadLoadingLevel(ThreadLoadingLevel&& _Other) noexcept = delete;
	ThreadLoadingLevel& operator=(const ThreadLoadingLevel& _Other) = delete;
	ThreadLoadingLevel& operator=(const ThreadLoadingLevel&& _Other) noexcept = delete;

	//���� �ε� �ۼ�Ʈ�� �˷��ݴϴ�
	inline float GetLoadingPercent() const
	{
		return LoadingPercent;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void LevelChangeStart() override;

private:
	static ThreadLoadingLevel* Inst;
	static std::string NextLevelName;

	//<Level�̸�, �ݹ�>
	std::map<std::string, std::vector<std::function<void()>>> AllLoadCallBack;
	size_t LoadWorkCount = 0;
	std::atomic<size_t> ExcuteWorkCount = 0;
	float LoadingPercent = 0.f;
	
	std::map<std::string, std::mutex> AllResMutex;

	
	/// <summary>
	/// Ư�� ������ ��ȯ�� �� ���ҽ� 1���� ������ �ε���Ű�� �Լ��Դϴ�.
	/// </summary>
	/// <typeparam name="LevelType">� ������ �Ѿ �� ������ �ε��� ȣ���� �� �־��ּ���</typeparam>
	/// <typeparam name="ResourceType">� Ÿ���� ���ҽ��� �ε��ϴ� �� �־��ּ���</typeparam>
	/// <param name="_Path">ContentResources ������ ��θ� �־��ּ���</param>
	template <typename LevelType, typename ResourceType>
	void PushLoadCallBack(const std::string_view& _Path)
	{
		//LevelType ���ø� ������ Ŭ���� �� ���ϱ�
		const type_info& Info = typeid(LevelType);
		std::string LevelName = Info.name();
		LevelName.replace(0, 6, "");

		
		GameEnginePath FilePath(_Path);
		const std::string& FileName = FilePath.GetFileName();
		AllResMutex[FileName];

		//�ݹ� ����
		AllLoadCallBack[LevelName].push_back([FilePath, this]()
		{
			const std::string& FileName = FilePath.GetFileName();

			GameEngineDirectory StartDir;
			StartDir.MoveParentToDirectory("ContentResources");
			StartDir.Move("ContentResources");
			GameEngineDirectory CheckDir(StartDir.GetFullPath() + "\\" + FilePath.GetFullPath());
			if (false == CheckDir.IsExists())
			{
				MsgAssert("ThreadLoadingLevel���� ��ȿ���� ���� ��η� �̵��Ͽ����ϴ�.\n" + FileName);
				return;
			}

			std::lock_guard<std::mutex> Lock(AllResMutex[FileName]);
			if (nullptr != ResourceType::Find(FileName))
				return;
			
			ResourceType::Load(CheckDir.GetFullPath());
		});
	}

	/// <summary>
	/// Ư�� ������ ��ȯ�� �� �ش� ����� ��� ���ҽ��� ã�� ������ �ε� ��Ű�� �Լ��Դϴ�
	/// </summary>
	/// <typeparam name="LevelType">� ������ �Ѿ �� ������ �ε��� ȣ���� �� �־��ּ���</typeparam>
	/// <typeparam name="ResourceType">� Ÿ���� ���ҽ��� �ε��ϴ� �� �־��ּ���</typeparam>
	/// <param name="_Path">ContentResources ������ ��θ� �־��ּ���</param>
	/// <param name="_Extents">�ε��� ���ҽ����� Ȯ���ڸ� �־��ּ���</param>
	template <typename LevelType, typename ResourceType>
	void PushAllLoadCallBack(const std::string_view& _Path, const std::vector<std::string_view>& _Extents)
	{
		if (true == _Extents.empty())
		{
			MsgAssert("Thread�ε��� �� ���ҽ��� Ȯ���� Ÿ���� �� �־��־����ϴ�");
			return;
		}

		GameEngineDirectory StartDir;
		StartDir.MoveParentToDirectory("ContentResources");
		StartDir.Move("ContentResources");

		std::string FilePath = StartDir.GetFullPath() + "\\" + GameEnginePath(_Path).GetFullPath();
		GameEngineDirectory CheckDir = GameEngineDirectory(FilePath);
		if (false == CheckDir.IsExists())
		{
			MsgAssert("ThreadLoadingLevel���� ��ȿ���� ���� ��η� �̵��Ͽ����ϴ�.\n" + FilePath);
			return;
		}

		const std::vector<GameEngineFile>& Files = CheckDir.GetAllFile(_Extents);
		for (const GameEngineFile& File : Files)
		{
			std::string RelativePath = File.GetFullPath();
			size_t CutIdx = RelativePath.find("ContentResources");
			RelativePath.replace(0, CutIdx + 17, "");
			PushLoadCallBack<LevelType, ResourceType>(RelativePath);
		}
	}

	void ThreadLoadStart();
};

