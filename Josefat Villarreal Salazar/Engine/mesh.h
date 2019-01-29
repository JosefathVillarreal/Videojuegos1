#pragma once

#include <string>
#include "axisalignedboundingbox.h"

using namespace std;

enum Collider
{
	BOUNDINGPOINT,
	BOUNDINGSPHERE,
	AXISALIGNEDBOUNGINGBOX
};

class Mesh
{
private:
	string materialName;
	unsigned int materialIndex;

	unsigned int offset;
	unsigned int size;

	Collider collider;
	AxisAlignedBoundingBox *aabb;
	BoundingSphere *bs;
	BoundingPoint *bp;

public:

	Mesh();
	~Mesh();

	void UpdateColliders(D3DXMATRIX);
	bool Collides(Mesh*);

	string GetMaterialName();
	unsigned int GetMaterialIndex();

	unsigned int GetOffset();
	unsigned int GetSize();

	Collider GetCollider();
	AxisAlignedBoundingBox* GetAABB();
	BoundingSphere* GetBS();
	BoundingPoint *GetBP();

	void SetMaterialName(string);
	void SetMaterialIndex(unsigned int);

	void SetOffset(unsigned int);
	void SetSize(unsigned int);

	void SetCollider(Collider);
	void SetAABB(AxisAlignedBoundingBox*);
	void SetBS(BoundingSphere*);
	void SetBP(BoundingPoint *);
};

