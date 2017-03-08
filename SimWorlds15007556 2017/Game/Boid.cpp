#include "Boid.h"
#include <dinput.h>
#include "GameData.h"

Boid::Boid(ID3D11Device* _pd3dDevice)
{
	m_pos = Vector3(0, 10, 0);
	int numVerts = 3;
	int vert = 0; 
	m_numPrims = numVerts / 3;
	m_vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];
	m_vertices[vert].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	m_vertices[vert++].Pos = Vector3(1, 0, 1);
	m_vertices[vert].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	m_vertices[vert++].Pos = Vector3(1, 0, 2);
	m_vertices[vert].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	m_vertices[vert++].Pos = Vector3(2, 0, 1.5);
	for (int i = 0; i<m_numPrims; i++)
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

void Boid::Update(GameData* _GD)
{
	VBGO::Tick(_GD);
}

void Boid::Tick(GameData* _GD)
{
	//switch (_GD->m_GS)
	//{
	//case GS_PLAY_MAIN_CAM:
	//{
	//	{
	//		//MOUSE CONTROL SCHEME HERE
	//		float speed = 10.0f;
	//		m_acc.x += speed * _GD->m_mouseState->lX;
	//		m_acc.z += speed * _GD->m_mouseState->lY;
	//		break;
	//	}
	//}
	//case GS_PLAY_TPS_CAM:
	//{
	//	//TURN AND FORWARD CONTROL HERE
	//	Vector3 forwardMove = 40.0f * Vector3::Forward;
	//	Matrix rotMove = Matrix::CreateRotationY(m_yaw);
	//	forwardMove = Vector3::Transform(forwardMove, rotMove);
	//	if (_GD->m_keyboardState[DIK_W] & 0x80)
	//	{
	//		m_acc += forwardMove;
	//	}
	//	if (_GD->m_keyboardState[DIK_S] & 0x80)
	//	{
	//		m_acc -= forwardMove;
	//	}
	//	break;
	//}
	//}

	////change orinetation of player
	//float rotSpeed = 2.0f * _GD->m_dt;
	//if (_GD->m_keyboardState[DIK_A] & 0x80)
	//{
	//	m_yaw += rotSpeed;
	//}
	//if (_GD->m_keyboardState[DIK_D] & 0x80)
	//{
	//	m_yaw -= rotSpeed;
	//}

	////move player up and down
	//if (_GD->m_keyboardState[DIK_R] & 0x80)
	//{
	//	m_acc.y += 40.0f;
	//}

	//if (_GD->m_keyboardState[DIK_F] & 0x80)
	//{
	//	m_acc.y -= 40.0f;
	//}

	////limit motion of the player
	//float length = m_pos.Length();
	//float maxLength = 500.0f;
	//if (length > maxLength)
	//{
	//	m_pos.Normalize();
	//	m_pos *= maxLength;
	//	m_vel *= -0.9; //VERY simple bounce back
	//}

	//apply my base behaviour
	//VBGO::Tick(_GD);
}