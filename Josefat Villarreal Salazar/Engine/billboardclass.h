#pragma once
#include "modelclass.h"
class BillboardClass :
	public ModelClass
{
protected:
	bool Strategy();

public:
	BillboardClass();
	~BillboardClass();

	static D3DXMATRIX GenerateWorldMatrix(D3DXVECTOR3, D3DXVECTOR3);
};

