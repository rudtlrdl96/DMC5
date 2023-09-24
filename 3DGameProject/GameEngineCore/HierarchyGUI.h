#pragma once
#include "GameEngineGUI.h"

class HierarchyGUI : public GameEngineGUIWindow
{
private:
	static bool CurFrameSetParent;

public:
	HierarchyGUI();
	~HierarchyGUI();

	HierarchyGUI(const HierarchyGUI& _Other) = delete;
	HierarchyGUI(HierarchyGUI&& _Other) noexcept = delete;
	HierarchyGUI& operator=(const HierarchyGUI& _Other) = delete;
	HierarchyGUI& operator=(HierarchyGUI&& _Other) noexcept = delete;

protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;

	void ObjectSelect(std::shared_ptr<GameEngineLevel> Level);
	void ShowObjectInfo();
	void ShowTransformInfo();

	void TransformControll(std::shared_ptr<GameEngineLevel> Level);
private:
	int ObjectIndex = -1;
	int ObjectSize = 0;
	GameEngineObject* SelectObject = nullptr;
};

