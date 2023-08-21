#pragma once
#include <GameEngineCore/GameEngineGUI.h>

// 설명 : 엑터의 정보를 ImGUI로 표시합니다
// Transform 값 수정을 게임화면에서 보면서 수정할 수 있습니다
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

	// 창에 표시할 GameEngineObject를 추가한다
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

