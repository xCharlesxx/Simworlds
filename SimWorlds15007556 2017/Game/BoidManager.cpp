#include "BoidManager.h"
#include <dinput.h>
#include "GameData.h"
#include "Boid.h"
#include "InvisibleObject.h"
#include "DrawData.h"
#include <iostream>
#include <AntTweakBar.h>
#include "BoidSettings.h"
#include <vector> 
#include <cstdlib>
#include <ctime>

BoidManager::BoidManager(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF)
{
	m_fxFactory = _EF; 
	m_pd3dDevice = _pd3dDevice; 
	m_fileName = _fileName; 
	initTweakBar();
	allBoids.reserve(10000); 
	/*for (int i = 0; i < boidPool; i++)
	{
		Boid* boid = new Boid(m_pd3dDevice, typeList);
		m_boids.push_back(boid);
	}*/
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

bool BoidManager::SpawnBoid(int type)
{
	Boid* boid = new Boid(m_pd3dDevice, type, typeList);
	typeList[type]->m_boids.push_back(boid);
	allBoids.push_back(boid); 
	boidsAlive++; 
	typeList[type]->boidsOfType++; 
	cout << "Boid Created of type ";
	cout << type;
	cout << "\n";
	return true; 
		/*if (m_boids[i]->getAlive() == false)
		{
			m_boids[i]->setAlive(true);  
			m_boids[i]->setType(type);
			boidsAlive++;
			cout << "Updating ";
			cout << boidsAlive;
			cout << " boids\n";
			cout << "Of Type ";
			cout << type; 
			cout << "\n"; 
			m_boids[i]->outputBoidPos(); 
			return true; 
		}*/
	//}
	return false; 
}

bool BoidManager::KillBoid(int type)
{
	vector<Boid*>::iterator it;
	vector<Boid*>::iterator itr;
	//Go through boids of specific type
	for (it = typeList[type]->m_boids.begin(); it != typeList[type]->m_boids.end();)
	{
		//Check if this is needed anymore
		if ((*it)->getType() == type)
		{
			//Erase boid from m_boids
			delete *it;
			it = typeList[type]->m_boids.erase(it);
			for (itr = allBoids.begin(); itr != allBoids.end();)
			{
				if ((*itr) == (*it))
				{
					//Erase boid pointer from allBoids
					delete *itr; 
					itr = allBoids.erase(itr); 
				}
				itr++; 
			}
			typeList[type]->boidsOfType--;
			boidsAlive--; 
			cout << "Boid Destroyed of type ";
			cout << type; 
			cout << "\n"; 
			return true;
		}
		++it;
	}
	//for (int i = 0; i < boidPool; i++)
	//{
	//	//m_boids.erase(m_boids.begin(), m_boids.end(), type), m_boids.end());
	//		
	//	//m_boids[i]->getAlive() == true &&
	//	if (m_boids[i]->getType() == type)
	//	{			
	//		m_boids[i]->setAlive(false);
	//		boidsAlive--;
	//		cout << "Updating ";
	//		cout << boidsAlive;
	//		cout << " boids\n";
	//		cout << "Removed Of Type ";
	//		cout << type;
	//		cout << "\n";
	//		//m_boids[i]->outputBoidPos();
	//		return true;
	//	}
	//}
	return false; 
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
	//Something wrong here, calling multiple times in one cycle
	
	AdjustBoidCounts(); 
	//updateBoidSettings(); 

	for (int i = 0; i < m_invObj.size(); ++i)
	{
		m_invObj[i]->SetPos(Vector3(xPos, xPos, xPos));
	}
	//apply my base behaviour
	UpdateBoidPos(_DD, _GD);
	DebugPrint(); 
}

void BoidManager::UpdateBoidPos(DrawData* _DD, GameData* _GD)
{
	Vector3 v1 = Vector3::Zero;
	Vector3 v2 = Vector3::Zero;
	Vector3 v3 = Vector3::Zero;
	Vector3 v4 = Vector3::Zero;
	for (int x = 0; x < typeList.size(); ++x)
	{
		//Get typelist
		for (int i = 0; i < typeList[x]->m_boids.size(); ++i)
		{
			//GetBoidNum
			if (typeList[x]->m_boids[i]->getAlive() == true)
			{
				v1 = Separation(i, x);
				v2 = Alignment(i, x);
				v3 = Cohesion(i, x);
				if (homing == true)
					v4 = Homing(i, x);
				Vector3 velocity = v1 + v2 + v3 + v4;
				Vector3 acceleration = velocity * _GD->m_dt;
				acceleration = XMVector3ClampLength(acceleration, 0.0, typeList[x]->m_boids[i]->getAcc());
				typeList[x]->m_boids[i]->Update(_GD, acceleration);
			}
		}
	}
}

Vector3 BoidManager::Separation(int thisBoid, int type)
{
	Vector3 seperationForce = Vector3::Zero; 
	// Loop through boids
	for (int i = 0; i < allBoids.size(); ++i)
	{
		//If boid != thisBoid && boid is alive 
		if (allBoids[i] != typeList[type]->m_boids[thisBoid] && allBoids[i]->getAlive() == true )
		{
			//Distance between boid and thisBoid
			float distance = Vector3::Distance(allBoids[i]->GetPos(), typeList[type]->m_boids[thisBoid]->GetPos());
			//If distance is less than set seperation distance
			if (distance < (typeList[type]->m_boids[i]->getSeperation() * 10))
			{
				//Apply seperation force  -= (boid pos - thisBoid pos)   
				seperationForce -= (allBoids[i]->GetPos() - typeList[type]->m_boids[thisBoid]->GetPos());
			}
			//If not of my flock
			if (allBoids[i]->getType() != typeList[type]->m_boids[thisBoid]->getType())
			{
				//Amplify seperation force
				seperationForce += seperationForce*0.5; 
			}
		}
	}
	return seperationForce;
}

Vector3 BoidManager::Alignment(int thisBoid, int type)
{
	Vector3 percievedVelocity = Vector3::Zero; 
	for (int i = 0; i < typeList[type]->m_boids.size(); ++i)
	{
		//If boid isn't ThisBoid && if boid is alive && if boid is in the same clan
		if (allBoids[i] != typeList[type]->m_boids[thisBoid] && typeList[type]->m_boids[i]->getAlive() == true && typeList[type]->m_boids[i]->getType() == typeList[type]->m_boids[thisBoid]->getType())
		{
			percievedVelocity = percievedVelocity + typeList[type]->m_boids[i]->getVelocity();
		}
	}
	if (typeList[type]->boidsOfType != 1)
	{
		percievedVelocity = percievedVelocity / (typeList[type]->boidsOfType - 1);
		return ((percievedVelocity - typeList[type]->m_boids[thisBoid]->getVelocity()) * typeList[type]->m_boids[thisBoid]->getAlignment());
	}
	return Vector3::Zero;
}

Vector3 BoidManager::Cohesion(int thisBoid, int type)
{
	Vector3 centerOfMass = Vector3::Zero; 
	for (int i = 0; i < typeList[type]->m_boids.size(); ++i)
	{
		//Do not include this boid in center of mass calculation
		if (i != thisBoid && typeList[type]->m_boids[i]->getAlive() == true && typeList[type]->m_boids[thisBoid]->getType() == typeList[type]->m_boids[i]->getType())
		{
			centerOfMass += typeList[type]->m_boids[i]->GetPos();
		}
	}
	//Stop Dividing by Zero
	if (typeList[type]->boidsOfType != 1)
	{
		centerOfMass = centerOfMass / (typeList[type]->boidsOfType - 1);
		//Move boids a percentage towards the center of the group
		return (centerOfMass - typeList[type]->m_boids[thisBoid]->GetPos()) *  typeList[type]->m_boids[thisBoid]->getCohesion();
	}
	return Vector3::Zero; 
}

Vector3 BoidManager::Homing(int thisBoid, int type)
{
	Vector3 home = m_invObj[0]->GetPos(); 
	//Always passively head home
	return home - typeList[type]->m_boids[thisBoid]->GetPos() *  typeList[type]->m_boids[thisBoid]->getHoming();
}

void BoidManager::DrawBoids(DrawData* _DD)
{
	for (int i = 0; i < m_invObj.size(); ++i)
	{
		//m_invObj[i]->Draw(_DD);
	}
	for (int i = 0; i < allBoids.size(); ++i)
	{
		if (allBoids[i]->getAlive() == true)
		{
			allBoids[i]->Draw(_DD);
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
	BS->colour = Vector4(RandomNumber(), RandomNumber(), RandomNumber(), 1);
	BS->m_boids.reserve(boidPool);
	typeList.push_back(BS); 
}

float BoidManager::RandomNumber()
{
	srand(rand());
	int num = rand() % 100;
	float number = num / 100.0f; 
	return number; 
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
		for (int i = 0; i < boidPool; i++)
		{
			KillBoid(clanNum); 
		}
	}
}

void BoidManager::AdjustBoidCounts()
{
	//Spawn Boids
	for (int i = 0; i < typeList.size(); ++i)
	{
		//If Boids are Requested
		if (typeList[i]->requestedSpecialBoid > typeList[i]->boidsOfType)
		{
			//Check if boid pool is full
			if (typeList[i]->requestedSpecialBoid > boidPool)
				typeList[i]->requestedSpecialBoid = boidPool;

			while (typeList[i]->requestedSpecialBoid > boidsAlive)
			{
				if (!SpawnBoid(i))
					break;
				typeList[i]->boidsOfType++; 
			}
		}
	}
	//De-Spawn Boids
	for (int i = 0; i < typeList.size(); ++i)
	{
		if (typeList[i]->requestedSpecialBoid < typeList[i]->boidsOfType)
		{
			while (typeList[i]->requestedSpecialBoid < boidsAlive)
			{
				if (!KillBoid(i))
					break;
				typeList[i]->boidsOfType--;
			}
		}
	}
}

void BoidManager::DebugPrint()
{
	if (once == false)
	{
		for (int i = 0; i < typeList.size(); ++i)
		{
			cout << "Type ";
			cout << i;
			cout << " ";
			cout << typeList[i]->colour.x;
			cout << " ";
			cout << typeList[i]->colour.y;
			cout << " ";
			cout << typeList[i]->colour.z;
			cout << "\n";
		}
		cout << "Boids alive ";
		cout << boidsAlive;
		cout << "\nZero Boids ";
		cout << typeList[0]->boidsOfType; 
		cout << "\nOne Boids ";
		if (typeList.size() > 1)
		cout << typeList[1]->boidsOfType;
		cout << "\nTwo Boids ";
		if (typeList.size() > 2)
		cout << typeList[2]->boidsOfType;
		cout << "\n";
		once = true;
	}
}
