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
	boidsAlive++; 
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
	//Go through boids of specific type
	for (it = typeList[type]->m_boids.begin(); it != typeList[type]->m_boids.end();)
	{
			//Erase boid from m_boids
			delete *it;
			it = typeList[type]->m_boids.erase(it);
			boidsAlive--; 
			cout << "Boid Destroyed of type ";
			cout << type; 
			cout << "\n"; 
			return true;
			it++;
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
	for (int i = 0; i < m_invObj.size(); ++i)
	{
		m_invObj[i]->SetPos(Vector3(xPos, xPos, xPos));
	}

	//apply my base behaviour
	AdjustBoidCounts();
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
	boidOfType currentBoid = typeList[type]->m_boids[thisBoid];
	// Loop through all boids
	for (int x = 0; x < typeList.size(); ++x)
	{
		//Get typelist
		for (int i = 0; i < typeList[x]->m_boids.size(); ++i)
		{
			boidOfType loopBoid = typeList[x]->m_boids[i];
		//If boid != thisBoid && boid is alive 
			if (loopBoid != currentBoid && loopBoid->getAlive() == true)
			{
				//Distance between boid and thisBoid
				float distance = Vector3::Distance(loopBoid->GetPos(), currentBoid->GetPos());
				//If distance is less than set seperation distance
				if (distance < (currentBoid->getSeperation() * 10))
				{
					//Apply seperation force  -= (boid pos - thisBoid pos)   
					seperationForce -= (loopBoid->GetPos() - currentBoid->GetPos());
					//If not of my flock
				if (loopBoid->getType() != currentBoid->getType())
				{
					//Amplify seperation force
					seperationForce += seperationForce*typeAvoidance;
				}
				}
				
			}
		}
	}
	return seperationForce;
}

Vector3 BoidManager::Alignment(int thisBoid, int type)
{
	Vector3 percievedVelocity = Vector3::Zero; 
	boidOfType currentBoid = typeList[type]->m_boids[thisBoid];
	//Loop through boids of type
	for (int i = 0; i < typeList[type]->m_boids.size(); ++i)
	{
		boidOfType loopBoid = typeList[type]->m_boids[i];
		//If boid isn't ThisBoid && if boid is alive
		if (i != thisBoid && loopBoid->getAlive() == true)
		{
			percievedVelocity = percievedVelocity + loopBoid->getVelocity();
		}
	}
		//If there is more than one boid in type
	if (typeList[type]->boidsOfType != 1)
	{
		//Divide accumilated velocity by number of boids of type
		percievedVelocity = percievedVelocity / (typeList[type]->boidsOfType - 1);
		return ((percievedVelocity - currentBoid->getVelocity()) * currentBoid->getAlignment());
	}
	//If there is only one boid there is no alignment force applied
	return Vector3::Zero;
}

Vector3 BoidManager::Cohesion(int thisBoid, int type)
{
	Vector3 centerOfMass = Vector3::Zero; 
	boidOfType currentBoid = typeList[type]->m_boids[thisBoid];
	for (int i = 0; i < typeList[type]->m_boids.size(); ++i)
	{
		boidOfType loopBoid = typeList[type]->m_boids[i];
		//Do not include this boid in center of mass calculation
		if (i != thisBoid && loopBoid->getAlive() == true)
		{
			centerOfMass += loopBoid->GetPos();
		}
	}
	//Stop Dividing by Zero
	if (typeList[type]->boidsOfType != 1)
	{
		centerOfMass = centerOfMass / (typeList[type]->boidsOfType - 1);
		//Move boids a percentage towards the center of the group
		return (centerOfMass - currentBoid->GetPos()) * currentBoid->getCohesion();
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
	// Loop through all boids
	for (int x = 0; x < typeList.size(); ++x)
	{
		//Get typelist
		for (int i = 0; i < typeList[x]->m_boids.size(); ++i)
		{
			if (typeList[x]->m_boids[i]->getAlive() == true)
			{
				typeList[x]->m_boids[i]->Draw(_DD);
			}
		}
	}
}

void BoidManager::initTweakBar()
{
	TwBar* p_myBar;
	p_myBar = TwGetBarByName("Boid Settings");
	string prefix = std::to_string(clanNum);
	BoidSettings* BS = new BoidSettings();
	BS->colour = Vector4(RandomNumber(), RandomNumber(), RandomNumber(), 1);
	TwAddVarRW(p_myBar, "Type Avoidance", TW_TYPE_FLOAT, &typeAvoidance, "min = 0    max = 1   step = 0.01");
	TwAddVarRW(p_myBar, ("Colour       " + prefix).c_str(),       TW_TYPE_COLOR4F,&BS->colour,              ("group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Num Boids    " + prefix).c_str(),       TW_TYPE_INT32,  &BS->requestedSpecialBoid,("min = 0    max = " + std::to_string(boidPool) + "   step = 1   group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Alignment    " + prefix).c_str(),       TW_TYPE_FLOAT,  &BS->alignmentForce,      ("min = 0    max = 1   step = 0.01   group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Cohesion     " + prefix).c_str(),       TW_TYPE_FLOAT,  &BS->percentCohesion,     ("min = 0    max = 1   step = 0.01   group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Seperation   " + prefix).c_str(),       TW_TYPE_FLOAT,  &BS->seperationDistance,  ("min = 0    max = 1   step = 0.01   group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Homing       " + prefix).c_str(),       TW_TYPE_FLOAT,  &BS->homingInstinct,      ("min = 0    max = 1   step = 0.001  group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Acceleration " + prefix).c_str(),       TW_TYPE_FLOAT,  &BS->maxAcc,              ("min = 0.01 max = 1   step = 0.01   group=Boids" + prefix).c_str()); 
	//TwAddVarRW(p_myBar, "xPos",         TW_TYPE_FLOAT, &xPos,               "min = 0    max = 100 step = 1      group=Object");
	/*TwAddVarRW(p_myBar, "NameOfMyVariable", TW_TYPE_FLOAT, &alignmentForce, "");
	TwAddVarRW(p_myBar, "NameOfMyVariable", TW_TYPE_FLOAT, &alignmentForce, "");
	TwAddVarRW(p_myBar, "NameOfMyVariable", TW_TYPE_FLOAT, &alignmentForce, "");*/
	clanNum++; 	
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

			while (typeList[i]->requestedSpecialBoid > typeList[i]->boidsOfType)
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
			while (typeList[i]->requestedSpecialBoid < typeList[i]->boidsOfType)
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
	//for (int x = 0; x < typeList.size(); ++x)
	//{
	//	if (x == 1 && typeList[x]->boidsOfType > 1)
	//	{
	//		system("cls");
	//		cout << typeList[x-1]->m_boids[x]->GetPos().x;
	//		cout << " ";
	//		cout << typeList[x-1]->m_boids[x]->GetPos().y;
	//		cout << " ";
	//		cout << typeList[x-1]->m_boids[x]->GetPos().z;
	//		cout << "\n";

	//	}
	//}
}
