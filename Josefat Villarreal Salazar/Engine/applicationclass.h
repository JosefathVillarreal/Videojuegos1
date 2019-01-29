////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "terrainclass.h"
#include "timerclass.h"
#include "positionclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "fontshaderclass.h"
#include "textclass.h"
#include "terrainshaderclass.h"
#include "watershaderclass.h"
#include "lightclass.h"
#include "skydomeClass.h"
#include "skydomeshaderclass.h"
#include "billboardclass.h"
#include "billboardshaderclass.h"
#include "model.h"
#include "groupshaderclass.h"
#include "sphereclass.h"
#include "diffuseshaderclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool HandleInput(float);
	bool RenderGraphics();
	bool APresionanda, colisionada;


	int m_projectileCount, factorDeEscalamientoDelTerreno;
	vector <D3DXVECTOR4> m_projectilePos;
	vector <D3DXVECTOR3> m_projectileRot;
	INT64 tiempo, tiempo1, tiempo2;
	INT64 tiempoDerecha, tiempo1Derecha, tiempo2Derecha;
	INT64 tiempoCamara, tiempoCamara1, tiempoCamara2;
	bool enemigoColisionado; 
	float x[60], y[60], z[60]; 
	int vida = 100;
	float xmueve = 0, ymueve = 0, zmueve = 0;

private:
	InputClass* m_Input;
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	TerrainClass* m_Terrain, *m_Water;
	TimerClass* m_Timer, *m_Camara, *m_Camara1, *m_TimerDerecha;
	PositionClass* m_Position;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	FontShaderClass* m_FontShader;
	TextClass* m_Text;
	TerrainShaderClass* m_TerrainShader;
	SkydomeShaderClass* m_SkydomeShader;
	WaterShaderClass* m_WaterShader;
	BillboardShaderClass* m_BillboardShader;
	LightClass* m_Light;
	ModelClass* m_Skydome, *m_Billboard[5], *m_Projectile;
	DiffuseShaderClass *m_DiffuseShader;
	Model* m_Principal, *m_Cup, *m_Cup1, *m_Enemigo, *m_NaveChida, *m_CuboAdorno[5], *m_Tortugas[4], *m_TablaAdorno[10];
	GroupShaderClass* m_GroupShader;
	D3DXMATRIX baseViewMatrix, inicioPrincipal;
	bool ganaste = false, perdiste = false, bala1 = false, bala2 = false, bloqueadoParaSiempre = false;
	float eje = 0; 
	bool ejebool = false, tortuga1 = false, tortuga2 = false, tortuga3 = false, tortuga4 = false;
	int mouseX, mouseY, mouseXAnterior, mouseYAnterior;
};

#endif