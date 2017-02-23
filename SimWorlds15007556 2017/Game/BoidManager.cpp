#include "BoidManager.h"
#include <dinput.h>
#include "GameData.h"

BoidManager::BoidManager(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF) : CMOGO(_fileName, _pd3dDevice, _EF)
{
}

BoidManager::~BoidManager()
{
	//tidy up anything I've created
}

void SpawnBoid();
{

}

void BoidManager::Tick(GameData* _GD)
{
	
	//apply my base behaviour
	CMOGO::Tick(_GD);
}