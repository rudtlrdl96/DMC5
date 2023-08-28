#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "BaseEnemyActor.h"


///	<param name="[Code] : ">�����Ϸ��� ������ ���� </param>
///	<param name="[SpawnLocalPos] : ">������ ���� ��ġ�� ���մϴ� EnemySpawnArea ������ ��ġ�� ������ �˴ϴ�. </param>
///	<param name="[SpawnLocalRotation] : ">������ ���� ȸ���� ���մϴ� EnemySpawnArea ������ ��ġ�� ������ �˴ϴ�. </param>
class EnemySpawnData
{
public:
	EnemyCode Code = EnemyCode::None;

	float4 SpawnLocalPos = float4::ZERO;
	float4 SpawnLocalRotation = float4::ZERO;

};

// ���� :
class EnemySpawnArea : public GameEngineActor, public GameEngineNetObject
{
public:
	// constrcuter destructer
	EnemySpawnArea();
	~EnemySpawnArea();

	// delete Function
	EnemySpawnArea(const EnemySpawnArea& _Other) = delete;
	EnemySpawnArea(EnemySpawnArea&& _Other) noexcept = delete;
	EnemySpawnArea& operator=(const EnemySpawnArea& _Other) = delete;
	EnemySpawnArea& operator=(EnemySpawnArea&& _Other) noexcept = delete;

	// ���̺꿡 ���� ���� �����͸� �߰��մϴ�
	void InsertSpawnData(int _WaveOrder, EnemySpawnData _EnemyData);

	// ���� ���̺��� ���͸� ��ȯ�մϴ� ���̺� ��ȯ�� ������ ��� true�� ��ȯ, �� �̻� ���̺� �����Ͱ� ���� ��� false�� ��ȯ�մϴ�
	bool PlayWave();

	// ���̺긦 �����մϴ� ��ȯ�� ���ʹ� ��� �����˴ϴ�
	void WaveReset();

	// ��� ���͸� �����մϴ� ��� ������ ������� �ʽ��ϴ�.
	void AllEnemyClear();

	// ��� ������ �������� ��ȯ�մϴ�.
	bool IsSpawnEnd();

	// ���� ��ȯ�� ���Ͱ� ��� �׾����� ��ȯ�մϴ�
	bool IsWaveEnd();

protected:

private:
	std::map<int, std::vector<EnemySpawnData>> WaveDatas;
	std::vector<std::shared_ptr<BaseEnemyActor>> SpawnEnemys;

	int CurWave = 0;
	int SpawnIndex = 0;
};

