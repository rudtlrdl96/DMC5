#pragma once
#include <GameEngineCore/GameEngineGUI.h>

// ���� : ������ ������ ImGUI�� ǥ���մϴ�
// Transform �� ������ ����ȭ�鿡�� ���鼭 ������ �� �ֽ��ϴ�
class ObjectWindow : public GameEngineGUIWindow
{
public:
	// constrcuter destructer
	ObjectWindow();
	~ObjectWindow();

	// delete Function
	ObjectWindow(const ObjectWindow& _Other) = delete;
	ObjectWindow(ObjectWindow&& _Other) noexcept = delete;
	ObjectWindow& operator=(const ObjectWindow& _Other) = delete;
	ObjectWindow& operator=(ObjectWindow&& _Other) noexcept = delete;

	// â�� ǥ���� GameEngineObject�� �߰��Ѵ�
	void AddObject(std::shared_ptr<GameEngineObject> _Object);

protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;

	void ObjectSelect();
	void ShowObjectInfo();
	void ShowTransformInfo();

private:
	std::shared_ptr<GameEngineObject> ObjectArray[100];
	const char* ObjectNames[100];
	int ObjectIndex = -1;
	int ObjectSize = 0;
	std::shared_ptr <GameEngineObject> SelectObject;
};

