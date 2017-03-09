#include "BoidManager.h"
#include <dinput.h>
#include "GameData.h"
#include "Boid.h"
#include "DrawData.h"
#include <iostream>

BoidManager::BoidManager(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF)
{
	m_fxFactory = _EF; 
	m_pd3dDevice = _pd3dDevice; 
	m_fileName = _fileName; 
	m_boids.reserve(boidPool);
	for (int i = 0; i < boidPool; i++)
	{
		Boid* boid = new Boid(m_pd3dDevice);
		m_boids.push_back(boid);
	}
}

BoidManager::~BoidManager()
{
	//tidy up anything I've created
}

void BoidManager::SpawnBoid()
{
	for (int i = 0; i < boidPool; i++)
	{
		if (m_boids[i]->getAlive() == false)
		{
			m_boids[i]->setAlive(true);
			break; 
		}
	}
}

void BoidManager::InitialiseBoidPos()
{
	//Draw boids 
	//UpdateBoidPos();
}

void BoidManager::UpdateBoidPos(DrawData* _DD, GameData* _GD)
{
	boidsAlive = 0; 
	for (int i = 0; i < m_boids.size(); ++i)
	{
		if (m_boids[i]->getAlive() == true)
		{
			m_boids[i]->Update(_GD);
			boidsAlive++; 
		}
	}
	//Debug print size of boid vector
	if (prevSize != boidsAlive)
	{
		cout << "Updating ";
		cout << boidsAlive;
		cout << " boids\n";
		prevSize = boidsAlive;
	}
}

void BoidManager::DrawBoids(DrawData* _DD)
{
	for (int i = 0; i < m_boids.size(); ++i)
	{
		if (m_boids[i]->getAlive() == true)
		{
			m_boids[i]->Draw(_DD);
		}
	}
}
void BoidManager::Tick(GameData* _GD, DrawData* _DD)
{
	UpdateBoidPos(_DD, _GD);
	//apply my base behaviour
}