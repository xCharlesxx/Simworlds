#pragma once 
#include "CMOGO.h"
#include <vector>
//=================================================================
//Base Player Class (i.e. a GO the player controls)
//=================================================================
class boid;
class BoidManager
{

public:
	BoidManager(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF);
	~BoidManager();

	void Tick(GameData* _GD);
	void UpdateBoidPos();
	void InitialiseBoidPos(); 
	void SpawnBoid(); 
	//virtual void Draw(DrawData* _DD) override;
protected:
	IEffectFactory* m_fxFactory;
	ID3D11Device* m_pd3dDevice;
	string m_fileName; 
	vector<boid> boids; 
};