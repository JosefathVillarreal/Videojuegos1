#pragma once

#include <fstream>
#include "group.h"

class Model
{
	vector <Group>* groupList;
	
	Collider collider;
	AxisAlignedBoundingBox *aabb;
	BoundingSphere *bs;
	BoundingPoint *bp;

	Model();
	~Model();

	static vector <Group>* LoadObj(ID3D11Device*, string, string, D3DXVECTOR3&, D3DXVECTOR3&);
	static void ComputeTangentSpace(vector <OBJvertex>&, vector <UINT>&);
	static vector <Material>* LoadMtl(ID3D11Device*, string, string);

public:
	static Model *ObjToModel(ID3D11Device*, string, string);
	
	void Render(ID3D11DeviceContext*);
	
	void UpdateColliders(D3DXMATRIX);
	bool Collides(Model*);

	void SetGroupList(vector <Group>*);

	vector <Group>* GetGroupList();

	Collider GetCollider();
	AxisAlignedBoundingBox* GetAABB();
	BoundingSphere* GetBS();
	BoundingPoint *GetBP();

	void SetCollider(Collider);
	void SetAABB(AxisAlignedBoundingBox*);
	void SetBS(BoundingSphere*);
	void SetBP(BoundingPoint *);
};

