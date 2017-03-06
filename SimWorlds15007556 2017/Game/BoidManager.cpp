#include "BoidManager.h"
#include <dinput.h>
#include "GameData.h"
#include "Boid.h"

BoidManager::BoidManager(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF)
{
	m_fxFactory = _EF; 
	m_pd3dDevice = _pd3dDevice; 
	m_fileName = _fileName; 
}

BoidManager::~BoidManager()
{
	//tidy up anything I've created
}

void BoidManager::SpawnBoid()
{
	Boid* boid = new Boid(m_fileName, m_pd3dDevice, m_fxFactory);
}

//void BoidManager::Draw(DrawData* _DD)
//{
//
//}

void BoidManager::InitialiseBoidPos()
{
	//Draw boids 
	UpdateBoidPos();
}

void BoidManager::UpdateBoidPos()
{
	for (std::vector<boid>::iterator it = boids.begin(); it != boids.end(); ++it)
	{

	}
}

void BoidManager::Tick(GameData* _GD)
{
	UpdateBoidPos(); 
	//apply my base behaviour
}