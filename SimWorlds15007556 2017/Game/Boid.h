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
	Boid(ID3D11Device* _pd3dDevice, int type, std::vector<BoidSettings*> &typeList);
	~Boid();
	void Update(GameData* _GD, Vector3 m_force);
	virtual void Draw(DrawData* _DD) override;
	Vector3 getVelocity(); 
	void outputBoidPos(); 
	float getSeparation();
	float getCohesion();
	float getAlignment();
	float getHoming();
	float getAcc(); 
	int getType(); 
protected:
	int m_type = 0; 
	Vector3 velocity; 
	myVertex* m_vertices;
	std::vector<BoidSettings*>& m_typelist;
};