#pragma once

enum class ShaderType
{
	None,
	Vertex,
	Pixel,
};

enum class CameraType
{
	None,
	Perspective,
	Orthogonal,
};

enum class PhysXFilterGroup
{
	Player = (1 << 0),			// 플레이어
	Ground = (1 << 1),			// 바닥
	Obstacle = (1 << 2),		// 충돌체(HitCallback을 호출할 용도)
	PlayerFace = (1 << 3),		// 충돌체(HitCallback을 호출할 용도)
	PlayerDynamic = (1 << 4),	// 플레이어 다이나믹의 필터
	PlayerHead = (1 << 5),		// 플레이어 머리 다이나믹의 필터
	GroundTrigger = (1 << 6),	// 땅 트리거
};