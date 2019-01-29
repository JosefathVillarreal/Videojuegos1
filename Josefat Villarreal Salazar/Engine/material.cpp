#include "material.h"

Material::Material()
{
	name = "";

	ambient = D3DXVECTOR3(0, 0, 0);
	diffuse = D3DXVECTOR3(0, 0, 0);
	specular = D3DXVECTOR3(0, 0, 0);

	shininess = 1;
	alpha = 1.0;

	illumination = 0;

	ambient_map = NULL;
	diffuse_map = NULL;
	specular_map = NULL;
	shininess_map = NULL;
	alpha_map = NULL;
	normal_map = NULL;
}

Material::~Material() {}

void Material::setName(string name) {
	this->name = name;
}

void Material::setAmbient(D3DXVECTOR3 ambient) {
	this->ambient = ambient;
}

void Material::setDiffuse(D3DXVECTOR3 diffuse) {
	this->diffuse = diffuse;
}

void Material::setSpecular(D3DXVECTOR3 specular) {
	this->specular = specular;
}

void Material::setShininess(unsigned int shininess) {
	this->shininess = shininess;
}

void Material::setTransparency(float alpha) {
	this->alpha = alpha;
}

void Material::setIllumination(unsigned int illum)
{
	this->illumination = illum;
}

void Material::setAmbientMap(ID3D11Device* device, string name) {
	wstring wname = wstring(name.begin(), name.end());
	D3DX11CreateShaderResourceViewFromFile(device, wname.c_str(), NULL, NULL, &ambient_map, NULL);
}

void Material::setDiffuseMap(ID3D11Device* device, string name) {
	wstring wname = wstring(name.begin(), name.end());
	D3DX11CreateShaderResourceViewFromFile(device, wname.c_str(), NULL, NULL, &diffuse_map, NULL);
}

void Material::setSpecularMap(ID3D11Device* device, string name) {
	wstring wname = wstring(name.begin(), name.end());
	D3DX11CreateShaderResourceViewFromFile(device, wname.c_str(), NULL, NULL, &specular_map, NULL);
}

void Material::setShininessMap(ID3D11Device* device, string name) {
	wstring wname = wstring(name.begin(), name.end());
	D3DX11CreateShaderResourceViewFromFile(device, wname.c_str(), NULL, NULL, &shininess_map, NULL);
}

void Material::setAlphaMap(ID3D11Device* device, string name) {
	wstring wname = wstring(name.begin(), name.end());
	D3DX11CreateShaderResourceViewFromFile(device, wname.c_str(), NULL, NULL, &alpha_map, NULL);
}

void Material::setNormalMap(ID3D11Device* device, string name) {
	wstring wname = wstring(name.begin(), name.end());
	D3DX11CreateShaderResourceViewFromFile(device, wname.c_str(), NULL, NULL, &normal_map, NULL);
}

string Material::getName()
{
	return this->name;
}

D3DXVECTOR3 Material::getAmbient()
{
	return this->ambient;
}

D3DXVECTOR3 Material::getDiffuse()
{
	return this->diffuse;
}

D3DXVECTOR3 Material::getSpecular()
{
	return this->specular;
}

unsigned int Material::getShininess()
{
	return this->shininess;
}

float Material::getTransparency()
{
	return this->alpha;
}

unsigned int Material::getIllumination()
{
	return this->illumination;
}

ID3D11ShaderResourceView *Material::getAmbientMap()
{
	return this->ambient_map;
}

ID3D11ShaderResourceView *Material::getDiffuseMap()
{
	return this->diffuse_map;
}

ID3D11ShaderResourceView *Material::getSpecularMap()
{
	return this->specular_map;
}

ID3D11ShaderResourceView *Material::getShininessMap()
{
	return this->shininess_map;
}

ID3D11ShaderResourceView *Material::getAlphaMap()
{
	return this->alpha_map;
}

ID3D11ShaderResourceView *Material::getNormalMap()
{
	return this->normal_map;
}


