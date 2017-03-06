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
	boids.reserve(boidPool); 

}

BoidManager::~BoidManager()
{
	//tidy up anything I've created
}

void BoidManager::SpawnBoid()
{
	Boid* boid = new Boid(m_fileName, m_pd3dDevice, m_fxFactory);
	boids.push_back(boid); 
}

void BoidManager::InitialiseBoidPos()
{
	//Draw boids 
	//UpdateBoidPos();
}

void BoidManager::UpdateBoidPos(DrawData* _DD)
{
	string output = ""; 
	for (int i = 0; i < boids.size(); ++i)
	{
		boids[i]->Update(); 
		boids[i]->Draw(_DD); 
		cout << "Update and Drawing boid ";
		cout << i;
		cout << "\n";
	}
	output = "Updated and Drawn ";
	output += boids.size();
	output += " boids\n";
	if (prevSize != boids.size())
	{
		cout << output;
		prevSize = boids.size();
	}
}

void BoidManager::DrawBoids(DrawData* _DD)
{
	for (int i = 0; i < boids.size(); ++i)
	{
		boids[i]->Draw(_DD);
	}
}
void BoidManager::Tick(GameData* _GD, DrawData* _DD)
{
	UpdateBoidPos(_DD); 
	//DrawBoids(_DD); 
	//apply my base behaviour
}