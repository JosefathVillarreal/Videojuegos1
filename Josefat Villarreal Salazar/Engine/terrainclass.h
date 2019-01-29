////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct HeightMapType
	{
		float x, y, z;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float tu2, tv2;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR2 texture2;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool Initialize(ID3D11Device*, char*, WCHAR**, int);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView** GetTextures();
	int GetTextureCount();

	bool GetHeightAtPosition(float positionX, float positionZ, float& height);
	float GetHeightAtPosition(float positionX, float positionZ);

private:
	bool LoadHeightMap(char*);
	void ShutdownHeightMap();
	void ReduceHeightMap();
	bool CalculateNormals();

	bool BuildTerrainModel();
	void ReleaseTerrainModel();

	void CalculateTerrainVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, WCHAR**, int);
	void ReleaseTextures();

private:
	int m_terrainWidth, m_terrainHeight;
	HeightMapType* m_heightMap;
	ModelType* m_TerrainModel;
	int m_vertexCount, m_indexCount, m_textureCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	TextureClass **m_Textures;
};

#endif