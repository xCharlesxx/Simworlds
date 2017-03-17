#pragma once 
#include "CMOGO.h"
#include <vector>

class Boid;
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
	int boidsAlive = 0; 
	float percentCohesion = 1; 
	float seperationDistance = 1; 
	float homingInstinct = 5;
	//Larger = less force
	float alignmentForce = 8; 
	const int boidPool = 1000; 
	float maxAcc = 1; 
	int x = 0; 
	bool homing = true;
};