#pragma once

struct BoidSettings
{
	int requestedSpecialBoid = 0; 
	float percentCohesion = 0.1;
	float seperationDistance = 0.05;
	float homingInstinct = 0.005;
	float alignmentForce = 0.15;
	float maxAcc = 0.1;
};