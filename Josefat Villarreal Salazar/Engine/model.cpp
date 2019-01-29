#include "model.h"

Model::Model()
{
	groupList = new vector <Group>();

	collider = BOUNDINGSPHERE;

	aabb = nullptr;
	bs = nullptr;
	bp = nullptr;
}

Model::~Model()
{
	delete groupList;
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	for (auto &group : *groupList)
	{
		group.Render(deviceContext);
	}
}

void Model::UpdateColliders(D3DXMATRIX world)
{
	this->aabb->Update(world);
	this->bs->Update(world);
	this->bp->Update(world);

	for (auto &group : *groupList)
	{
		group.UpdateColliders(world);
	}
}

bool Model::Collides(Model *model)
{
	bool result = false;

	switch (this->collider)
	{
	case BOUNDINGPOINT:
		switch (model->GetCollider())
		{
		case BOUNDINGPOINT:
			result = model->GetBP()->Collides(this->bp);
			break;
		case BOUNDINGSPHERE:
			result = model->GetBS()->Collides(this->bp);
			break;
		case AXISALIGNEDBOUNGINGBOX:
			result = model->GetAABB()->Collides(this->bp);
			break;
		default:
			result = false;
		}
		break;
	case BOUNDINGSPHERE:
		switch (model->GetCollider())
		{
		case BOUNDINGPOINT:
			result = this->bs->Collides(model->GetBP());
			break;
		case BOUNDINGSPHERE:
			result = model->GetBS()->Collides(this->bs);
			break;
		case AXISALIGNEDBOUNGINGBOX:
			result = model->GetAABB()->Collides(this->bs);
			break;
		default:
			result = false;
		}
		break;
	case AXISALIGNEDBOUNGINGBOX:
		switch (model->GetCollider())
		{
		case BOUNDINGPOINT:
			result = this->aabb->Collides(model->bp);
			break;
		case BOUNDINGSPHERE:
			result = this->aabb->Collides(model->bs);
			break;
		case AXISALIGNEDBOUNGINGBOX:
			result = model->GetAABB()->Collides(this->aabb);
			break;
		default:
			result = false;
		}
		break;
	default:
		result = false;
	}

	if (result)
	{
		for (auto &group : *groupList)
		{
			for (auto &group2 : *model->GetGroupList())
			{
				if (group.Collides(&group2)) return true;
			}
		}
	}

	return result;
}

void Model::SetGroupList(vector <Group>* groupList)
{
	this->groupList = groupList;
}

vector <Group>* Model::GetGroupList()
{
	return groupList;
}

Model *Model::ObjToModel(ID3D11Device* device, string relativePath, string fileName) {

	D3DXVECTOR3 modelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXVECTOR3 modelMax = D3DXVECTOR3(FLT_MIN, FLT_MIN, FLT_MIN);

	vector <Group>* groupList = LoadObj(device, relativePath, fileName, modelMin, modelMax);

	D3DXVECTOR3 size = (modelMin - modelMax) / 2;
	size *= -1.0f;
	D3DXVECTOR3 position = modelMin + size;
	FLOAT radio = max(max(size.x, size.y), size.z);

	Model *_model = new Model();
	_model->SetGroupList(groupList);
	_model->SetAABB(new AxisAlignedBoundingBox(modelMin, modelMax));
	_model->SetBS(new BoundingSphere(position, radio));
	_model->SetBP(new BoundingPoint(position));

	return _model;
}

