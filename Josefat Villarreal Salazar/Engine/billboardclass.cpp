#include "billboardClass.h"

bool BillboardClass::Strategy()
{
	m_vertexCount = 4;
	m_indexCount = 6;

	//Create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	//Create the index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	vertices[0].position = D3DXVECTOR3(-2.5, 0, 0);
	vertices[0].normal = D3DXVECTOR3(0, 0, -1);
	vertices[0].tangent = D3DXVECTOR3(0, 1, 0);
	vertices[0].binormal = D3DXVECTOR3(1, 0, 0);
	vertices[0].texture = D3DXVECTOR2(0, 1);

	vertices[1].position = D3DXVECTOR3(2.5, 0, 0);
	vertices[1].normal = D3DXVECTOR3(0, 0, -1);
	vertices[1].tangent = D3DXVECTOR3(0, 1, 0);
	vertices[1].binormal = D3DXVECTOR3(1, 0, 0);
	vertices[1].texture = D3DXVECTOR2(1, 1);

	vertices[2].position = D3DXVECTOR3(-2.5, 5, 0);
	vertices[2].normal = D3DXVECTOR3(0, 0, -1);
	vertices[2].tangent = D3DXVECTOR3(0, 1, 0);
	vertices[2].binormal = D3DXVECTOR3(1, 0, 0);
	vertices[2].texture = D3DXVECTOR2(0, 0);

	vertices[3].position = D3DXVECTOR3(2.5, 5, 0);
	vertices[3].normal = D3DXVECTOR3(0, 0, -1);
	vertices[3].tangent = D3DXVECTOR3(0, 1, 0);
	vertices[3].binormal = D3DXVECTOR3(1, 0, 0);
	vertices[3].texture = D3DXVECTOR2(1, 0);

	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 3;

	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 1;

	return true;
}

D3DXMATRIX BillboardClass::GenerateWorldMatrix(D3DXVECTOR3 cameraPosition, D3DXVECTOR3 modelPosition)
{
	D3DXMATRIX worldMatrix, translateMatrix;
	double angle;
	float rotation;

	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	angle = atan2(modelPosition.x - cameraPosition.x, modelPosition.z - cameraPosition.z) * (180.0 / D3DX_PI);

	// Convert rotation into radians.
	rotation = (float)angle * 0.0174532925f;

	// Setup the rotation the billboard at the origin using the world matrix.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// Setup the translation matrix from the billboard model.
	D3DXMatrixTranslation(&translateMatrix, modelPosition.x, modelPosition.y, modelPosition.z);

	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	return worldMatrix;
}

BillboardClass::BillboardClass()
{
}


BillboardClass::~BillboardClass()
{
}
