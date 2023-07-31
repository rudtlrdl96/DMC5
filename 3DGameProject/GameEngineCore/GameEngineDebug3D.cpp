#include "PrecompileHeader.h"
#include "GameEngineDebug3D.h"

#include "GameEngineRenderer.h"

namespace GameEngineDebug
{
	enum class DebugDrawType
	{
		Box,
		Sphere,
		Point
	};

	class DebugData
	{
	public:
		DebugDrawType Type;
		class GameEngineTransform* Trans;
		float4 Color;
	};

	GameEngineRenderUnit DebugRenderUnit;

	std::map<GameEngineCamera*, std::vector<DebugData>> DebugDrawDatas;

	void DrawBox(class GameEngineCamera* _Cam, class GameEngineTransform* _Trans, float4 Color)
	{
		if (DebugDrawDatas.end() == DebugDrawDatas.find(_Cam))
		{
			DebugDrawDatas[_Cam].reserve(1000);
		}

		DebugDrawDatas[_Cam].push_back({ DebugDrawType::Box, _Trans, Color });
	}

	void DrawSphere(class GameEngineCamera* _Cam, class GameEngineTransform* _Trans, float4 Color) 
	{
		if (DebugDrawDatas.end() == DebugDrawDatas.find(_Cam))
		{
			DebugDrawDatas[_Cam].reserve(1000);
		}

		DebugDrawDatas[_Cam].push_back({ DebugDrawType::Sphere, _Trans, Color });
	}

	void DebugRender(GameEngineCamera* _Camera, float _Delta)
	{
		if (nullptr == DebugRenderUnit.Pipe)
		{
			DebugRenderUnit.SetPipeLine("DebugMeshRender");
		}

		std::vector<DebugData>& Data = DebugDrawDatas[_Camera];

		for (size_t i = 0; i < Data.size(); i++)
		{
			DebugData& CurData = Data[i];

			DebugDrawType Type = CurData.Type;

			switch (Type)
			{
			case GameEngineDebug::DebugDrawType::Box:
				DebugRenderUnit.SetMesh("DebugBox");
				break;
			case GameEngineDebug::DebugDrawType::Sphere:
				DebugRenderUnit.SetMesh("DebugSphere");
				break;
			case GameEngineDebug::DebugDrawType::Point:
				break;
			default:
				break;
			}

			static TransformData DrawData;
			DrawData = CurData.Trans->GetTransDataRef();

			switch (Type)
			{
			case GameEngineDebug::DebugDrawType::Box:
				DebugRenderUnit.SetMesh("DebugBox");
				break;
			case GameEngineDebug::DebugDrawType::Sphere:
				DebugRenderUnit.SetMesh("DebugSphere");
				{
					float4 TempScale, TempRotation, TempPosition;
					DrawData.WorldMatrix.Decompose(TempScale, TempRotation, TempPosition);
					TempScale.y = TempScale.z = TempScale.x;

					float4x4 MatScale, MatRot, MatPos;
					MatScale.Scale(TempScale);
					MatRot = TempRotation.QuaternionToRotationMatrix();
					MatPos.Pos(TempPosition);
					DrawData.WorldMatrix = MatScale * MatRot * MatPos;
				}
				break;
			case GameEngineDebug::DebugDrawType::Point:
				break;
			default:
				break;
			}
			DrawData.SetViewAndProjection(_Camera->GetView(), _Camera->GetProjection());


			DebugRenderUnit.ShaderResHelper.SetConstantBufferLink("TransformData", DrawData);
			DebugRenderUnit.ShaderResHelper.SetConstantBufferLink("DebugColor", CurData.Color);

			DebugRenderUnit.Render(_Delta);
		}

		Data.clear();
	}
}