vector <Group>* Model::LoadObj(ID3D11Device* device, string relativePath, string fileName, D3DXVECTOR3& min, D3DXVECTOR3& max)
{
	vector <Group>* groupList = new vector <Group>();

	vector <OBJvertex> vertexBuffer;
	vector <UINT> indexBuffer;

	vector <D3DXVECTOR3> positions;
	vector <D3DXVECTOR3> normals;
	vector <D3DXVECTOR2> texCoords;

	D3DXVECTOR3 modelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXVECTOR3 modelMax = D3DXVECTOR3(FLT_MIN, FLT_MIN, FLT_MIN);

	D3DXVECTOR3 groupMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXVECTOR3 groupMax = D3DXVECTOR3(FLT_MIN, FLT_MIN, FLT_MIN);

	D3DXVECTOR3 meshMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXVECTOR3 meshMax = D3DXVECTOR3(FLT_MIN, FLT_MIN, FLT_MIN);

	string mtl;

	ifstream file;
	file.open(relativePath + "/" + fileName);

	if (file.is_open())
	{
		while (file.good())
		{
			string token;
			file >> token;

			if (token == "mtllib")
			{
				file >> mtl;
			}
			else if (token == "g")
			{
				if (!groupList->empty())
				{
					groupList->back().GetMeshList()->back().SetSize((indexBuffer.size() - groupList->back().GetMeshList()->back().GetOffset()));

					D3DXVECTOR3 groupSize = (groupMax - groupMin) / 2;
					groupSize *= -1.0f;
					D3DXVECTOR3 groupPosition = groupMin + groupSize;
					FLOAT groupRadio = max(max(groupSize.x, groupSize.y), groupSize.z);
					groupList->back().SetAABB(new AxisAlignedBoundingBox(groupMin, groupMax));
					groupList->back().SetBS(new BoundingSphere(groupPosition, groupRadio));
					groupList->back().SetBP(new BoundingPoint(groupPosition));
					groupMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
					groupMax = D3DXVECTOR3(FLT_MIN, FLT_MIN, FLT_MIN);

					D3DXVECTOR3 size = (meshMax - meshMin) / 2;
					size *= -1.0f;
					D3DXVECTOR3 position = meshMin + size;
					FLOAT radio = max(max(size.x, size.y), size.z);
					groupList->back().GetMeshList()->back().SetAABB(new AxisAlignedBoundingBox(meshMin, meshMax));
					groupList->back().GetMeshList()->back().SetBS(new BoundingSphere(position, radio));
					groupList->back().GetMeshList()->back().SetBP(new BoundingPoint(position));
					meshMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
					meshMax = D3DXVECTOR3(FLT_MIN, FLT_MIN, FLT_MIN);

					ComputeTangentSpace(vertexBuffer, indexBuffer);
					groupList->back().SetVertexBuffer(device, &vertexBuffer);
					groupList->back().SetIndexBuffer(device, &indexBuffer);
				}

				Group _group;
				groupList->push_back(_group);
				string name;
				file >> name;
				groupList->back().SetName(name);

				vertexBuffer.clear();
				indexBuffer.clear();
			}
			else if (token == "usemtl")
			{
				if (!groupList->back().GetMeshList()->empty())
				{
					groupList->back().GetMeshList()->back().SetSize((indexBuffer.size() - groupList->back().GetMeshList()->back().GetOffset()));

					D3DXVECTOR3 size = (meshMax - meshMin) / 2;
					size *= -1.0f;
					D3DXVECTOR3 position = meshMin + size;
					FLOAT radio = max(max(size.x, size.y), size.z);
					groupList->back().GetMeshList()->back().SetAABB(new AxisAlignedBoundingBox(meshMin, meshMax));
					groupList->back().GetMeshList()->back().SetBS(new BoundingSphere(position, radio));
					groupList->back().GetMeshList()->back().SetBP(new BoundingPoint(position));
					meshMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
					meshMax = D3DXVECTOR3(FLT_MIN, FLT_MIN, FLT_MIN);
				}

				Mesh _mesh;
				groupList->back().GetMeshList()->push_back(_mesh);

				string name;
				file >> name;
				groupList->back().GetMeshList()->back().SetMaterialName(name);
				groupList->back().GetMeshList()->back().SetOffset((indexBuffer.size()));
			}
			else if (token == "v")
			{
				D3DXVECTOR3 position;
				file >> position.x >> position.y >> position.z;

				if (position.x > modelMax.x) modelMax.x = position.x;
				if (position.y > modelMax.y) modelMax.y = position.y;
				if (position.z > modelMax.z) modelMax.z = position.z;
				if (position.x < modelMin.x) modelMin.x = position.x;
				if (position.y < modelMin.y) modelMin.y = position.y;
				if (position.z < modelMin.z) modelMin.z = position.z;

				if (position.x > groupMax.x) groupMax.x = position.x;
				if (position.y > groupMax.y) groupMax.y = position.y;
				if (position.z > groupMax.z) groupMax.z = position.z;
				if (position.x < groupMin.x) groupMin.x = position.x;
				if (position.y < groupMin.y) groupMin.y = position.y;
				if (position.z < groupMin.z) groupMin.z = position.z;

				if (position.x > meshMax.x) meshMax.x = position.x;
				if (position.y > meshMax.y) meshMax.y = position.y;
				if (position.z > meshMax.z) meshMax.z = position.z;
				if (position.x < meshMin.x) meshMin.x = position.x;
				if (position.y < meshMin.y) meshMin.y = position.y;
				if (position.z < meshMin.z) meshMin.z = position.z;

				positions.push_back(position);
			}
			else if (token == "vt")
			{
				D3DXVECTOR2 texCoord;
				file >> texCoord.x >> texCoord.y;
				texCoord.y = 1.0 - texCoord.y;
				texCoords.push_back(texCoord);
			}
			else if (token == "vn")
			{
				D3DXVECTOR3 normal;
				file >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}
			else if (token[0] == 'f')
			{
				for (unsigned int i = 0; i < 3; i++)
				{
					unsigned int index, positionIndex, texCoordIndex, normalIndex;

					file >> positionIndex;
					file.ignore(INT_MAX, '/');
					file >> texCoordIndex;
					file.ignore(INT_MAX, '/');
					file >> normalIndex;

					OBJvertex vertex;
					vertex.position = positions[positionIndex - 1];
					vertex.texCoord = texCoords[texCoordIndex - 1];
					vertex.normal = normals[normalIndex - 1];

					index = find(vertexBuffer.begin(), vertexBuffer.end(), vertex) - vertexBuffer.begin();

					if (index < vertexBuffer.size())
					{
						indexBuffer.push_back(index);
					}
					else
					{
						vertexBuffer.push_back(vertex);
						indexBuffer.push_back(vertexBuffer.size() - 1);
					}
				}
			}
			file.ignore(INT_MAX, '\n');
		}
		file.close();
	}

	groupList->back().GetMeshList()->back().SetSize((indexBuffer.size() - groupList->back().GetMeshList()->back().GetOffset()));

	D3DXVECTOR3 groupSize = (groupMax - groupMin) / 2;
	groupSize *= -1.0f;
	D3DXVECTOR3 groupPosition = groupMin + groupSize;
	FLOAT groupRadio = max(max(groupSize.x, groupSize.y), groupSize.z);
	groupList->back().SetAABB(new AxisAlignedBoundingBox(groupMin, groupMax));
	groupList->back().SetBS(new BoundingSphere(groupPosition, groupRadio));
	groupList->back().SetBP(new BoundingPoint(groupPosition));
	groupMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	groupMax = D3DXVECTOR3(FLT_MIN, FLT_MIN, FLT_MIN);

	D3DXVECTOR3 size = (meshMax - meshMin) / 2;
	size *= -1.0f;
	D3DXVECTOR3 position = meshMin + size;
	FLOAT radio = max(max(size.x, size.y), size.z);
	groupList->back().GetMeshList()->back().SetAABB(new AxisAlignedBoundingBox(meshMin, meshMax));
	groupList->back().GetMeshList()->back().SetBS(new BoundingSphere(position, radio));
	groupList->back().GetMeshList()->back().SetBP(new BoundingPoint(position));
	meshMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	meshMax = D3DXVECTOR3(FLT_MIN, FLT_MIN, FLT_MIN);

	ComputeTangentSpace(vertexBuffer, indexBuffer);
	groupList->back().SetVertexBuffer(device, &vertexBuffer);
	groupList->back().SetIndexBuffer(device, &indexBuffer);

	vector <Material>* materialList = LoadMtl(device, relativePath, mtl);

	for (auto &group : *groupList)
	{
		for (auto &mesh : *group.GetMeshList())
		{
			string name = mesh.GetMaterialName();
			for (auto &material : *materialList)
			{
				if (material.getName() == name)
				{
					group.GetMaterialList()->push_back(material);
					mesh.SetMaterialIndex(group.GetMaterialList()->size() - 1);
					break;
				}
			}
		}
	}

	min = modelMin;
	max = modelMax;

	return groupList;
}

