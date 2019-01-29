#pragma once

#include "BoundingSphere.h"

class AxisAlignedBoundingBox
{
public:
	D3DXVECTOR3 initialMinimumPoint, initialMaximumPoint, transformedMinimumPoint, transformedMaximumPoint;

	~AxisAlignedBoundingBox();
	AxisAlignedBoundingBox(D3DXVECTOR3, D3DXVECTOR3);
		
	void Update(D3DXMATRIX);

	bool Collides(AxisAlignedBoundingBox*);
	bool Collides(BoundingSphere*);
	bool Collides(BoundingPoint*);
};

