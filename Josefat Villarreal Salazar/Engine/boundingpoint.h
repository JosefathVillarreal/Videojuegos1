#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

class BoundingPoint
{
public:
	D3DXVECTOR3 initialPosition, transformedPosition;

	~BoundingPoint();

	BoundingPoint(D3DXVECTOR3);
	
	void Update(D3DXMATRIX);

	bool Collides(BoundingPoint*);
};