void Model::ComputeTangentSpace(vector <OBJvertex> &vertices, vector <UINT> &indices)
{
	D3DXVECTOR3 *tangents = new D3DXVECTOR3[vertices.size()];
	D3DXVECTOR3 *binormals = new D3DXVECTOR3[vertices.size()];

	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		D3DXVECTOR3 vertex0 = vertices[indices[i + 0]].position;
		D3DXVECTOR3 vertex1 = vertices[indices[i + 1]].position;
		D3DXVECTOR3 vertex2 = vertices[indices[i + 2]].position;

		D3DXVECTOR3 normal;
		D3DXVec3Cross(&normal, &(vertex1 - vertex0), &(vertex2 - vertex0));

		D3DXVECTOR3 deltaPos;
		if (vertex0.x == vertex1.x && vertex0.y == vertex1.y && vertex0.z == vertex1.z)
		{
			deltaPos = vertex2 - vertex0;
		}
		else
		{
			deltaPos = vertex1 - vertex0;
		}

		D3DXVECTOR2 uv0 = vertices[indices[i + 0]].texCoord;
		D3DXVECTOR2 uv1 = vertices[indices[i + 1]].texCoord;
		D3DXVECTOR2 uv2 = vertices[indices[i + 2]].texCoord;

		D3DXVECTOR2 deltaUV1 = uv1 - uv0;
		D3DXVECTOR2 deltaUV2 = uv2 - uv0;

		D3DXVECTOR3 tan;
		D3DXVECTOR3 bin;

		tan = deltaPos / 1.0f;
		tan = tan - (D3DXVec3Dot(&normal, &tan) * normal);
		D3DXVec3Normalize(&tan, &tan);

		D3DXVec3Cross(&bin, &tan, &normal);
		D3DXVec3Normalize(&bin, &bin);

		tangents[indices[i + 0]] = tan;
		tangents[indices[i + 1]] = tan;
		tangents[indices[i + 2]] = tan;

		binormals[indices[i + 0]] = bin;
		binormals[indices[i + 1]] = bin;
		binormals[indices[i + 2]] = bin;
	}

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		vertices[i].tangent = tangents[i];
		vertices[i].binormal = binormals[i];
	}
}

