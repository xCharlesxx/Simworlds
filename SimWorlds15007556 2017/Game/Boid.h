#pragma once
#include "VBGO.h"
#include "vertex.h"

//=================================================================
//Base Boid Class (i.e. a GO the Boid controls)
//=================================================================

class Boid : public VBGO
{

public:
	Boid(ID3D11Device* _pd3dDevice);
	~Boid();
	void Update(GameData* _GD, Vector3 m_force);
	virtual void Draw(DrawData* _DD) override;
	bool getAlive(); 
	Vector3 getVelocity(); 
	void setAlive(bool alive); 
	void outputBoidPos(); 
protected:
	int m_size;
	Vector3 velocity; 
	myVertex* m_vertices;
	bool isAlive = false; 
};