#include "mesh.h"

Mesh::Mesh()
{
	materialName = "";
	materialIndex = 0;

	offset = 0;
	size = 0;

	collider = AXISALIGNEDBOUNGINGBOX;

	aabb = nullptr;
	bs = nullptr;
	bp = nullptr;
}

Mesh::~Mesh()
{
}

void Mesh::UpdateColliders(D3DXMATRIX world)
{
	this->aabb->Update(world);
	this->bs->Update(world);
	this->bp->Update(world);
}

bool Mesh::Collides(Mesh *mesh)
{
	bool result = false;

	switch (this->collider)
	{
	case BOUNDINGPOINT:
		switch (mesh->GetCollider())
		{
		case BOUNDINGPOINT:
			result = mesh->GetBP()->Collides(this->bp);
			break;
		case BOUNDINGSPHERE:
			result = mesh->GetBS()->Collides(this->bp);
			break;
		case AXISALIGNEDBOUNGINGBOX:
			result = mesh->GetAABB()->Collides(this->bp);
			break;
		default:
			result = false;
		}
		break;
	case BOUNDINGSPHERE:
		switch (mesh->GetCollider())
		{
		case BOUNDINGPOINT:
			result = this->bs->Collides(mesh->GetBP());
			break;
		case BOUNDINGSPHERE:
			result = mesh->GetBS()->Collides(this->bs);
			break;
		case AXISALIGNEDBOUNGINGBOX:
			result = mesh->GetAABB()->Collides(this->bs);
			break;
		default:
			result = false;
		}
		break;
	case AXISALIGNEDBOUNGINGBOX:
		switch (mesh->GetCollider())
		{
		case BOUNDINGPOINT:
			result = this->aabb->Collides(mesh->bp);
			break;
		case BOUNDINGSPHERE:
			result = this->aabb->Collides(mesh->bs);
			break;
		case AXISALIGNEDBOUNGINGBOX:
			result = mesh->GetAABB()->Collides(this->aabb);
			break;
		default:
			result = false;
		}
		break;
	default:
		result = false;
	}

	return result;
}

string Mesh::GetMaterialName()
{
	return this->materialName;
}

unsigned int Mesh::GetMaterialIndex()
{
	return this->materialIndex;
}

unsigned int Mesh::GetOffset()
{
	return this->offset;
}

unsigned int Mesh::GetSize()
{
	return this->size;
}

Collider Mesh::GetCollider()
{
	return this->collider;
}

AxisAlignedBoundingBox * Mesh::GetAABB()
{
	return this->aabb;
}

BoundingSphere * Mesh::GetBS()
{
	return this->bs;
}

BoundingPoint * Mesh::GetBP()
{
	return this->bp;
}

void Mesh::SetMaterialName(string materialName)
{
	this->materialName = materialName;
}

void Mesh::SetMaterialIndex(unsigned int materialIndex)
{
	this->materialIndex = materialIndex;
}

void Mesh::SetOffset(unsigned int offset)
{
	this->offset = offset;
}

void Mesh::SetSize(unsigned int size)
{
	this->size = size;
}

void Mesh::SetCollider(Collider collider)
{
	this->collider = collider;
}

void Mesh::SetAABB(AxisAlignedBoundingBox *aabb)
{
	this->aabb = aabb;
}

void Mesh::SetBS(BoundingSphere *bs)
{
	this->bs = bs;
}

void Mesh::SetBP(BoundingPoint *bp)
{
	this->bp = bp;
}
