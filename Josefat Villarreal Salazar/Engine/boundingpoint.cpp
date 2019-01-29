#include "boundingpoint.h"

BoundingPoint::~BoundingPoint() {};

BoundingPoint::BoundingPoint(D3DXVECTOR3 position)
{
	initialPosition = transformedPosition = position;
};

void BoundingPoint::Update(D3DXMATRIX world)
{
	D3DXVECTOR3 scaledPoint = initialPosition;
	scaledPoint.x *= world.m[0][0];
	scaledPoint.y *= world.m[1][1];
	scaledPoint.z *= world.m[2][2];
	transformedPosition = scaledPoint + D3DXVECTOR3(world.m[3][0], world.m[3][1], world.m[3][2]);
}

bool BoundingPoint::Collides(BoundingPoint *point)
{
	D3DXVECTOR3 vectorSub;
	D3DXVec3Subtract(&vectorSub, &point->transformedPosition, &this->transformedPosition);
	FLOAT length = D3DXVec3Length(&vectorSub);
	return length <= 0.01;
}