vector <Material> *Model::LoadMtl(ID3D11Device* device, string relativePath, string fileName)
{
	vector <Material>* materialList = new vector <Material>();
	ifstream file;
	file.open(relativePath + "/" + fileName);
	if (file.is_open())
	{
		while (file.good())
		{
			string token;
			file >> token;
			if (token == "newmtl")
			{
				Material _material;
				materialList->push_back(_material);
				string name;
				file >> name;
				materialList->back().setName(name);
			}
			else if (token == "Ka")
			{
				D3DXVECTOR3 ambient;
				file >> ambient.x >> ambient.y >> ambient.z;
				materialList->back().setAmbient(ambient);
			}
			else if (token == "Kd")
			{
				D3DXVECTOR3 diffuse;
				file >> diffuse.x >> diffuse.y >> diffuse.z;
				materialList->back().setDiffuse(diffuse);
			}
			else if (token == "Ks")
			{
				D3DXVECTOR3 specular;
				file >> specular.x >> specular.y >> specular.z;
				materialList->back().setSpecular(specular);
			}
			else if (token == "Ns")
			{
				unsigned int shininess;
				file >> shininess;
				materialList->back().setShininess(shininess);
			}
			else if (token == "d")
			{
				float alpha;
				file >> alpha;
				materialList->back().setTransparency(alpha);
			}
			else if (token == "illum")
			{
				unsigned int illumination;
				file >> illumination;
				materialList->back().setIllumination(illumination);
			}
			else if (token == "map_Ka")
			{
				string name;
				file >> name;
				materialList->back().setAmbientMap(device, relativePath + "/" + name);
			}
			else if (token == "map_Kd")
			{
				string name;
				file >> name;
				materialList->back().setDiffuseMap(device, relativePath + "/" + name);
			}
			else if (token == "map_Ks")
			{
				string name;
				file >> name;
				materialList->back().setSpecularMap(device, relativePath + "/" + name);
			}
			else if (token == "map_Ns")
			{
				string name;
				file >> name;
				materialList->back().setShininessMap(device, relativePath + "/" + name);
			}
			else if (token == "map_d")
			{
				string name;
				file >> name;
				materialList->back().setAlphaMap(device, relativePath + "/" + name);
			}
			else if (token == "map_bump")
			{
				string name;
				file >> name;
				file >> name;
				file >> name;
				//getline(file, name);
				//name.erase(0, name.find_first_not_of(" "));
				materialList->back().setNormalMap(device, relativePath + "/" + name);
			}
			file.ignore(INT_MAX, '\n');
		}
		file.close();
	}

	return materialList;
}


Collider Model::GetCollider()
{
	return this->collider;
}

AxisAlignedBoundingBox * Model::GetAABB()
{
	return this->aabb;
}

BoundingSphere * Model::GetBS()
{
	return this->bs;
}

BoundingPoint * Model::GetBP()
{
	return this->bp;
}

void Model::SetCollider(Collider collider)
{
	this->collider = collider;
}

void Model::SetAABB(AxisAlignedBoundingBox *aabb)
{
	this->aabb = aabb;
}

void Model::SetBS(BoundingSphere *bs)
{
	this->bs = bs;
}

void Model::SetBP(BoundingPoint *bp)
{
	this->bp = bp;
}
