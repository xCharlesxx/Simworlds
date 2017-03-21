#pragma once 
#include "CMOGO.h"
#include <vector>

class Boid;
class InvisibleObject; 
struct GameData; 
struct DrawData; 

class BoidManager
{

public:
	BoidManager(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF);
	~BoidManager();

	void Tick(GameData* _GD, DrawData* _DD);
	void UpdateBoidPos(DrawData* _DD, GameData* _GD);
	void InitialiseBoidPos(); 
	void DrawBoids(DrawData* _DD);
	void SpawnBoid();
	void ToggleHoming(); 
	void initTweakBar(); 
	void RemoveBar(); 
	Vector3 Separation(int thisBoid);
	Vector3 Alignment(int thisBoid);
	Vector3 Cohesion(int thisBoid); 
	Vector3 Homing(int thisBoid); 
	float getSeperation(); 
	float getCohesion(); 
	float getAlignment(); 
	float getHoming(); 
protected:
	IEffectFactory* m_fxFactory;
	ID3D11Device* m_pd3dDevice;
	string m_fileName; 
	vector<Boid*> m_boids; 
	vector<InvisibleObject*> m_invObj; 
	int boidsAlive = 0; 
	int clanNum = 0; 
	float percentCohesion = 0.1; 
	float seperationDistance = 0.05; 
	float homingInstinct = 0.005;
	//Larger = less force
	float alignmentForce = 0.15; 
	const int boidPool = 1000; 
	float maxAcc = 0.1; 
	int x = 0; 
	float xPos = 0; 
	bool homing = true;
};