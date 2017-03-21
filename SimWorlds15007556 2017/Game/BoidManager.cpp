#include "BoidManager.h"
#include <dinput.h>
#include "GameData.h"
#include "Boid.h"
#include "InvisibleObject.h"
#include "DrawData.h"
#include <iostream>
#include <AntTweakBar.h>
#include "BoidSettings.h"

BoidManager::BoidManager(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF)
{
	m_fxFactory = _EF; 
	m_pd3dDevice = _pd3dDevice; 
	m_fileName = _fileName; 
	m_boids.reserve(boidPool);
	initTweakBar();
	for (int i = 0; i < boidPool; i++)
	{
		Boid* boid = new Boid(m_pd3dDevice, typeList);
		m_boids.push_back(boid);
	}
	for (int i = 0; i < 1; i++)
	{
		InvisibleObject* invObj = new InvisibleObject("Bust.cmo", m_pd3dDevice, m_fxFactory, Vector3::Zero, 0.0f, 4.7f, 0.0f, 0.25f * Vector3(100.0f));
		m_invObj.push_back(invObj);
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
			boidsAlive++;
			cout << "Updating ";
			cout << boidsAlive;
			cout << " boids\n";
			m_boids[i]->outputBoidPos(); 
			break; 
		}
	}
}

void BoidManager::KillBoid()
{
	for (int i = 0; i < boidPool; i++)
	{
		if (m_boids[i]->getAlive() == true)
		{
			m_boids[i]->setAlive(false);
			boidsAlive--;
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
	for (int i = 0; i < m_invObj.size(); ++i)
	{
		//set special boids to request boids
	}
	if (requestBoids > boidsAlive)
	{
		SpawnBoid();
	}
	else if (requestBoids < boidsAlive)
	{
		KillBoid();
	}
	for (int i = 0; i < m_invObj.size(); ++i)
	{
		m_invObj[i]->SetPos(Vector3(xPos, xPos, xPos));
	}
	//apply my base behaviour
	UpdateBoidPos(_DD, _GD);
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
			m_boids[i]->setBoid(seperationDistance, percentCohesion, alignmentForce, homingInstinct, maxAcc);
			v1 = Separation(i);
			v2 = Alignment(i);
			v3 = Cohesion(i);
			if (homing == true)
			v4 = Homing(i); 
			Vector3 velocity = v1 + v2 + v3 + v4;
			Vector3 acceleration = velocity * _GD->m_dt;
			acceleration = XMVector3ClampLength(acceleration, 0.0, m_boids[i]->getAcc());
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
			if (distance < (m_boids[i]->getSeperation() * 10))
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
		return ((percievedVelocity - m_boids[thisBoid]->getVelocity()) * m_boids[thisBoid]->getAlignment());
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
		return (centerOfMass - m_boids[thisBoid]->GetPos()) *  m_boids[thisBoid]->getCohesion();
	}
	return Vector3::Zero; 
}

Vector3 BoidManager::Homing(int thisBoid)
{
	Vector3 home = m_invObj[0]->GetPos(); 
	//Always passively head home
	return home - m_boids[thisBoid]->GetPos() *  m_boids[thisBoid]->getHoming();
}

void BoidManager::DrawBoids(DrawData* _DD)
{
	for (int i = 0; i < m_invObj.size(); ++i)
	{
		m_invObj[i]->Draw(_DD);
	}
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
	string prefix = std::to_string(clanNum);
	BoidSettings* BS = new BoidSettings();
	TwAddVarRW(p_myBar, ("Num Boids    " + prefix).c_str(),       TW_TYPE_INT32, &BS->requestedSpecialBoid,("min = 0    max = "+std::to_string(boidPool)+"   step = 1   group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Alignment    " + prefix).c_str(),       TW_TYPE_FLOAT, &BS->alignmentForce,     ("min = 0    max = 1   step = 0.01   group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Cohesion     " + prefix).c_str(),       TW_TYPE_FLOAT, &BS->percentCohesion,    ("min = 0    max = 1   step = 0.01   group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Seperation   " + prefix).c_str(),       TW_TYPE_FLOAT, &BS->seperationDistance, ("min = 0    max = 1   step = 0.01   group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Homing       " + prefix).c_str(),       TW_TYPE_FLOAT, &BS->homingInstinct,     ("min = 0    max = 1   step = 0.001  group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Acceleration " + prefix).c_str(),       TW_TYPE_FLOAT, &BS->maxAcc,             ("min = 0.01 max = 1   step = 0.01   group=Boids" + prefix).c_str()); 
	//TwAddVarRW(p_myBar, "xPos",         TW_TYPE_FLOAT, &xPos,               "min = 0    max = 100 step = 1      group=Object");
	/*TwAddVarRW(p_myBar, "NameOfMyVariable", TW_TYPE_FLOAT, &alignmentForce, "");
	TwAddVarRW(p_myBar, "NameOfMyVariable", TW_TYPE_FLOAT, &alignmentForce, "");
	TwAddVarRW(p_myBar, "NameOfMyVariable", TW_TYPE_FLOAT, &alignmentForce, "");
	TwAddVarRW(p_myBar, "NameOfMyVariable", TW_TYPE_FLOAT, &alignmentForce, "");*/
	clanNum++; 
	typeList.push_back(BS); 
}

void BoidManager::RemoveBar()
{
	if (clanNum != 0)
	{
		clanNum--;
		TwBar* p_myBar;
		p_myBar = TwGetBarByName("Boid Settings");
		string prefix = std::to_string(clanNum);
		TwRemoveVar(p_myBar, ("Num Boids    " + prefix).c_str());
		TwRemoveVar(p_myBar, ("Alignment    " + prefix).c_str());
		TwRemoveVar(p_myBar, ("Cohesion     " + prefix).c_str());
		TwRemoveVar(p_myBar, ("Seperation   " + prefix).c_str());
		TwRemoveVar(p_myBar, ("Homing       " + prefix).c_str());
		TwRemoveVar(p_myBar, ("Acceleration " + prefix).c_str());
		typeList.pop_back(); 
	}
}