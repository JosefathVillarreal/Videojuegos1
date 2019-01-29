#include "group.h"

Group::Group()
{
	name = "";
	meshList = new vector <Mesh>();
	materialList = new vector <Material>();
	vertexBuffer = nullptr;
	indexBuffer = nullptr;

	collider = BOUNDINGSPHERE;

	aabb = nullptr;
	bs = nullptr;
	bp = nullptr;
}

Group::~Group()
{
}

void Group::Render(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride = sizeof(OBJvertex);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Group::UpdateColliders(D3DXMATRIX world)
{
	this->aabb->Update(world);
	this->bs->Update(world);
	this->bp->Update(world);

	for (auto &mesh : *meshList)
	{
		mesh.UpdateColliders(world);
	}
}

bool Group::Collides(Group *group)
{
	bool result = false;

	switch (this->collider)
	{
	case BOUNDINGPOINT:
		switch (group->GetCollider())
		{
		case BOUNDINGPOINT:
			result = group->GetBP()->Collides(this->bp);
			break;
		case BOUNDINGSPHERE:
			result = group->GetBS()->Collides(this->bp);
			break;
		case AXISALIGNEDBOUNGINGBOX:
			result = group->GetAABB()->Collides(this->bp);
			break;
		default:
			result = false;
		}
		break;
	case BOUNDINGSPHERE:
		switch (group->GetCollider())
		{
		case BOUNDINGPOINT:
			result = this->bs->Collides(group->GetBP());
			break;
		case BOUNDINGSPHERE:
			result = group->GetBS()->Collides(this->bs);
			break;
		case AXISALIGNEDBOUNGINGBOX:
			result = group->GetAABB()->Collides(this->bs);
			break;
		default:
			result = false;
		}
		break;
	case AXISALIGNEDBOUNGINGBOX:
		switch (group->GetCollider())
		{
		case BOUNDINGPOINT:
			result = this->aabb->Collides(group->bp);
			break;
		case BOUNDINGSPHERE:
			result = this->aabb->Collides(group->bs);
			break;
		case AXISALIGNEDBOUNGINGBOX:
			result = group->GetAABB()->Collides(this->aabb);
			break;
		default:
			result = false;
		}
		break;
	default:
		result = false;
	}

	for (auto &mesh : *meshList)
	{
		for (auto &mesh2 : *group->GetMeshList())
		{
			if (mesh.Collides(&mesh2)) return true;
		}
	}

	return result;
}

void Group::SetName(string name) 
{
	this->name = name;
}

void Group::SetMeshList(vector <Mesh>* meshList)
{
	this->meshList = meshList;
}

void Group::SetMaterialList(vector <Material>* materialList)
{
	this->materialList = materialList;
}

void Group::SetVertexBuffer(ID3D11Device* device, vector <OBJvertex>* vertices) {

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(OBJvertex) * vertices->size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));
	vertexData.pSysMem = &(*vertices)[0];

	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
}

void Group::SetIndexBuffer(ID3D11Device* device, vector <UINT>* indices) {

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * indices->size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(D3D11_SUBRESOURCE_DATA));
	indexData.pSysMem = &(*indices)[0];

	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
}

string Group::GetName()
{
	return this->name;
}

vector<Mesh>* Group::GetMeshList()
{
	return this->meshList;
}

vector<Material>* Group::GetMaterialList()
{
	return this->materialList;
}

ID3D11Buffer * Group::GetBertexBuffer()
{
	return this->vertexBuffer;
}

ID3D11Buffer * Group::GetIndexBuffer()
{
	return this->indexBuffer;
}

Collider Group::GetCollider()
{
	return this->collider;
}

AxisAlignedBoundingBox * Group::GetAABB()
{
	return this->aabb;
}

BoundingSphere * Group::GetBS()
{
	return this->bs;
}

BoundingPoint * Group::GetBP()
{
	return this->bp;
}

void Group::SetCollider(Collider collider)
{
	this->collider = collider;
}

void Group::SetAABB(AxisAlignedBoundingBox *aabb)
{
	this->aabb = aabb;
}

void Group::SetBS(BoundingSphere *bs)
{
	this->bs = bs;
}

void Group::SetBP(BoundingPoint *bp)
{
	this->bp = bp;
}
