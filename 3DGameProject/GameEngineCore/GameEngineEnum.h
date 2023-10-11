#pragma once

enum class ShaderType
{
	None,
	Vertex,
	Geometry,
	Pixel,
	Compute,
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
	Slope = (1 << 3),           // 비탈길
	Obstacle = (1 << 4),		// 장애물
	Trigger = (1 << 5),	        // 땅 트리거
};