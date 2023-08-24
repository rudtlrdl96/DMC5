#pragma once
#include <GameEngineCore/GameEngineActor.h>


// ���� :
class PickableActor : public GameEngineActor
{
public:

	/// CreateActor<PickableActor> �Ŀ� ������ ȣ���������

	// ���� �޽� ������ PickableActor ����
	void SetStaticMesh(const std::string& _FBX, const std::string& _Texture = "");

	// Collision�� �ִ� PickableActor ����, ����� �����ϰ� ���� ���� ȣ�� ����
	void SetCollisionOnly(const float4& _Scale);

	/// ~ CreateActor<PickableActor> �Ŀ� ������ ȣ���������

	// Axis ����
	void SetAxisMove(float4 _Color, float4 _Scale, float4 _MoveDir);
	void SetAxisRot(float4 _Color, float4 _Scale);


	bool IsAxis()
	{
		return IsAxis_;
	}

	float4 GetMoveDir()
	{
		return MoveDir_;
	}

public:
	static std::shared_ptr<GameEngineCollision> CurPicking_Collision;

	// constrcuter destructer
	PickableActor();
	~PickableActor();

	// delete Function
	PickableActor(const PickableActor& _Other) = delete;
	PickableActor(PickableActor&& _Other) noexcept = delete;
	PickableActor& operator=(const PickableActor& _Other) = delete;
	PickableActor& operator=(PickableActor&& _Other) noexcept = delete;

protected:
	/////////////////////////////////////////////////////////////////////
	//	���� ��ŷ
	/////////////////////////////////////////////////////////////////////

		// ��ŷ �������� ��Ҵ��� ���� üũ
	void CheckPickingRay();
	void CheckSelected();

	// �� ������ �ݸ���, ĳ���� �ݸ����� ������ �����ؾ���
	std::shared_ptr<GameEngineCollision> Collision_Picking_;
	std::shared_ptr<GameEngineFBXStaticRenderer> FBXRenderer_;

	std::shared_ptr<GameEngineTextureRenderer> CollisionRenderer_;
	float4 ResultColor_;

	void Start() override;
	void Update(float _DeltaTime) override;

private:
	// Axis ��
	bool IsAxis_;
	float4 MoveDir_;

	// �����޽� ���� �ݸ����� �����ϰ� �ʹ�(����, ���̺�����Ʈ, �� ����)


};

