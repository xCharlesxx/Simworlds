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
	//for (int i = 0; i < 1; i++)
	//{
		InvisibleObject* invObj = new InvisibleObject("Bust.cmo", m_pd3dDevice, m_fxFactory, Vector3::Zero, 0.0f, 4.7f, 0.0f, 0.25f * Vector3(100.0f));
		m_invObj.push_back(invObj);
	//}
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
	//	if (m_boids[i]->getType() == type)
	//	{			
	//		m_boids[i]->setAlive(false);
	//		boidsAlive--;
	//      cout << "Boid Destroyed of type ";
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

void BoidManager::Tick(GameData* _GD, DrawData* _DD)
{	
	//for (int i = 0; i < m_invObj.size(); ++i)
	//{
		m_invObj[0]->SetPos(Vector3(xPos, 0, 0));
	//}

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
	//Get typelist
	for (int x = 0; x < typeList.size(); ++x)
	{
		//GetBoidNum
		for (int i = 0; i < typeList[x]->m_boids.size(); ++i)
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

Vector3 BoidManager::Separation(int thisBoid, int type)
{
	Vector3 separationForce = Vector3::Zero;  
	boidOfType currentBoid = typeList[type]->m_boids[thisBoid];
	//If type 1: seperate from all boids
	if (type == 1 && typeSeparation != 0)
	{
		for (int x = 0; x < typeList.size(); ++x)
		{
				//Get typelist
			for (int i = 0; i < typeList[x]->m_boids.size(); ++i)
			{
				boidOfType loopBoid = typeList[x]->m_boids[i];
				//If boid != thisBoid && boid is alive 
				if (loopBoid != currentBoid)
				{
					//Distance between boid and thisBoid
					float distance = Vector3::Distance(loopBoid->GetPos(), currentBoid->GetPos());
					//If not of my flock
					if (loopBoid->getType() != currentBoid->getType())
					{
						//Use different separation
						if (distance < (typeSeparation * 50))
						{
							separationForce -= (loopBoid->GetPos() - currentBoid->GetPos());
						}
					}
					//else if distance is less than set separation distance
					else if (distance < (currentBoid->getSeparation() * 10))
					{
						//Apply separation force  -= (boid pos - thisBoid pos)   
						separationForce -= (loopBoid->GetPos() - currentBoid->GetPos());
					}
				}
			}
		}
		return separationForce;
	}
	// Type is not 1 or type separation is turned off
		if (typeList[type]->separationDistance != 0)
			for (int i = 0; i < typeList[type]->m_boids.size(); ++i)
			{
				boidOfType loopBoid = typeList[type]->m_boids[i];
				//If boid != thisBoid && boid is alive 
				if (loopBoid != currentBoid)
				{
					//Distance between boid and thisBoid
					float distance = Vector3::Distance(loopBoid->GetPos(), currentBoid->GetPos());
					//else if distance is less than set separation distance
					if (distance < (currentBoid->getSeparation() * 10))
					{
						//Apply separation force  -= (boid pos - thisBoid pos)   
						separationForce -= (loopBoid->GetPos() - currentBoid->GetPos());
					}
				}
			}
	return separationForce;
}

Vector3 BoidManager::Alignment(int thisBoid, int type)
{
	Vector3 percievedVelocity = Vector3::Zero; 
	//Performance enhancement
	if (typeList[type]->alignmentForce != 0)
	{
		boidOfType currentBoid = typeList[type]->m_boids[thisBoid];
		//Loop through boids of type
		for (int i = 0; i < typeList[type]->m_boids.size(); ++i)
		{
			boidOfType loopBoid = typeList[type]->m_boids[i];
			//If boid isn't ThisBoid && if boid is alive
			if (i != thisBoid)
			{
				percievedVelocity = percievedVelocity + loopBoid->getVelocity();
			}
		}
		//If there is more than one boid in type
		if (typeList[type]->m_boids.size() != 1)
		{
			//Divide accumilated velocity by number of boids of type
			percievedVelocity /= (typeList[type]->m_boids.size() - 1);
			return ((percievedVelocity - currentBoid->getVelocity()) * currentBoid->getAlignment());
		}
	}
	//If there is only one boid there is no alignment force applied
	return Vector3::Zero;
}

Vector3 BoidManager::Cohesion(int thisBoid, int type)
{
	Vector3 centerOfMass = Vector3::Zero; 
	boidOfType currentBoid = typeList[type]->m_boids[thisBoid];
	// Type 0 chasing type 1 
	if (typeCohesion != 0 && type == 0 && typeList.size() > 1)
	{
		// Make type 0 chase type 1
		for (int i = 0; i < typeList[1]->m_boids.size(); ++i)
		{
			boidOfType loopBoid = typeList[1]->m_boids[i];
			//Add all boids apart from this boid to center of mass
			centerOfMass += loopBoid->GetPos();
		}
		if (boidsAlive != 1)
		{
			//centerOfMass = centerOfMass / (typeList[1]->m_boids.size());
			centerOfMass /= (typeList[1]->m_boids.size());
			//Move boids a percentage towards the center of type1
			return (centerOfMass - currentBoid->GetPos()) * typeCohesion;
		}
	}
	//Performance improvement
	if (typeList[type]->percentCohesion != 0)
	{
		//loop thorough boid's own type
		for (int i = 0; i < typeList[type]->m_boids.size(); ++i)
		{
			boidOfType loopBoid = typeList[type]->m_boids[i];
			//Do not include this boid in center of mass calculation
			if (i != thisBoid)
			{
				centerOfMass += loopBoid->GetPos();
					//*(typeList[type]->percentCohesion));
			}
		}
		//Stop Dividing by Zero
		if (typeList[type]->m_boids.size() != 1)
		{
			centerOfMass /= (typeList[type]->m_boids.size() - 1);
			//Move boids a percentage towards the center of the group
			return (centerOfMass - currentBoid->GetPos()) * typeList[type]->percentCohesion;
		}
	}
	return Vector3::Zero; 
}

Vector3 BoidManager::Homing(int thisBoid, int type)
{
	Vector3 home = m_invObj[0]->GetPos(); 
	//Always passively head home
	return (home - typeList[type]->m_boids[thisBoid]->GetPos()) *  typeList[type]->m_boids[thisBoid]->getHoming();
}

void BoidManager::DrawBoids(DrawData* _DD)
{
	// Loop through all boids
	for (int x = 0; x < typeList.size(); ++x)
	{
		//Get typelist
		for (int i = 0; i < typeList[x]->m_boids.size(); ++i)
		{
			typeList[x]->m_boids[i]->Draw(_DD);
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
	TwAddVarRW(p_myBar, "Type separation", TW_TYPE_FLOAT, &typeSeparation, "min = 0    max = 1   step = 0.001");
	TwAddVarRW(p_myBar, "Type Cohesion", TW_TYPE_FLOAT, &typeCohesion, "min = 0    max = 1   step = 0.001");
	TwAddVarRW(p_myBar, "Home xPos    ", TW_TYPE_FLOAT, &xPos, "min = -100 max = 100   step = 10");
	TwAddVarRW(p_myBar, ("Colour       " + prefix).c_str(),       TW_TYPE_COLOR4F,&BS->colour,              ("group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Num Boids    " + prefix).c_str(),       TW_TYPE_INT32,  &BS->requestedSpecialBoid,("min = 0    max = " + std::to_string(boidPool) + "   step = 1   group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Alignment    " + prefix).c_str(),       TW_TYPE_FLOAT,  &BS->alignmentForce,      ("min = 0    max = 1   step = 0.01   group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Cohesion     " + prefix).c_str(),       TW_TYPE_FLOAT,  &BS->percentCohesion,     ("min = 0    max = 1   step = 0.01   group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Separation   " + prefix).c_str(),       TW_TYPE_FLOAT,  &BS->separationDistance,  ("min = 0    max = 1   step = 0.01   group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Homing       " + prefix).c_str(),       TW_TYPE_FLOAT,  &BS->homingInstinct,      ("min = 0    max = 1   step = 0.001  group=Boids" + prefix).c_str());
	TwAddVarRW(p_myBar, ("Acceleration " + prefix).c_str(),       TW_TYPE_FLOAT,  &BS->maxAcc,              ("min = 0.01 max = 1   step = 0.01   group=Boids" + prefix).c_str()); 

	clanNum++; 	
	BS->m_boids.reserve(boidPool);
	typeList.push_back(BS); 
}

float BoidManager::RandomNumber()
{
	//Produce a random number between 0.5 and 1 or 0
	//This regularly produces bright and vibrant colours 
	srand(rand());
	int num = rand() % 75;
	if (num > 50)
	{
		return 0.0f; 
	}
	float number = num / 100.0f; 
	number += 0.50f; 
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
		TwRemoveVar(p_myBar, ("Colour       " + prefix).c_str());
		TwRemoveVar(p_myBar, ("Num Boids    " + prefix).c_str());
		TwRemoveVar(p_myBar, ("Alignment    " + prefix).c_str());
		TwRemoveVar(p_myBar, ("Cohesion     " + prefix).c_str());
		TwRemoveVar(p_myBar, ("Separation   " + prefix).c_str());
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
		if (typeList[i]->requestedSpecialBoid > typeList[i]->m_boids.size())
		{
			//Check if boid pool is full
			if (typeList[i]->requestedSpecialBoid > boidPool)
				typeList[i]->requestedSpecialBoid = boidPool;

			while (typeList[i]->requestedSpecialBoid > typeList[i]->m_boids.size())
			{
				if (!SpawnBoid(i))
					break;
			}
		}
	}
	//De-Spawn Boids
	for (int i = 0; i < typeList.size(); ++i)
	{
		if (typeList[i]->requestedSpecialBoid < typeList[i]->m_boids.size())
		{
			while (typeList[i]->requestedSpecialBoid < typeList[i]->m_boids.size())
			{
				if (!KillBoid(i))
					break;
			}
		}
	}
}

void BoidManager::Presets()
{
	//Allow user to cycle through presets when space is pressed
	if (preset == 9)
		preset = 0;
	else
		preset++;
	int size = typeList.size(); 
	for (int i = 0; i < size; i++)
	{
		RemoveBar(); 
	}
	typeCohesion = 0;
	typeSeparation = 0; 
	homing = true; 
	switch (preset)
	{
		//Flocks
	case 0:
		for (int i = 0; i < 2; i++)
		{
			initTweakBar();
			typeList[i]->requestedSpecialBoid = 750;
			typeList[i]->colour = Vector4(1, i, 0, 1);
			typeList[i]->homingInstinct = 0.001; 
			typeSeparation = 0.01; 
		}
		break; 
		//Chasing leader boid
	case 1:
		typeCohesion = 0.02; 
		initTweakBar();
		typeList[0]->requestedSpecialBoid = 500;
		typeList[0]->colour = Vector4(1, 0, 0, 1);
		typeList[0]->homingInstinct = 0;
		typeList[0]->maxAcc = 0.24;
		typeList[0]->separationDistance = 0.2;
		initTweakBar();
		typeList[1]->requestedSpecialBoid = 1;
		typeList[1]->colour = Vector4(1, 1, 0, 1);
		typeList[1]->homingInstinct = 0.35;
		typeList[1]->alignmentForce = 0;
		typeList[1]->percentCohesion = 0;
		typeList[1]->separationDistance = 0;
		break;
		//Avoiding predator 
	case 2:
		typeSeparation = 0.6; 
		initTweakBar();
		typeList[0]->requestedSpecialBoid = 1;
		typeList[0]->colour = Vector4(1, 0, 0, 1);
		typeList[0]->homingInstinct = 1;
		typeList[0]->alignmentForce = 0;
		typeList[0]->percentCohesion = 0;
		typeList[0]->separationDistance = 0;
		initTweakBar();
		typeList[1]->requestedSpecialBoid = 500;
		typeList[1]->colour = Vector4(1, 1, 0, 1);
		typeList[1]->homingInstinct = 0.4;
		typeList[1]->maxAcc = 1;
		typeList[1]->alignmentForce = 0.1;
		typeList[1]->percentCohesion = 0.2;
		typeList[1]->separationDistance = 0.17;
		break;
		//Classic Boids
	case 3:
		initTweakBar();
		typeList[0]->requestedSpecialBoid = 800;
		typeList[0]->colour = Vector4(1, 1, 1, 1);
		typeList[0]->homingInstinct = 0.001;
		typeList[0]->alignmentForce = 0.12;
		typeList[0]->percentCohesion = 0.01;
		typeList[0]->separationDistance = 0.1;
		break;
		//Swarms
	case 4:
		initTweakBar(); 
		for (int i = 0; i < 2; i++)
		{
			initTweakBar();
			typeList[i]->requestedSpecialBoid = 250 - (i*-100);
			typeList[i]->colour = Vector4(1 - (i * 1), 1 - (i * 1), 0, 1);
		}
		break;
	case 5:
		break;
	case 6:
		break;
		//Particle simulation 1
	case 7:
		for (int i = 0; i < 3; i++)
		{
			initTweakBar();
			typeList[i]->requestedSpecialBoid = 1000;
			typeList[i]->colour = Vector4(1 - i, i, -1 + i, 1);
			typeList[i]->homingInstinct = 0.3 + (i*0.3);
			typeList[i]->maxAcc = 1;
			typeList[i]->alignmentForce = 0;
			typeList[i]->percentCohesion = 0;
			typeList[i]->separationDistance = 0;
		}
		break;
		//Particle simulation 2
	case 8:
		for (int i = 0; i < 6; i++)
		{
			initTweakBar();
			typeList[i]->requestedSpecialBoid = 1000;
			typeList[i]->colour = Vector4(1 - i, i, -1 + i, 1);
			typeList[i]->homingInstinct = 0.3 + (i*0.3);
			typeList[i]->maxAcc = 0.3 + (i*0.3);
			typeList[i]->alignmentForce = 0;
			typeList[i]->percentCohesion = 0;
			typeList[i]->separationDistance = 0;
		}
		break;
		//Max number of boids particle simulation
	case 9:
		for (int i = 0; i < 50; i++)
		{
			initTweakBar();
			typeList[i]->requestedSpecialBoid = 1000;
			typeList[i]->colour = Vector4(RandomNumber(), RandomNumber(), RandomNumber(), 1);
			typeList[i]->homingInstinct = 0.1 + (i*0.02);
			typeList[i]->maxAcc = 1;
			typeList[i]->alignmentForce = 0;
			typeList[i]->percentCohesion = 0;
			typeList[i]->separationDistance = 0;
		}
		break;
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
		cout << "xPos ";
		cout << xPos;
		cout << "\n";
		cout << "Boids alive ";
		cout << boidsAlive;
		cout << "\nZero Boids ";
		cout << typeList[0]->m_boids.size(); 
		cout << "\nOne Boids ";
		if (typeList.size() > 1)
		cout << typeList[1]->m_boids.size();
		cout << "\nTwo Boids ";
		if (typeList.size() > 2)
		cout << typeList[2]->m_boids.size();
		cout << "\n";
		once = true;
	}
	//for (int x = 0; x < typeList.size(); ++x)
	//{
	//	if (x == 1 && typeList[x]->m_boids.size() > 1)
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
