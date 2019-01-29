#pragma once

#include <vector>
#include "mesh.h"
#include "material.h"

using namespace std;

struct OBJvertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texCoord;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 binormal;

	bool operator == (OBJvertex a) {

		return (position.x == a.position.x
			&& position.y == a.position.y
			&& position.z == a.position.z
			&& texCoord.x == a.texCoord.x
			&& texCoord.y == a.texCoord.y
			&& normal.x == a.normal.x
			&& normal.y == a.normal.y
			&& normal.z == a.normal.z);
	}
};

class Group
{
private:
	string name;
	vector <Mesh> *meshList;
	vector <Material> *materialList;
	ID3D11Buffer *vertexBuffer, *indexBuffer;
	
	Collider collider;
	AxisAlignedBoundingBox *aabb;
	BoundingSphere *bs;
	BoundingPoint *bp;

public:

	Group();
	~Group();

	void Render(ID3D11DeviceContext*);

	void UpdateColliders(D3DXMATRIX);
	bool Collides(Group*);

	void SetName(string);
	void SetMeshList(vector <Mesh>*);
	void SetMaterialList(vector <Material>*);
	void SetVertexBuffer(ID3D11Device*, vector <OBJvertex>*);
	void SetIndexBuffer(ID3D11Device*, vector <UINT>*);

	string GetName();
	vector <Mesh> *GetMeshList();
	vector <Material> *GetMaterialList();
	ID3D11Buffer* GetBertexBuffer();
	ID3D11Buffer* GetIndexBuffer();

	Collider GetCollider();
	AxisAlignedBoundingBox* GetAABB();
	BoundingSphere* GetBS();
	BoundingPoint *GetBP();

	void SetCollider(Collider);
	void SetAABB(AxisAlignedBoundingBox*);
	void SetBS(BoundingSphere*);
	void SetBP(BoundingPoint *);
};

