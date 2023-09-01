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
	Wall = (1 << 2),            // 벽
	Obstacle = (1 << 3),		// 장애물
	GroundTrigger = (1 << 4),	// 땅 트리거
};