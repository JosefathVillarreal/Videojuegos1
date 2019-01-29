#include "boundingsphere.h"

BoundingSphere::~BoundingSphere() {};

BoundingSphere::BoundingSphere(D3DXVECTOR3 position, FLOAT radius)
{
	initialPosition = transformedPosition = position;
	transformedRadius = this->radius = radius;
};

void BoundingSphere::Update(D3DXMATRIX world)
{
	D3DXVECTOR3 scaledPoint = initialPosition;
	scaledPoint.x *= world.m[0][0];
	scaledPoint.y *= world.m[1][1];
	scaledPoint.z *= world.m[2][2];
	transformedPosition = scaledPoint + D3DXVECTOR3(world.m[3][0], world.m[3][1], world.m[3][2]);

	float size = max(max(world.m[0][0], world.m[1][1]), world.m[2][2]);
	transformedRadius = radius * size;
}

bool BoundingSphere::Collides(BoundingSphere *sphere)
{
	D3DXVECTOR3 vectorSub;
	D3DXVec3Subtract(&vectorSub, &sphere->transformedPosition, &this->transformedPosition);
	FLOAT length = D3DXVec3Length(&vectorSub);
	return length <= (sphere->radius + this->radius);
}

bool BoundingSphere::Collides(BoundingPoint *point)
{
	D3DXVECTOR3 vectorSub;
	D3DXVec3Subtract(&vectorSub, &point->transformedPosition, &this->transformedPosition);
	FLOAT length = D3DXVec3Length(&vectorSub);
	return length <= this->radius;
}