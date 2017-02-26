#include "BoidManager.h"
#include <dinput.h>
#include "GameData.h"
#include "Boid.h"

BoidManager::BoidManager(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	//m_fxFactory = new EffectFactory(_pd3dDevice);
}

BoidManager::~BoidManager()
{
	//tidy up anything I've created
}

void BoidManager::SpawnBoid()
{
	//Boid* boid = new Boid("BirdModelV1.cmo", _pd3dDevice, m_fxFactory);

}

void BoidManager::InitialiseBoidPos()
{
	//loop through all boids
	//Draw boids 
	UpdateBoidPos(); 
}

void BoidManager::UpdateBoidPos()
{

}

void BoidManager::Tick(GameData* _GD)
{
	
	//apply my base behaviour
	CMOGO::Tick(_GD);
}