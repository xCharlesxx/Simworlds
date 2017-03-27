#pragma once 
#include "CMOGO.h"
#include <vector>

class Boid;
class InvisibleObject; 
struct GameData; 
struct DrawData; 
struct BoidSettings;

class BoidManager
{

public:
	BoidManager(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF);
	~BoidManager();

	void Tick(GameData* _GD, DrawData* _DD);
	void UpdateBoidPos(DrawData* _DD, GameData* _GD);
	void InitialiseBoidPos(); 
	void DrawBoids(DrawData* _DD);
	bool SpawnBoid(int type);
	bool KillBoid(int type); 
	void ToggleHoming(); 
	void initTweakBar(); 
	void RemoveBar(); 
	void AdjustBoidCounts(); 
	float RandomNumber(); 
	Vector3 Separation(int thisBoid, int type);
	Vector3 Alignment(int thisBoid, int type);
	Vector3 Cohesion(int thisBoid, int type);
	Vector3 Homing(int thisBoid, int type);
	void DebugPrint(); 
	bool once = false;
protected:
	IEffectFactory* m_fxFactory;
	ID3D11Device* m_pd3dDevice;
	string m_fileName; 
	vector<InvisibleObject*> m_invObj; 
	vector<BoidSettings*> typeList;
	vector<Boid*> allBoids;
	int boidsAlive = 0; 
	int clanNum = 0; 
	const int boidPool = 1000; 
	int x = 0; 
	float xPos = 0; 
	bool homing = true;
	float percentCohesion = 0.1;
	float seperationDistance = 0.05;
	float homingInstinct = 0.005;
	//Larger = less force
	float alignmentForce = 0.15;
	float maxAcc = 0.1;

};