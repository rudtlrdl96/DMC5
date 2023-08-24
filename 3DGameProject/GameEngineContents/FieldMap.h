#pragma once
#include <GameEngineCore/GameEngineActor.h>

class FieldMap : public GameEngineActor
{
public:
	// construtor destructor
	FieldMap();
	~FieldMap();

	// delete Function
	FieldMap(const FieldMap& _Other) = delete;
	FieldMap(FieldMap&& _Other) noexcept = delete;
	FieldMap& operator=(const FieldMap& _Other) = delete;
	FieldMap& operator=(FieldMap&& _Other) noexcept = delete;

	static std::shared_ptr<FieldMap> CreateFieldMap(GameEngineLevel* _Level, std::string_view _FBXName);

protected:
	void Start();
	void Update(float _DeltaTime);

private:
	std::shared_ptr<class GameEngineFBXRenderer> FieldMapRenderer = nullptr;
	std::vector<std::shared_ptr<class GameEngineCollision>> MapCols;
};

