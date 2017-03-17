#include "Boid.h"
#include <dinput.h>
#include "GameData.h"
#include <iostream>
#include <ctime>
Boid::Boid(ID3D11Device* _pd3dDevice)
{
	int randx = rand() % 20 - 10;
	int randy = rand() % 20 - 10;
	int randz = rand() % 20 - 10;
	m_pos = Vector3(randx, randy, randz);
	velocity = Vector3::Zero;

	int numVerts = 3;
	int vert = 0; 
	m_numPrims = numVerts / 3;
	m_vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];
	for (int i = 0; i<numVerts; i++)
	{
		indices[i] = i;
		m_vertices[i].texCoord = Vector2::One;
	}
	m_vertices[0].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	m_vertices[0].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[1].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	m_vertices[1].Pos = Vector3(0.0f, 0.0f, 2.0f);
	m_vertices[2].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	m_vertices[2].Pos = Vector3(3.0f, 0.0f, 1.0f);


	for (int i = 0; i< m_numPrims; i++)
	{
		WORD V1 = 3 * i;
		WORD V2 = 3 * i + 1;
		WORD V3 = 3 * i + 2;

		//build normals
		Vector3 norm;
		Vector3 vec1 = m_vertices[V1].Pos - m_vertices[V2].Pos;
		Vector3 vec2 = m_vertices[V3].Pos - m_vertices[V2].Pos;
		norm = vec1.Cross(vec2);
		norm.Normalize();

		m_vertices[V1].Norm = norm;
		m_vertices[V2].Norm = norm;
		m_vertices[V3].Norm = norm;
	}

	BuildIB(_pd3dDevice, indices);
	BuildVB(_pd3dDevice, numVerts, m_vertices);

	delete[] indices;    //this is no longer needed as this is now in the index Buffer
	delete[] m_vertices; //this is no longer needed as this is now in the Vertex Buffer
	m_vertices = nullptr;
}

Boid::~Boid()
{
	//tidy up anything I've created
}

void Boid::Update(GameData* _GD, Vector3 m_force)
{
	velocity = velocity + m_force; 
	//std::cout << velocity.x; 
	/*if (velocity.x > maxVelocity.x || velocity.y > maxVelocity.y || velocity.z > maxVelocity.z)
	{
		velocity = maxVelocity;
	}*/
	m_pos = m_pos + velocity;
	m_rotMat = Matrix::CreateWorld(m_pos, velocity, Vector3::Up); 
	Matrix  scaleMat = Matrix::CreateScale(m_scale);
	Matrix  transMat = Matrix::CreateTranslation(m_pos);
	m_worldMat = m_fudge * scaleMat * m_rotMat * transMat;
	/*std::cout << "Velocity is currently: (";
	std::cout << velocity.x;
	std::cout << ", ";
	std::cout << velocity.y;
	std::cout << ", ";
	std::cout << velocity.z;
	std::cout << ")\n";*/
	//outputBoidPos(); 
	//VBGO::Tick(_GD);
}

void Boid::Draw(DrawData * _DD)
{
	VBGO::Draw(_DD);
}

bool Boid::getAlive()
{
	return isAlive;
}

Vector3 Boid::getVelocity()
{
	return velocity; 
}

void Boid::setAlive(bool alive)
{
	isAlive = alive; 
}

void Boid::outputBoidPos()
{
	std::cout << "The Coordinates for this boid is: (";
	std::cout << this->m_pos.x;
	std::cout << ", ";
	std::cout << this->m_pos.y;
	std::cout << ", ";
	std::cout << this->m_pos.z;
	std::cout << ")\n";
}
