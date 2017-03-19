#pragma once
#include "CMOGO.h"

//=================================================================
//Terrian Game Object Class (i.e. an Object that just sits there)
//=================================================================

struct GameData;

class InvisibleObject : public CMOGO
{
public:
	InvisibleObject(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale);
	~InvisibleObject();

	void Tick(GameData* _GD) override {};

protected:

};