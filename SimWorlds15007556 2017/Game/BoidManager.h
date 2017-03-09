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
	
protected:
	IEffectFactory* m_fxFactory;
	ID3D11Device* m_pd3dDevice;
	string m_fileName; 
	vector<Boid*> m_boids; 
	int prevSize = 0;
	int boidsAlive = 0; 
	const int boidPool = 1000; 
};