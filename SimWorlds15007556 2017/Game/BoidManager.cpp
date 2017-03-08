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
}

BoidManager::~BoidManager()
{
	//tidy up anything I've created
}

void BoidManager::SpawnBoid()
{
	Boid* boid = new Boid(m_pd3dDevice);
	m_boids.push_back(boid); 
}

void BoidManager::InitialiseBoidPos()
{
	//Draw boids 
	//UpdateBoidPos();
}

void BoidManager::UpdateBoidPos(DrawData* _DD, GameData* _GD)
{
	for (int i = 0; i < m_boids.size(); ++i)
	{
		m_boids[i]->Update(_GD);
		//m_boids[i]->Tick(_GD); 
	}
	//Debug print size of boid vector
	if (prevSize != m_boids.size())
	{
		cout << "Updating ";
		cout << m_boids.size();
		cout << " boids\n";
		prevSize = m_boids.size();
	}
}

void BoidManager::DrawBoids(DrawData* _DD)
{
	for (int i = 0; i < m_boids.size(); ++i)
	{
		m_boids[i]->Draw(_DD);
	}
}
void BoidManager::Tick(GameData* _GD, DrawData* _DD)
{
	UpdateBoidPos(_DD, _GD);
	DrawBoids(_DD); 
	//apply my base behaviour
}