#pragma once
#include "BaseLevel.h"

class ThreadLoadingLevel : public BaseLevel
{
public:
	//이 함수를 통해 로딩 레벨을 활성화 시킵니다
	static void ChangeLevel(const std::string_view& _NextLevelName)
	{
		PrevLevelName = GameEngineCore::GetCurLevel()->GetName();
		std::string UpperName = GameEngineString::ToUpper(_NextLevelName);

		//해당 레벨에 로딩할 대상, 언로드 대상이 없는 경우엔
		if (true == Inst->AllThreadUnLoadCallBack[UpperName].empty() && true == Inst->AllThreadLoadCallBack[UpperName].empty())
		{
			GameEngineCore::ChangeLevel(UpperName);
			return;
		}
		
		NextLevelName = UpperName;
		GameEngineCore::ChangeLevel("ThreadLoadingLevel");
	}

	//이 함수를 통해 로딩 레벨을 활성화 시킵니다
	template <typename LevelType>
	static void ChangeLevel()
	{
		//LevelType 템플릿 인자의 클래스 명 구하기
		const std::string& LevelName = GetLevelName<LevelType>();
		ChangeLevel(LevelName);
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
	static std::string PrevLevelName;
	static std::string NextLevelName;
	static ThreadLoadingLevel* Inst;

	std::shared_ptr<class NeroLoading> LoadingUI = nullptr;


	//<Level이름, 콜백> : 멀티스레드를 사용하는 콜백
	std::map<std::string, std::vector<std::function<void()>>> AllThreadLoadCallBack;
	std::map<std::string, std::vector<std::function<void()>>> AllThreadUnLoadCallBack;
	std::map<std::string, std::mutex> AllResMutex;




	size_t UnLoadWorkCount = 0;
	size_t LoadWorkCount = 0;
	size_t AllWorkCount = 0;
	std::atomic<size_t> ExcuteWorkCount = 0;
	float LoadingPercent = 0.f;
	bool IsUnload = false;	// 현재 언로드중일시 true
	bool IsLoad = false;	// 현재 로드중일시  true

	void StartUnLoad();
	void StartLoad();




	//LevelType 템플릿 인자의 클래스 명 구하기
	template <typename LevelType>
	std::string GetLevelName()
	{
		const type_info& Info = typeid(LevelType);
		std::string LevelName = Info.name();
		LevelName.replace(0, 6, "");

		return GameEngineString::ToUpper(LevelName);
	}


	/// <summary>
	/// 특정 레벨로 전환될 때 리소스 1개를 스레드 로딩시키는 함수입니다.
	/// </summary>
	/// <typeparam name="LevelType">어떤 레벨로 넘어갈 때 스레드 로딩을 호출할 지 넣어주세요</typeparam>
	/// <typeparam name="ResourceType">어떤 타입의 리소스를 로드하는 지 넣어주세요</typeparam>
	/// <param name="_Path">ContentResources 이후의 경로를 넣어주세요</param>
	template <typename LevelType, typename ResourceType>
	void PushLoadCallBack(const std::string_view& _Path)
	{
		//LevelType 템플릿 인자의 클래스 명 구하기
		const std::string& LevelName = GetLevelName<LevelType>();
		
		GameEnginePath FilePath(_Path);
		const std::string& FolderPath = FilePath.GetFolderPath(FilePath.GetFullPath());
		AllResMutex[FolderPath];

		//콜백 저장
		AllThreadLoadCallBack[LevelName].push_back([FilePath, this]()
		{
			const std::string& FileName = FilePath.GetFileName();

			GameEngineDirectory StartDir;
			StartDir.MoveParentToDirectory("ContentResources");
			StartDir.Move("ContentResources");
			GameEngineDirectory CheckDir(StartDir.GetFullPath() + "\\" + FilePath.GetFullPath());
			if (false == CheckDir.IsExists())
			{
				MsgAssert("ThreadLoadingLevel에서 유효하지 않은 경로로 이동하였습니다.\n" + FileName);
				return;
			}

			const std::string& FolderPath = FilePath.GetFolderPath(FilePath.GetFullPath());
			std::lock_guard<std::mutex> Lock(AllResMutex[FolderPath]);
			if (nullptr != ResourceType::Find(FileName))
				return;

			//로딩
			ResourceType::Load(CheckDir.GetFullPath());

			//사운드는 Initialize함수가 없으므로 제외
			if (true == std::is_base_of<GameEngineSound, ResourceType>::value)
				return;

			std::shared_ptr<ResourceType> ResPtr = ResourceType::Find(FileName);
			InitResource(ResPtr.get());	
		});
	}

	void InitResource(void* _Ptr);


	/// <summary>
	/// 특정 레벨로 전환될 때 해당 경로의 모든 리소스를 찾아 스레드 로딩 시키는 함수입니다
	/// </summary>
	/// <typeparam name="LevelType">어떤 레벨로 넘어갈 때 스레드 로딩을 호출할 지 넣어주세요</typeparam>
	/// <typeparam name="ResourceType">어떤 타입의 리소스를 로드하는 지 넣어주세요</typeparam>
	/// <param name="_Path">ContentResources 이후의 경로를 넣어주세요</param>
	/// <param name="_Extents">로드할 리소스들의 확장자를 넣어주세요</param>
	template <typename LevelType, typename ResourceType>
	void PushAllLoadCallBack(const std::string_view& _Path, const std::vector<std::string_view>& _Extents)
	{
		if (true == _Extents.empty())
		{
			MsgAssert("Thread로딩을 할 리소스의 확장자 타입을 안 넣어주었습니다");
			return;
		}

		GameEngineDirectory StartDir;
		StartDir.MoveParentToDirectory("ContentResources");
		StartDir.Move("ContentResources");

		std::string FilePath = StartDir.GetFullPath() + "\\" + GameEnginePath(_Path).GetFullPath();
		GameEngineDirectory CheckDir = GameEngineDirectory(FilePath);
		if (false == CheckDir.IsExists())
		{
			MsgAssert("ThreadLoadingLevel에서 유효하지 않은 경로로 이동하였습니다.\n" + FilePath);
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

	/// <summary>
/// 특정 레벨이 끝날 때 리소스 1개를 스레드 언로딩시키는 함수입니다.
/// </summary>
/// <typeparam name="LevelType">어떤 레벨이 끝날 때 스레드 언로딩을 호출할 지 넣어주세요</typeparam>
/// <typeparam name="ResourceType">어떤 타입의 리소스를 언로드하는 지 넣어주세요</typeparam>
/// <param name="_Path">ContentResources 이후의 경로를 넣어주세요</param>
	template <typename LevelType, typename ResourceType>
	void PushUnLoadCallBack(const std::string_view& _Path)
	{
		//LevelType 템플릿 인자의 클래스 명 구하기
		const std::string& LevelName = GetLevelName<LevelType>();

		GameEnginePath FilePath(_Path);
		const std::string& FolderPath = FilePath.GetFolderPath(FilePath.GetFullPath());
		AllResMutex[FolderPath];

		//콜백 저장
		AllThreadUnLoadCallBack[LevelName].push_back([FilePath, this]()
			{
				const std::string& FileName = FilePath.GetFileName();
				const std::string& FolderPath = FilePath.GetFolderPath(FilePath.GetFullPath());
				std::lock_guard<std::mutex> Lock(AllResMutex[FolderPath]);
				if (nullptr == ResourceType::Find(FileName))
					return;

				//언로딩
				ResourceType::UnLoad(FileName);
			});
	}

	/// <summary>
/// 특정 레벨이 끝날 때 해당 경로의 모든 리소스를 찾아 스레드 언로딩 시키는 함수입니다
/// </summary>
/// <typeparam name="LevelType">어떤 레벨이 끝날 때 스레드 언로딩을 호출할 지 넣어주세요</typeparam>
/// <typeparam name="ResourceType">어떤 타입의 리소스를 로드하는 지 넣어주세요</typeparam>
/// <param name="_Path">ContentResources 이후의 경로를 넣어주세요</param>
/// <param name="_Extents">언로드할 리소스들의 확장자를 넣어주세요</param>
	template <typename LevelType, typename ResourceType>
	void PushAllUnLoadCallBack(const std::string_view& _Path, const std::vector<std::string_view>& _Extents)
	{
		if (true == _Extents.empty())
		{
			MsgAssert("Thread언로딩을 할 리소스의 확장자 타입을 안 넣어주었습니다");
			return;
		}

		GameEngineDirectory StartDir;
		StartDir.MoveParentToDirectory("ContentResources");
		StartDir.Move("ContentResources");

		std::string FilePath = StartDir.GetFullPath() + "\\" + GameEnginePath(_Path).GetFullPath();
		GameEngineDirectory CheckDir = GameEngineDirectory(FilePath);
		if (false == CheckDir.IsExists())
		{
			MsgAssert("ThreadLoadingLevel에서 유효하지 않은 경로로 이동하였습니다.\n" + FilePath);
			return;
		}

		const std::vector<GameEngineFile>& Files = CheckDir.GetAllFile(_Extents);
		for (const GameEngineFile& File : Files)
		{
			std::string RelativePath = File.GetFullPath();
			size_t CutIdx = RelativePath.find("ContentResources");
			RelativePath.replace(0, CutIdx + 17, "");

			PushUnLoadCallBack<LevelType, ResourceType>(RelativePath);
		}
	}

	void ThreadLoadStart();
};

