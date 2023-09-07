#pragma once
#include "BaseStageActor.h"

enum class MapColType
{

};

class MapCollisionMesh : public BaseStageActor
{
public:
	// construtor destructor
	MapCollisionMesh();
	~MapCollisionMesh();

	// delete Function
	MapCollisionMesh(const MapCollisionMesh& _Other) = delete;
	MapCollisionMesh(MapCollisionMesh&& _Other) noexcept = delete;
	MapCollisionMesh& operator=(const MapCollisionMesh& _Other) = delete;
	MapCollisionMesh& operator=(MapCollisionMesh&& _Other) noexcept = delete;

	static std::shared_ptr<MapCollisionMesh> CreateGroundCollisionMesh(GameEngineLevel* _Level, const std::string_view& _FBXName);
	static std::shared_ptr<MapCollisionMesh> CreateWallCollisionMesh(GameEngineLevel* _Level, const std::string_view& _FBXName);

	void RenderOn();
	void RenderOff();
	bool RenderIsUpdate();

protected:

private:
	static std::shared_ptr<MapCollisionMesh> CreateMapCollisionMesh(GameEngineLevel* _Level, const std::string_view& _FBXName, bool _IsGround);
	
	std::shared_ptr<class GameEngineFBXRenderer> Renderer = nullptr;
	std::shared_ptr<class PhysXTriangleComponent> Collision = nullptr;
};

