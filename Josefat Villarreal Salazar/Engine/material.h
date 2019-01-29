#pragma once

#include <d3d11.h>
#include <d3dx11tex.h>
#include <d3dx10math.h>
#include <string>

using namespace std;

class Material
{
private:
	string name;

	D3DXVECTOR3 ambient;
	D3DXVECTOR3 diffuse;
	D3DXVECTOR3 specular;

	unsigned int shininess;
	float alpha;

	unsigned int illumination;

	ID3D11ShaderResourceView *ambient_map;
	ID3D11ShaderResourceView *diffuse_map;
	ID3D11ShaderResourceView *specular_map;
	ID3D11ShaderResourceView *shininess_map;
	ID3D11ShaderResourceView *alpha_map;
	ID3D11ShaderResourceView *normal_map;
	
public:

	Material();
	~Material();

	void setName(string name);
	void setAmbient(D3DXVECTOR3 ambient);
	void setDiffuse(D3DXVECTOR3 diffuse);
	void setSpecular(D3DXVECTOR3 specular);
	void setShininess(unsigned int shininess);
	void setTransparency(float alpha);
	void setIllumination(unsigned int illum);
	void setAmbientMap(ID3D11Device* device, string name);
	void setDiffuseMap(ID3D11Device* device, string name);
	void setSpecularMap(ID3D11Device* device, string name);
	void setShininessMap(ID3D11Device* device, string name);
	void setAlphaMap(ID3D11Device* device, string name);
	void setNormalMap(ID3D11Device* device, string name);

	string getName();
	D3DXVECTOR3 getAmbient();
	D3DXVECTOR3 getDiffuse();
	D3DXVECTOR3 getSpecular();
	unsigned int getShininess();
	float getTransparency();
	unsigned int getIllumination();

	ID3D11ShaderResourceView *getAmbientMap();
	ID3D11ShaderResourceView *getDiffuseMap();
	ID3D11ShaderResourceView *getSpecularMap();
	ID3D11ShaderResourceView *getShininessMap();
	ID3D11ShaderResourceView *getAlphaMap();
	ID3D11ShaderResourceView *getNormalMap();
};

