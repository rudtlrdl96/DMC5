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
	Player = (1 << 0),			// �÷��̾�
	Ground = (1 << 1),			// �ٴ�
	Wall = (1 << 2),            // ��
	Slope = (1 << 3),           // ��Ż��
	Obstacle = (1 << 4),		// ��ֹ�
	Trigger = (1 << 5),	        // �� Ʈ����
};