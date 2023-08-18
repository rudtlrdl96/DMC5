#include "PrecompileHeader.h"
#include "EnemySpawnArea.h"

EnemySpawnArea::EnemySpawnArea() 
{
}

EnemySpawnArea::~EnemySpawnArea() 
{
}

bool EnemySpawnArea::IsSpawnEnd()
{
	if (false == WaveDatas.contains(CurWave))
	{
		return true;
	}

	return true;
}

bool EnemySpawnArea::IsWaveEnd()
{
	return false;
}

void EnemySpawnArea::InsertSpawnData(int _WaveOrder, EnemySpawnData _EnemyData)
{
}

bool EnemySpawnArea::PlayWave()
{
	return false;
}

void EnemySpawnArea::WaveReset()
{
}

void EnemySpawnArea::AllEnemyClear()
{
}
