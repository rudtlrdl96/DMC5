#pragma once

// 설명 :
class ContentsCore
{
public:
	// constrcuter destructer
	ContentsCore();
	~ContentsCore();

	// delete Function
	ContentsCore(const ContentsCore& _Other) = delete;
	ContentsCore(ContentsCore&& _Other) noexcept = delete;
	ContentsCore& operator=(const ContentsCore& _Other) = delete;
	ContentsCore& operator=(ContentsCore&& _Other) noexcept = delete;

	static void GameStart();
	static void GameEnd();
	static void ContentsResourcesCreate();
	static void CreateContentsKey();
	static void SetOption();

	//ContentsCore::GameEnd() 에서 수행될 콜백을 추가합니다
	static void PushGameEndCallBack(std::function<void()> _CallBack)
	{
		GameEndCallBacks.push_back(_CallBack);
	}

protected:

private:
	//ContentsCore::GameEnd() 에서 수행될 콜백들을 보관하고 있습니다
	static std::vector<std::function<void()>> GameEndCallBacks;

	static void FontUpRelease();
	static void FontUpload();

};

