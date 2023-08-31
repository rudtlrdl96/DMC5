#pragma once
#include <GameEngineCore/EngineGrid.h>

class ContentGrid : public EngineGrid
{
public:
	// construtor destructor
	ContentGrid();
	~ContentGrid();

	// delete Function
	ContentGrid(const ContentGrid& _Other) = delete;
	ContentGrid(ContentGrid&& _Other) noexcept = delete;
	ContentGrid& operator=(const ContentGrid& _Other) = delete;
	ContentGrid& operator=(ContentGrid&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
	void Render(float _Delta) override;

private:
	std::shared_ptr<class PhysXBoxComponent> PhysComponent = nullptr;

};

