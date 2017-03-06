#pragma once
#include "CMOGO.h"

//=================================================================
//Base Boid Class (i.e. a GO the Boid controls)
//=================================================================

class Boid : public CMOGO
{

public:
	Boid(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF);
	~Boid();
	void Update(); 
	virtual void Tick(GameData* _GD) override;
protected:

};