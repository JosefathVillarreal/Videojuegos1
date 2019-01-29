#include "axisalignedboundingbox.h"

AxisAlignedBoundingBox::~AxisAlignedBoundingBox() {};

AxisAlignedBoundingBox::AxisAlignedBoundingBox(D3DXVECTOR3 MinimumPoint, D3DXVECTOR3 MaximumPoint)
{
	initialMinimumPoint = transformedMinimumPoint = MinimumPoint;
	initialMaximumPoint = transformedMaximumPoint = MaximumPoint;
}

void AxisAlignedBoundingBox::Update(D3DXMATRIX world)
{
	D3DXVECTOR3 scaledMinimumPoint = initialMinimumPoint;
	scaledMinimumPoint.x *= world.m[0][0];
	scaledMinimumPoint.y *= world.m[1][1];
	scaledMinimumPoint.z *= world.m[2][2];
	transformedMinimumPoint = scaledMinimumPoint + D3DXVECTOR3(world.m[3][0], world.m[3][1], world.m[3][2]);

	D3DXVECTOR3 scaledMaximumPoint = initialMaximumPoint;
	scaledMaximumPoint.x *= world.m[0][0];
	scaledMaximumPoint.y *= world.m[1][1];
	scaledMaximumPoint.z *= world.m[2][2];
	transformedMaximumPoint = scaledMaximumPoint + D3DXVECTOR3(world.m[3][0], world.m[3][1], world.m[3][2]);
}

bool AxisAlignedBoundingBox::Collides(AxisAlignedBoundingBox *box)
{
	return (this->transformedMaximumPoint.x >= box->transformedMinimumPoint.x &&
		this->transformedMinimumPoint.x <= box->transformedMaximumPoint.x &&
		this->transformedMaximumPoint.y >= box->transformedMinimumPoint.y &&
		this->transformedMinimumPoint.y <= box->transformedMaximumPoint.y &&
		this->transformedMaximumPoint.z >= box->transformedMinimumPoint.z &&
		this->transformedMinimumPoint.z <= box->transformedMaximumPoint.z);
}

bool AxisAlignedBoundingBox::Collides(BoundingSphere *sphere)
{
	D3DXVECTOR3 nearestPoint;
	nearestPoint.x = (sphere->transformedPosition.x < transformedMinimumPoint.x) ? transformedMinimumPoint.x : (sphere->transformedPosition.x > transformedMaximumPoint.x) ? transformedMaximumPoint.x : sphere->transformedPosition.x;
	nearestPoint.y = (sphere->transformedPosition.y < transformedMinimumPoint.y) ? transformedMinimumPoint.y : (sphere->transformedPosition.y > transformedMaximumPoint.y) ? transformedMaximumPoint.y : sphere->transformedPosition.y;
	nearestPoint.z = (sphere->transformedPosition.z < transformedMinimumPoint.z) ? transformedMinimumPoint.z : (sphere->transformedPosition.z > transformedMaximumPoint.z) ? transformedMaximumPoint.z : sphere->transformedPosition.z;

	D3DXVECTOR3 vectorSub;
	D3DXVec3Subtract(&vectorSub, &sphere->transformedPosition, &nearestPoint);
	FLOAT length = D3DXVec3Length(&vectorSub);
	return length <= (sphere->radius);
}

bool AxisAlignedBoundingBox::Collides(BoundingPoint *point)
{
	return (this->transformedMaximumPoint.x >= point->transformedPosition.x &&
		this->transformedMinimumPoint.x <= point->transformedPosition.x &&
		this->transformedMaximumPoint.y >= point->transformedPosition.y &&
		this->transformedMinimumPoint.y <= point->transformedPosition.y &&
		this->transformedMaximumPoint.z >= point->transformedPosition.z &&
		this->transformedMinimumPoint.z <= point->transformedPosition.z);
}

