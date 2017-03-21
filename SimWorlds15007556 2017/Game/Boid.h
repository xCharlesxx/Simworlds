#pragma once
#include "VBGO.h"
#include "vertex.h"
#include <vector>

//=================================================================
//Base Boid Class (i.e. a GO the Boid controls)
//=================================================================

struct BoidSettings;

class Boid : public VBGO
{

public:
	Boid(ID3D11Device* _pd3dDevice, std::vector<BoidSettings*> &typeList);
	~Boid();
	void Update(GameData* _GD, Vector3 m_force);
	virtual void Draw(DrawData* _DD) override;
	bool getAlive(); 
	Vector3 getVelocity(); 
	void setAlive(bool alive); 
	void outputBoidPos(); 
	void setBoid(float sep, float coh, float ali, float hom, float acc);
	float getSeperation();
	float getCohesion();
	float getAlignment();
	float getHoming();
	float getAcc(); 
protected:
	int m_size;
	int type = 0; 
	Vector3 velocity; 
	myVertex* m_vertices;
	bool isAlive = false; 
	std::vector<BoidSettings*>& m_typelist;
};