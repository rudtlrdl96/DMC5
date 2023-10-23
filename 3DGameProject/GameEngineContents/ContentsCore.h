#pragma once

// ���� :
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
	static void EffectTextureLoad();
	static void SetOption();

	//ContentsCore::GameEnd() ���� ����� �ݹ��� �߰��մϴ�
	static void PushGameEndCallBack(std::function<void()> _CallBack)
	{
		GameEndCallBacks.push_back(_CallBack);
	}

protected:

private:
	//ContentsCore::GameEnd() ���� ����� �ݹ���� �����ϰ� �ֽ��ϴ�
	static std::vector<std::function<void()>> GameEndCallBacks;

	static void FontUpRelease();
	static void FontUpload();
};

