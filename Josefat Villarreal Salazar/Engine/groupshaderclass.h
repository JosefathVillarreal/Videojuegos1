////////////////////////////////////////////////////////////////////////////////
// Filename: terrainshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELSHADERCLASS_H_
#define _MODELSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include "Group.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainShaderClass
////////////////////////////////////////////////////////////////////////////////
class GroupShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct LightBufferType
	{
		struct
		{
			D3DXVECTOR4 ambient;
			D3DXVECTOR4 diffuse;
			D3DXVECTOR4 specular;
			D3DXVECTOR4 direction;
		} light;
	};

	struct CameraBufferType
	{
		D3DXVECTOR3 cameraPosition;
		FLOAT padding;
	};

	struct MaterialBufferType
	{
		struct
		{
			D3DXVECTOR3 ambient;
			FLOAT padding1;
			D3DXVECTOR3 diffuse;
			FLOAT padding2;
			D3DXVECTOR3 specular;
			FLOAT padding3;

			FLOAT alpha;
			D3DXVECTOR3 padding4;
			unsigned int shininess;
			D3DXVECTOR3 padding5;
			unsigned int illumination;
			D3DXVECTOR3 padding6;

		} material;

		unsigned int ambient_map;
		D3DXVECTOR3 padding5;
		unsigned int diffuse_map;
		D3DXVECTOR3 padding6;
		unsigned int specular_map;
		D3DXVECTOR3 padding7;
		unsigned int shininess_map;
		D3DXVECTOR3 padding8;
		unsigned int alpha_map;
		D3DXVECTOR3 padding9;
		unsigned int normal_map;
		D3DXVECTOR3 padding10;
	};
public:
	GroupShaderClass();
	GroupShaderClass(const GroupShaderClass&);
	~GroupShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR3, Group);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR3);
	void RenderShader(ID3D11DeviceContext*, int, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_materialBuffer;
};

#endif