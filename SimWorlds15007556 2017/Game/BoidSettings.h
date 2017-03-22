#pragma once

struct BoidSettings
{
	int boidsInClan = 0; 
	int requestedSpecialBoid = 0; 
	float percentCohesion = 0.1;
	float seperationDistance = 0.05;
	float homingInstinct = 0.005;
	float alignmentForce = 0.15;
	float maxAcc = 0.1;
	Vector4 colour = Vector4(1, 0, 0, 1);
};

