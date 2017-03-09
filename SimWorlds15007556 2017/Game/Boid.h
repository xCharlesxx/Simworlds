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
	void Update(GameData* _GD);
	virtual void Draw(DrawData* _DD) override;
	bool getAlive(); 
	void setAlive(bool alive); 
protected:
	int m_size;
	myVertex* m_vertices;
	bool isAlive = false; 
	bool once = false; 
};