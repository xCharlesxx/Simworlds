#include "BoidManager.h"
#include <dinput.h>
#include "GameData.h"
#include "Boid.h"
#include "DrawData.h"
#include <iostream>
#include <AntTweakBar.h>

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
	percentCohesion = percentCohesion / 100;
	homingInstinct = homingInstinct / 100; 
	initTweakBar(); 
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
			boidsAlive++;
			cout << "Updating ";
			cout << boidsAlive;
			cout << " boids\n";
			m_boids[i]->outputBoidPos(); 
			break; 
		}
	}
}

void BoidManager::ToggleHoming()
{
	if (homing == true)
	{
		homing = false;
		cout << "Homing = False\n"; 
	}
	else
	{
		homing = true;
		cout << "Homing = True\n";
	}
}

void BoidManager::InitialiseBoidPos()
{
	//Draw boids 
	//UpdateBoidPos();
}

void BoidManager::Tick(GameData* _GD, DrawData* _DD)
{
	UpdateBoidPos(_DD, _GD);
	//apply my base behaviour
}

void BoidManager::UpdateBoidPos(DrawData* _DD, GameData* _GD)
{
	Vector3 v1 = Vector3::Zero;
	Vector3 v2 = Vector3::Zero;
	Vector3 v3 = Vector3::Zero;
	Vector3 v4 = Vector3::Zero;
	for (int i = 0; i < m_boids.size(); ++i)
	{
		if (m_boids[i]->getAlive() == true)
		{
			v1 = Separation(i);
			v2 = Alignment(i);
			v3 = Cohesion(i);
			if (homing == true)
			v4 = Homing(i); 
			Vector3 velocity = v1 + v2 + v3 + v4;
			Vector3 acceleration = velocity * _GD->m_dt;
			acceleration = XMVector3ClampLength(acceleration, 0.0, maxAcc);
			m_boids[i]->Update(_GD, acceleration);
		}
	}
}

Vector3 BoidManager::Separation(int thisBoid)
{
	Vector3 seperationForce = Vector3::Zero; 
	for (int i = 0; i < m_boids.size(); ++i)
	{
		if (i != thisBoid && m_boids[i]->getAlive() == true)
		{
			float distance = Vector3::Distance(m_boids[i]->GetPos(), m_boids[thisBoid]->GetPos());
			if (distance < seperationDistance)
			{
				seperationForce = seperationForce - (m_boids[i]->GetPos() - m_boids[thisBoid]->GetPos());
			}
		}
	}
	return seperationForce;
}

Vector3 BoidManager::Alignment(int thisBoid)
{
	Vector3 percievedVelocity = Vector3::Zero; 
	for (int i = 0; i < m_boids.size(); ++i)
	{
		if (i != thisBoid && m_boids[i]->getAlive() == true)
		{
			percievedVelocity = percievedVelocity + m_boids[i]->getVelocity(); 
		}
	}
	if (boidsAlive != 1)
	{
		percievedVelocity = percievedVelocity / (boidsAlive - 1); 
		return ((percievedVelocity - m_boids[thisBoid]->getVelocity()) / alignmentForce);
	}
	return Vector3::Zero;
}

Vector3 BoidManager::Cohesion(int thisBoid)
{
	Vector3 centerOfMass = Vector3::Zero; 
	for (int i = 0; i < m_boids.size(); ++i)
	{
		//Do not include this boid in center of mass calculation
		if (i != thisBoid && m_boids[i]->getAlive() == true)
		{
			centerOfMass += m_boids[i]->GetPos();
		}
	}
	//Stop Dividing by Zero
	if (boidsAlive != 1)
	{
		centerOfMass = centerOfMass / (boidsAlive - 1);
		//Move boids a percentage towards the center of the group
		return (centerOfMass - m_boids[thisBoid]->GetPos()) * percentCohesion;
	}
	return Vector3::Zero; 
}

Vector3 BoidManager::Homing(int thisBoid)
{
	Vector3 home = Vector3::Zero;
	//Always passively head home
	return home - m_boids[thisBoid]->GetPos() * homingInstinct;
}

float BoidManager::getSeperation()
{
	return seperationDistance;
}

float BoidManager::getCohesion()
{
	return percentCohesion;
}

float BoidManager::getAlignment()
{
	return alignmentForce;
}

float BoidManager::getHoming()
{
	return homingInstinct;
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

void BoidManager::initTweakBar()
{
	TwBar* p_myBar;
	p_myBar = TwGetBarByName("Boid Settings");
	
	TwAddVarRW(p_myBar, "Alignment", TW_TYPE_FLOAT, &alignmentForce, "min = 0.1 max = 10 step = 0.1");
	TwAddVarRW(p_myBar, "Cohesion", TW_TYPE_FLOAT, &percentCohesion, "min = 0 max = 1 step = 0.01");
	TwAddVarRW(p_myBar, "Seperation", TW_TYPE_FLOAT, &seperationDistance, "min = 0 max = 10 step = 0.1");
	TwAddVarRW(p_myBar, "Homing", TW_TYPE_FLOAT, &homingInstinct, "min = 0 max = 1 step = 0.01");
	TwAddVarRW(p_myBar, "Acceleration", TW_TYPE_FLOAT, &maxAcc, "min = 0.01 max = 1 step = 0.01");
	/*TwAddVarRW(p_myBar, "NameOfMyVariable", TW_TYPE_FLOAT, &alignmentForce, "");
	TwAddVarRW(p_myBar, "NameOfMyVariable", TW_TYPE_FLOAT, &alignmentForce, "");
	TwAddVarRW(p_myBar, "NameOfMyVariable", TW_TYPE_FLOAT, &alignmentForce, "");
	TwAddVarRW(p_myBar, "NameOfMyVariable", TW_TYPE_FLOAT, &alignmentForce, "");
	TwAddVarRW(p_myBar, "NameOfMyVariable", TW_TYPE_FLOAT, &alignmentForce, "");*/

}