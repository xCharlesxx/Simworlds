#pragma once
#include <vector>

struct BoidSettings
{
	std::vector<Boid*> m_boids;
	int requestedSpecialBoid = 0; 
	float percentCohesion = 0.1;
	float separationDistance = 0.05;
	float homingInstinct = 0.005;
	float alignmentForce = 0.15;
	float maxAcc = 0.1;
	Vector4 colour = Vector4(1, 0, 0, 1);
};

