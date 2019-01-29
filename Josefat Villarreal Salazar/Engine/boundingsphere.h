#pragma once

#include "boundingpoint.h"

class BoundingSphere
{
public:
	D3DXVECTOR3 initialPosition, transformedPosition;
	FLOAT radius, transformedRadius;

	~BoundingSphere();

	BoundingSphere(D3DXVECTOR3, FLOAT);
	
	void Update(D3DXMATRIX);

	bool Collides(BoundingSphere*);
	bool Collides(BoundingPoint*);
};

