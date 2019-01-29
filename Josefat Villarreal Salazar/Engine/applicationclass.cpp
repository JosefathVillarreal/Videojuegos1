////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	m_Input = 0;
	m_Direct3D = 0;
	m_Camera = 0;
	m_Terrain = 0;
	m_Timer = 0;
	m_Position = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_FontShader = 0;
	m_Text = 0;
	m_TerrainShader = 0;
	m_Light = 0;
	m_projectileCount = 0;
	m_Camara = 0; 
	m_Camara1 = 0;
	m_TimerDerecha = 0; 
	tiempo = 0, tiempo2 = 0, tiempo1 = 0;
	tiempoCamara = 0, tiempoCamara2 = 0, tiempoCamara1 = 0;
	tiempoDerecha = 0, tiempo2Derecha = 0, tiempo1Derecha = 0;
	factorDeEscalamientoDelTerreno = 5; 
	APresionanda = false, colisionada = false; 
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	float cameraX, cameraY, cameraZ, dx = 0, dy = 0, dz = 0;
	
	char videoCard[128];
	int videoMemory;

	enemigoColisionado = false;
	
	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Set the initial position of the camera.
	cameraX = 32.0f;
	cameraY = 6.0f;
	cameraZ = 51.0f;

	m_Camera->SetPosition(cameraX, cameraY, cameraZ);

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	int textureCount = 5;
	WCHAR** filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/rocas(1).jpg";
	filenames[1] = L"../Engine/data/algas(1).jpg";
	filenames[2] = L"../Engine/data/arena2.jpg";
	filenames[3] = L"../Engine/data/arena1.jpg";
	filenames[4] = L"../Engine/data/alpha1.jpg";

	result = m_Terrain->Initialize(m_Direct3D->GetDevice(), "../Engine/data/heightmap1.bmp", filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain object.
	m_Water = new TerrainClass;
	if (!m_Water)
	{
		return false;
	}

	// Initialize the terrain object.
	textureCount = 3;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/agua.jpg";
	filenames[1] = L"../Engine/data/waterNormal.png";
	filenames[2] = L"../Engine/data/water.png";

	result = m_Water->Initialize(m_Direct3D->GetDevice(), "../Engine/data/waterHeightmap.bmp", filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_Skydome = new SkydomeClass;
	if (!m_Skydome)
	{
		return false;
	}

	textureCount = 2;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/dia.jpg";
	filenames[1] = L"../Engine/data/noche.jpg";

	// Initialize the model object.
	result = m_Skydome->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Billboard[0] = new BillboardClass;
	if (!m_Billboard[0])
	{
		return false;
	}

	textureCount = 2;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/blob.png";
	filenames[1] = L"../Engine/data/blobNormal.png";

	// Initialize the model object.
	result = m_Billboard[0]->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Billboard[1] = new BillboardClass;
	if (!m_Billboard[1])
	{
		return false;
	}

	textureCount = 2;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/alga.png";
	filenames[1] = L"../Engine/data/algaNormal.png";

	// Initialize the model object.
	result = m_Billboard[1]->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Billboard[2] = new BillboardClass;
	if (!m_Billboard[2])
	{
		return false;
	}

	textureCount = 2;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/pulpo.png";
	filenames[1] = L"../Engine/data/pulpoNormal.png";

	// Initialize the model object.
	result = m_Billboard[2]->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Billboard[3] = new BillboardClass;
	if (!m_Billboard[3])
	{
		return false;
	}

	textureCount = 2;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/win.png";
	filenames[1] = L"../Engine/data/win.png";

	// Initialize the model object.
	result = m_Billboard[3]->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Billboard[4] = new BillboardClass;
	if (!m_Billboard[4])
	{
		return false;
	}

	textureCount = 2;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/Failed.png";
	filenames[1] = L"../Engine/data/Failed.png";

	// Initialize the model object.
	result = m_Billboard[4]->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if(!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}


	m_Camara = new TimerClass;
	if (!m_Camara)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Camara->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	m_Camara1 = new TimerClass;
	if (!m_Camara1)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Camara1->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	m_TimerDerecha = new TimerClass;
	if (!m_TimerDerecha)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_TimerDerecha->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new PositionClass;
	if(!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	m_Position->SetPosition(cameraX, m_Terrain->GetHeightAtPosition(cameraX, cameraZ), cameraZ);

	// Create the fps object.
	m_Fps = new FpsClass;
	if(!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if(!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if(!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Retrieve the video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	// Set the video card information in the text object.
	result = m_Text->SetVideoCardInfo(videoCard, videoMemory, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShaderClass;
	if(!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_SkydomeShader = new SkydomeShaderClass;
	if (!m_SkydomeShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_SkydomeShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_WaterShader = new WaterShaderClass;
	if (!m_WaterShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_WaterShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_BillboardShader = new BillboardShaderClass;
	if (!m_BillboardShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_BillboardShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_GroupShader = new GroupShaderClass;
	if (!m_GroupShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_GroupShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_DiffuseShader = new DiffuseShaderClass;
	if (!m_DiffuseShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_DiffuseShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}


	// Create the model object.
	m_Projectile = new SphereClass;
	if (!m_Projectile)
	{
		return false;
	}

	textureCount = 1;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/waterDiffuse.jpg";

	// Initialize the model object.
	result = m_Projectile->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Initialize the light object.
	//m_Light->SetAmbientColor(0.00f, 1.0f, 0.1f, 1.0f);
	m_Light->SetAmbientColor(0.00f, 0.02f, 0.1f, 1.0f);
	m_Light->SetDiffuseColor(0.2f, 0.5f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -1.0f, 1.0f);
	
	//KogMaw (Deep Sea)
	m_Cup = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Bomb Block", "bomba1.obj");
	m_Enemigo = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/KogMaw (Deep Sea)", "enemigo.obj");
	m_Cup1 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/SharpClaw Bomb", "bomba2.obj");
	m_Principal = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Ocean Tornado", "nave.obj");
	m_NaveChida = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Bowser Ship", "Ship.obj");
	m_CuboAdorno[0] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Bucket of the Sea", "cubo.obj");
	m_CuboAdorno[1] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Bucket of the Sea", "cubo.obj");
	m_CuboAdorno[2] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Bucket of the Sea", "cubo.obj");
	m_CuboAdorno[3] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Bucket of the Sea", "cubo.obj");
	m_CuboAdorno[4] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Bucket of the Sea", "cubo.obj");
	m_TablaAdorno[0] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Sea Skim Surfboard", "tabla.obj");
	m_TablaAdorno[1] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Sea Skim Surfboard", "tabla.obj");
	m_TablaAdorno[2] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Sea Skim Surfboard", "tabla.obj");
	m_TablaAdorno[3] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Sea Skim Surfboard", "tabla.obj");
	m_TablaAdorno[4] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Sea Skim Surfboard", "tabla.obj");
	m_TablaAdorno[5] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Sea Skim Surfboard", "tabla.obj");
	m_TablaAdorno[6] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Sea Skim Surfboard", "tabla.obj");
	m_TablaAdorno[7] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Sea Skim Surfboard", "tabla.obj");
	m_TablaAdorno[8] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Sea Skim Surfboard", "tabla.obj");
	m_TablaAdorno[9] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Sea Skim Surfboard", "tabla.obj");
	m_Tortugas[0] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/14480", "tortuga.obj");
	m_Tortugas[1] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/14480", "tortuga.obj");
	m_Tortugas[2] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/14480", "tortuga.obj");
	m_Tortugas[3] = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/14480", "tortuga.obj");
	
	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	for (int i = 0; i < 60; i++)
	{
		x[i] = rand() % 400; 
		z[i] = rand() % 400;
		y[i] = m_Terrain->GetHeightAtPosition(x[i], z[i]); 
	}

	D3DXMatrixTranslation(&inicioPrincipal, cameraX, m_Terrain->GetHeightAtPosition(cameraX, cameraZ) * factorDeEscalamientoDelTerreno, cameraZ);
	//inicioPrincipal


	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the terrain shader object.
	if(m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the font shader object.
	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the cpu object.
	if(m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if(m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object.
	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	if (m_TimerDerecha)
	{
		delete m_TimerDerecha;
		m_TimerDerecha = 0;
	}

	// Release the terrain object.
	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	return;
}


bool ApplicationClass::Frame()
{
	bool result;


	// Read the user input.
	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}
	
	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Update the FPS value in the text object.
	result = m_Text->SetFps(vida, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}
	
	// Update the CPU usage value in the text object.
	result = m_Text->SetCpu(30 - m_projectileCount, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Do the frame input processing.
	result = HandleInput(m_Timer->GetTime());
	if(!result)
	{
		return false;
	}

	// Render the graphics.
	result = RenderGraphics();
	if(!result)
	{
		return false;
	}

	return result;
}


bool ApplicationClass::HandleInput(float frameTime)
{
	bool keyDown, result, type;
	float posX, posY, posZ, rotX, rotY, rotZ;
	
	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	//if (!bloqueadoParaSiempre) {
		m_Input->GetMouseLocation(mouseX, mouseY);
		//mouseXAnterior = mouseX;
		//mouseYAnterior = mouseY;
	//}
	/*else {
		mouseX = mouseXAnterior;
		mouseY = mouseYAnterior;
	}*/
	

	// Handle the input.
	if (!bloqueadoParaSiempre) {

	keyDown = m_Input->IsAPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsDPressed();
	m_Position->TurnRight(keyDown);
	m_Camera->SetRotation(mouseY, mouseX, 0);

	if (!colisionada){
	keyDown = m_Input->IsWPressed();
	m_Position->MoveForward(keyDown);
	}

	keyDown = m_Input->IsSPressed();
	m_Position->MoveBackward(keyDown);

	}
	keyDown = m_Input->IsAPressed();
	m_Position->MoveUpward(keyDown);
	m_Camera->SetRotation(mouseY, mouseX, 0);

	keyDown = m_Input->IsZPressed();
	m_Position->MoveDownward(keyDown);
	if (keyDown) {
		if (bloqueadoParaSiempre) {

			m_Position->SetPosition(32.0, m_Terrain->GetHeightAtPosition(32.0, 51.0), 51.0);
			//principalWorldMatrix = inicioPrincipal;

			bloqueadoParaSiempre = false;
			perdiste = false; 
			ganaste = false; 
			bala1 = false; 
			bala2 = false;
			m_projectileCount = 0; 
			enemigoColisionado = false; 
			vida = 100; 

		}
		
	}

	keyDown = m_Input->IsPgUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = m_Input->IsPgDownPressed();
	m_Position->LookDownward(keyDown);
	
	keyDown = m_Input->IsCPressed();
	if (keyDown){
		tiempoCamara = m_Camara1->GetTicks();
		tiempoCamara2 = tiempoCamara - tiempoCamara1;
		
		if (tiempoCamara2 > 500){
			m_Position->ChangeCameraType(keyDown);
			tiempoCamara1 = tiempoCamara;
		}
	}

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);
	m_Position->GetCameraType(type);

	// Set the position of the camera.
	
	m_Terrain->GetHeightAtPosition(posX, posZ, posY);

	// Update the position values in the text object.
	/*result = m_Text->SetCameraPosition(posX, posY, posZ, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}*/

	//// Update the rotation values in the text object.
	//result = m_Text->SetCameraRotation(rotX, rotY, rotZ, m_Direct3D->GetDeviceContext());
	//if(!result)
	//{
	//	return false;
	//}

	keyDown = m_Input->IsLeftClickPressed();

	APresionanda = m_Input->IsSpacePressed();
	
	//aquí lo que hace es que al picarle recibe el numero de milisegundos que ya pasaron
	if (keyDown)
	{
		if (bala1) {

		
		//aquí se le resta con el anterior para saber cuántos milisegundos han pasado desde que se picó por ultima vez
			tiempo = m_Camara->GetTicks();
			tiempo2 = tiempo - tiempo1;
			//entonces si ha pasado más de medio segundo se agrega una bala a la pila
			if (tiempo2 > 500){
				if (m_projectileCount<30) {

				
				m_projectileCount++;

				m_projectilePos.push_back(D3DXVECTOR4(posX, posY, posZ,0));
				m_projectileRot.push_back(D3DXVECTOR3(rotX, rotY, rotZ));
				//se actualiza el tiempo para poder seguir actualizando el timer
				tiempo1 = tiempo;
				}
			}

		}
		
	}
	
	if (APresionanda){
		if (bala2) {
			if (m_projectileCount < 30) {
				m_projectileCount++;

				m_projectilePos.push_back(D3DXVECTOR4(posX, posY, posZ, 1));
				m_projectileRot.push_back(D3DXVECTOR3(rotX, rotY, rotZ));
			}
		}
	}

	keyDown = m_Input->IsRightClickPressed();

	if (keyDown)
	{
		//aquí se le resta con el anterior para saber cuántos milisegundos han pasado desde que se picó por ultima vez
		tiempo = m_TimerDerecha->GetTicks();
		tiempo2Derecha = tiempoDerecha - tiempo1Derecha;

		//entonces si ha pasado más de medio segundo se agrega una bala a la pila
		if (tiempo2Derecha > 100) {
			if (m_projectileCount < 30) {
				m_projectileCount++;

				m_projectilePos.push_back(D3DXVECTOR4(posX, posY, posZ, 1));
				m_projectileRot.push_back(D3DXVECTOR3(rotX, rotY, rotZ));
				//se actualiza el tiempo para poder seguir actualizando el timer
				tiempo1Derecha = tiempoDerecha;
			}
		}

		
	}

	m_Input->GetMouseLocation(mouseX , mouseY);

	m_Camera->SetPosition(posX, (m_Terrain->GetHeightAtPosition(posX, posZ) * factorDeEscalamientoDelTerreno) +5, posZ);
	m_Camera->SetRotation(mouseY, mouseX, 0);
	m_Camera->SetCameraType(type);
	

	return true;
}


bool ApplicationClass::RenderGraphics()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, terrainWorld, terrainScale;
	bool result;
	D3DXMATRIX worldwater, watertrans, waterscale, worldnaveChida, naveChidatrans, naveChidascale,
		cubotrans1, cubotrans2, cubotrans3, cubotrans4, cubotrans5;
	static D3DXMATRIX tempprincipalWorldMatrix, principalWorldMatrix, EnemigoWorld, transEnemigo;
	//D3DXMATRIX transEnemigo, EnemigoWorld;

	if (ejebool) {
		if (eje < 360) {
			eje = eje + 0.01;
		}
		else {
			ejebool = false;
			//eje--;
		}
	}
	else {
		if (eje > 0) {
			eje = eje - 0.01;
		}
		else {
			ejebool = true;
			//eje--;
		}
	}

	float dx, dy, dz;
	float speedX, speedZ;
	float lenght;
	float vel = 0.01;
	D3DXVECTOR3 positionc = m_Camera->GetPosition();

	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	//m_Direct3D->TurnZBufferOff();

	m_Skydome->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	result = m_SkydomeShader->Render(m_Direct3D->GetDeviceContext(), m_Skydome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Skydome->GetTexture(), m_Skydome->GetTextureCount());
	if (!result)
	{
		return false;
	}

	//m_Direct3D->TurnZBufferOn();

	// Render the terrain buffers.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());

	D3DXMatrixScaling(&terrainScale, 1, factorDeEscalamientoDelTerreno, 1);
	terrainWorld = terrainScale;

	// Render the terrain using the terrain shader.
	result = m_TerrainShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), terrainWorld, viewMatrix, projectionMatrix,
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Terrain->GetTextures(), m_Terrain->GetTextureCount());
	if (!result)
	{
		return false;
	}

	// Render the terrain buffers.

	if (!ganaste) {

		if (perdiste) {

			m_Billboard[4]->Render(m_Direct3D->GetDeviceContext());

			// Render the terrain using the terrain shader.
			result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[4]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(principalWorldMatrix._41, (m_Terrain->GetHeightAtPosition(principalWorldMatrix._41, principalWorldMatrix._43) * factorDeEscalamientoDelTerreno) + 4, principalWorldMatrix._43)), viewMatrix, projectionMatrix,
				m_Billboard[4]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
			if (!result)
			{
				return false;
			}
		}
	}

	if (ganaste) {

		m_Billboard[3]->Render(m_Direct3D->GetDeviceContext());

		// Render the terrain using the terrain shader.
		result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[3]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(principalWorldMatrix._41, (m_Terrain->GetHeightAtPosition(principalWorldMatrix._41, principalWorldMatrix._43) * factorDeEscalamientoDelTerreno) + 4, principalWorldMatrix._43)), viewMatrix, projectionMatrix,
			m_Billboard[3]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
		if (!result)
		{
			return false;
		}
	}

	for (int i = 0; i < 20; i++) {

		// Render the terrain buffers.
		m_Billboard[0]->Render(m_Direct3D->GetDeviceContext());

		// Render the terrain using the terrain shader.
		result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[0]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(x[i], m_Terrain->GetHeightAtPosition(x[i], z[i])*factorDeEscalamientoDelTerreno, z[i])), viewMatrix, projectionMatrix,
			m_Billboard[0]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
		if (!result)
		{
			return false;
		}


		// Render the terrain buffers.
		m_Billboard[1]->Render(m_Direct3D->GetDeviceContext());

		// Render the terrain using the terrain shader.
		result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[1]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(x[i + 20], m_Terrain->GetHeightAtPosition(x[i + 20], z[i + 20])*factorDeEscalamientoDelTerreno, z[i + 20])), viewMatrix, projectionMatrix,
			m_Billboard[1]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
		if (!result)
		{
			return false;
		}

		// Render the terrain buffers.
		m_Billboard[2]->Render(m_Direct3D->GetDeviceContext());

		// Render the terrain using the terrain shader.
		result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[2]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(x[i + 40], m_Terrain->GetHeightAtPosition(x[i + 40], z[i + 40])*factorDeEscalamientoDelTerreno, z[i + 40])), viewMatrix, projectionMatrix,
			m_Billboard[2]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
		if (!result)
		{
			return false;
		}

	}

	D3DXMATRIX trans, scale, cupWorldMatrix;
	D3DXMatrixTranslation(&trans, 81, m_Terrain->GetHeightAtPosition(81, 154) * factorDeEscalamientoDelTerreno, 154);
	//D3DXMatrixScaling(&scale, 5, 5, 5);
	cupWorldMatrix = trans;
	if (!bala1) {

		m_Cup->UpdateColliders(cupWorldMatrix);
		for (auto &group : *m_Cup->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), cupWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}

	D3DXMatrixTranslation(&cubotrans1, 46, m_Terrain->GetHeightAtPosition(46, 112) * factorDeEscalamientoDelTerreno, 112);
	//D3DXMatrixScaling(&scale, 5, 5, 5);
	//cupWorldMatrix = cubotrans1;
	m_CuboAdorno[0]->UpdateColliders(cubotrans1);
	for (auto &group : *m_CuboAdorno[0]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), cubotrans1, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixTranslation(&cubotrans2, 70, m_Terrain->GetHeightAtPosition(70, 193) * factorDeEscalamientoDelTerreno, 193);
	//D3DXMatrixScaling(&scale, 5, 5, 5);
	//cupWorldMatrix = cubotrans1;
	m_CuboAdorno[1]->UpdateColliders(cubotrans2);
	for (auto &group : *m_CuboAdorno[1]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), cubotrans2, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixTranslation(&cubotrans3, 182, m_Terrain->GetHeightAtPosition(182, 252) * factorDeEscalamientoDelTerreno, 252);
	//D3DXMatrixScaling(&scale, 5, 5, 5);
	//cupWorldMatrix = cubotrans1;
	m_CuboAdorno[2]->UpdateColliders(cubotrans3);
	for (auto &group : *m_CuboAdorno[2]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), cubotrans3, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixTranslation(&cubotrans4, 165, m_Terrain->GetHeightAtPosition(165, 371) * factorDeEscalamientoDelTerreno, 371);
	//D3DXMatrixScaling(&scale, 5, 5, 5);
	//cupWorldMatrix = cubotrans1;
	m_CuboAdorno[3]->UpdateColliders(cubotrans3);
	for (auto &group : *m_CuboAdorno[3]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), cubotrans4, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}


	D3DXMatrixTranslation(&cubotrans5, 12, m_Terrain->GetHeightAtPosition(12, 351) * factorDeEscalamientoDelTerreno, 351);
	//D3DXMatrixScaling(&scale, 5, 5, 5);
	//cupWorldMatrix = cubotrans1;
	m_CuboAdorno[4]->UpdateColliders(cubotrans3);
	for (auto &group : *m_CuboAdorno[4]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), cubotrans5, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}


	D3DXMatrixTranslation(&cubotrans5, 12, m_Terrain->GetHeightAtPosition(12, 351) * factorDeEscalamientoDelTerreno, 351);
	//D3DXMatrixScaling(&scale, 5, 5, 5);
	//cupWorldMatrix = cubotrans1;
	m_CuboAdorno[4]->UpdateColliders(cubotrans3);
	for (auto &group : *m_CuboAdorno[4]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), cubotrans5, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX tortugatrans1, tortugatrans2, tortugatrans3, tortugatrans4, tortugatrans5, tortugatrans6, tortugatrans7,
		tortugatrans8, tortugatrans9, tortugatrans10, tortugatrans11, tortugatrans12, tortugatrans13, tortugatrans14, tortugatrans15,
		tortuga1world, tablatrans1, tablatrans2, tablatrans3, tablatrans4, tablatrans5, tablatrans6, tablatrans7, tablatrans8, tablatrans9, tablatrans10;

#pragma region tortugas

	D3DXMatrixTranslation(&tortugatrans1, 25 + cos(eje), (m_Terrain->GetHeightAtPosition(25 + cos(eje), 275 + sin(eje)) * factorDeEscalamientoDelTerreno) + 5 + (sin(eje)) * 2, 275 + sin(eje));
	//D3DXMatrixScaling(&scale, 5, 5, 5);
	//D3DXMatrixRotationYawPitchRoll(&tortugarotate, eje/6, 0,0);
	//cupWorldMatrix = cubotrans1;
	tortuga1world = tortugatrans1;// *tortugarotate;
	if (!tortuga1) {

	m_Tortugas[0]->UpdateColliders(tortuga1world);
	for (auto &group : *m_Tortugas[0]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), tortuga1world, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}
	}

	D3DXMatrixTranslation(&tortugatrans2, 50 + cos(eje), (m_Terrain->GetHeightAtPosition(50 + cos(eje), 300 + sin(eje)) * factorDeEscalamientoDelTerreno) + 5 + (sin(eje)) * 2, 300 + sin(eje));
	if (!tortuga2) {
		m_Tortugas[1]->UpdateColliders(tortugatrans2);
		for (auto &group : *m_Tortugas[1]->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), tortugatrans2, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}
	D3DXMatrixTranslation(&tortugatrans3, 75 + cos(eje), (m_Terrain->GetHeightAtPosition(75 + cos(eje), 325 + sin(eje)) * factorDeEscalamientoDelTerreno) + 5 + (sin(eje)) * 2, 325 + sin(eje));

	if (!tortuga3) {
		m_Tortugas[2]->UpdateColliders(tortugatrans3);
		for (auto &group : *m_Tortugas[2]->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), tortugatrans3, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}
	D3DXMatrixTranslation(&tortugatrans4, 100 + cos(eje), (m_Terrain->GetHeightAtPosition(100 + cos(eje), 350 + sin(eje)) * factorDeEscalamientoDelTerreno) + 5 + (sin(eje)) * 2, 350 + sin(eje));

	if (!tortuga4) {
		m_Tortugas[3]->UpdateColliders(tortugatrans4);
		for (auto &group : *m_Tortugas[3]->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), tortugatrans4, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}
#pragma endregion

#pragma region tablas

	D3DXMatrixTranslation(&tablatrans1, 70, (m_Terrain->GetHeightAtPosition(70, 70) * factorDeEscalamientoDelTerreno) - 2, 70);
	//D3DXMatrixScaling(&scale, 5, 5, 5);
	//cupWorldMatrix = cubotrans1;
	m_TablaAdorno[0]->UpdateColliders(tablatrans1);
	for (auto &group : *m_TablaAdorno[0]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), tablatrans1, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixTranslation(&tablatrans2, 24, (m_Terrain->GetHeightAtPosition(24, 82) * factorDeEscalamientoDelTerreno) - 10, 82);
	m_TablaAdorno[1]->UpdateColliders(tablatrans2);
	for (auto &group : *m_TablaAdorno[1]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), tablatrans2, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixTranslation(&tablatrans3, 39, (m_Terrain->GetHeightAtPosition(39, 259) * factorDeEscalamientoDelTerreno) - 2, 259);
	m_TablaAdorno[2]->UpdateColliders(tablatrans2);
	for (auto &group : *m_TablaAdorno[2]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), tablatrans3, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixTranslation(&tablatrans4, 79, (m_Terrain->GetHeightAtPosition(79, 339) * factorDeEscalamientoDelTerreno) - 2, 339);
	m_TablaAdorno[3]->UpdateColliders(tablatrans2);
	for (auto &group : *m_TablaAdorno[3]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), tablatrans4, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixTranslation(&tablatrans5, 8, (m_Terrain->GetHeightAtPosition(8, 339) * factorDeEscalamientoDelTerreno) - 5, 339);
	m_TablaAdorno[4]->UpdateColliders(tablatrans5);
	for (auto &group : *m_TablaAdorno[4]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), tablatrans5, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixTranslation(&tablatrans6, 8, (m_Terrain->GetHeightAtPosition(7, 385) * factorDeEscalamientoDelTerreno) - 2, 385);
	m_TablaAdorno[5]->UpdateColliders(tablatrans6);
	for (auto &group : *m_TablaAdorno[5]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), tablatrans6, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixTranslation(&tablatrans7, 103, (m_Terrain->GetHeightAtPosition(103, 387) * factorDeEscalamientoDelTerreno) - 2, 387);
	m_TablaAdorno[6]->UpdateColliders(tablatrans6);
	for (auto &group : *m_TablaAdorno[6]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), tablatrans7, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixTranslation(&tablatrans8, 258, (m_Terrain->GetHeightAtPosition(258, 318) * factorDeEscalamientoDelTerreno) - 2, 318);
	m_TablaAdorno[7]->UpdateColliders(tablatrans6);
	for (auto &group : *m_TablaAdorno[7]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), tablatrans8, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixTranslation(&tablatrans9, 322, (m_Terrain->GetHeightAtPosition(322, 233) * factorDeEscalamientoDelTerreno) - 2, 233);
	m_TablaAdorno[8]->UpdateColliders(tablatrans6);
	for (auto &group : *m_TablaAdorno[8]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), tablatrans9, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixTranslation(&tablatrans10, 372, (m_Terrain->GetHeightAtPosition(372, 158) * factorDeEscalamientoDelTerreno) - 2, 158);
	m_TablaAdorno[9]->UpdateColliders(tablatrans6);
	for (auto &group : *m_TablaAdorno[9]->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), tablatrans10, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

#pragma endregion

	D3DXMatrixTranslation(&naveChidatrans, 73, m_Terrain->GetHeightAtPosition(73, 380) * factorDeEscalamientoDelTerreno, 380);
	//D3DXMatrixScaling(&scale, 5, 5, 5);
	worldnaveChida = naveChidatrans;

	if (enemigoColisionado) {
		m_NaveChida->UpdateColliders(worldnaveChida);
		for (auto &group : *m_NaveChida->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), worldnaveChida, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}

	m_NaveChida->UpdateColliders(worldnaveChida);

	D3DXMATRIX trans1, scale1, cupWorldMatrix1;
	D3DXMatrixTranslation(&trans1, 311, m_Terrain->GetHeightAtPosition(311, 182) * factorDeEscalamientoDelTerreno, 182);
	D3DXMatrixScaling(&scale1, 5, 5, 5);
	cupWorldMatrix1 = scale1 * trans1;

	if (!bala2) {
		m_Cup1->UpdateColliders(cupWorldMatrix1);
		for (auto &group : *m_Cup1->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), cupWorldMatrix1, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}


	if (vida < 10) {
		D3DXMatrixTranslation(&transEnemigo, 43 + sin(eje) * 12 + xmueve, m_Terrain->GetHeightAtPosition(43 + sin(eje) * 12 + xmueve, 362 + zmueve) * factorDeEscalamientoDelTerreno, 362 + zmueve);
		EnemigoWorld = transEnemigo;
	}
	else {
		D3DXMatrixTranslation(&transEnemigo, 43 + sin(eje) * 12, m_Terrain->GetHeightAtPosition(43 + sin(eje) * 12, 362) * factorDeEscalamientoDelTerreno, 362);
		EnemigoWorld = transEnemigo;

	}
	//D3DXMatrixScaling(&scale1, 5, 5, 5);
	//EnemigoWorld = transEnemigo;
	//m_Enemigo

	if (!enemigoColisionado) {
		m_Enemigo->UpdateColliders(EnemigoWorld);
		for (auto &group : *m_Enemigo->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), EnemigoWorld, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}

		m_Enemigo->UpdateColliders(EnemigoWorld);
	}
	D3DXMATRIX transla, rotat;

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 rotation = D3DXVECTOR3(0, 0, 0);
	float yaw, pitch, roll;

	m_Position->GetPosition(position.x, position.y, position.z);
	m_Position->GetRotation(rotation.x, rotation.y, rotation.z);
	position.y = m_Terrain->GetHeightAtPosition(position.x, position.z);
	position.y = (position.y * factorDeEscalamientoDelTerreno) + 3;
	pitch = rotation.x * 0.0174532925f;
	yaw = (rotation.y + 180.0) * 0.0174532925f;
	roll = rotation.z * 0.0174532925f;

	//m_Terrain->GetHeightAtPosition(position.x, position.z, position.y * factorDeEscalamientoDelTerreno);

	D3DXMatrixTranslation(&transla, position.x, position.y, position.z);
	D3DXMatrixRotationYawPitchRoll(&rotat, yaw, 0, 0);

	tempprincipalWorldMatrix = rotat * transla;

	m_Principal->UpdateColliders(tempprincipalWorldMatrix);


	//aquí se checa la colisión con los 2 vasos, el de nivel del suelo y el otro

	if (!m_Tortugas[0]->Collides(m_Cup1) && !m_Tortugas[1]->Collides(m_Cup1) && !m_Tortugas[2]->Collides(m_Cup1) && !!m_Tortugas[3]->Collides(m_Cup1)) {
		//principalWorldMatrix = tempprincipalWorldMatrix;
		colisionada = false;
		//ganaste = false;

	}
	else {
		if (m_Tortugas[0]->Collides(m_Cup1)) {
			tortuga1 = true; 
		}
		if (m_Tortugas[1]->Collides(m_Cup1)) {
			tortuga2 = true;
		}
		if (m_Tortugas[2]->Collides(m_Cup1)) {
			tortuga3 = true;
		}
		if (m_Tortugas[3]->Collides(m_Cup1)) {
			tortuga4 = true;
		}
	}

	if (!m_Principal->Collides(m_Enemigo)) {


		if (!m_Principal->Collides(m_Cup) && !m_Principal->Collides(m_Cup1) && !m_Principal->Collides(m_NaveChida) && !m_Principal->Collides(m_Tortugas[0]) && !m_Principal->Collides(m_Tortugas[1]) && !m_Principal->Collides(m_Tortugas[2]) && !m_Principal->Collides(m_Tortugas[3])) {
			principalWorldMatrix = tempprincipalWorldMatrix;
			colisionada = false;
			ganaste = false;
			
		}
		else
		{
			tempprincipalWorldMatrix = tempprincipalWorldMatrix;
			colisionada = true;

			if (m_Principal->Collides(m_Cup)) {
				bala1 = true; 
			}

			if (m_Principal->Collides(m_Cup1)) {
				bala2 = true;
			}

			if (enemigoColisionado) {
				if (m_Principal->Collides(m_NaveChida)) {
					if (tortuga1 || tortuga2 || tortuga3 || tortuga4) {

					ganaste = true;
					bloqueadoParaSiempre = true; 
					
					//m_Position->SetPosition(32.0, m_Terrain->GetHeightAtPosition(32.0, 51.0), 51.0);
					//principalWorldMatrix = inicioPrincipal;

					//ganaste = false;
					//colisionada = false;
					}
				}
			}

			if (m_Principal->Collides(m_Tortugas[0]) || !m_Principal->Collides(m_Tortugas[1]) || !m_Principal->Collides(m_Tortugas[2]) || !m_Principal->Collides(m_Tortugas[3])) {
			
				//entonces si ha pasado más de medio segundo se agrega una bala a la pila
					if (vida > 1) {
						vida = vida - 1;
					}
					
			}
		}

		for (auto &group : *m_Principal->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), principalWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}
	else {

		if (enemigoColisionado) {
			
		}
		else {
			perdiste = true;
			bloqueadoParaSiempre = true;
			m_Position->SetPosition(32.0, m_Terrain->GetHeightAtPosition(32.0, 51.0), 51.0);
			principalWorldMatrix = inicioPrincipal;
		}
		
	}

	m_Projectile->Render(m_Direct3D->GetDeviceContext());

	if (bala1 || bala2) {
		
		for (int i = 0; i < m_projectileCount; i++)
		{
			D3DXMATRIX rotatPro, proWorldMatrix;

			D3DXVECTOR3 move = D3DXVECTOR3(0, 0, 0.5);

			pitch = m_projectileRot[i].x * 0.0174532925f;
			yaw = m_projectileRot[i].y * 0.0174532925f;
			roll = m_projectileRot[i].z * 0.0174532925f;

			D3DXMatrixRotationYawPitchRoll(&rotatPro, yaw, 0, 0);

			D3DXVECTOR4 transformed;
			D3DXVec3Transform(&transformed, &move, &rotatPro);

			m_projectilePos[i].x += transformed.x;
			m_projectilePos[i].y += transformed.y;
			m_projectilePos[i].z += transformed.z;

			D3DXMatrixTranslation(&proWorldMatrix, m_projectilePos[i].x + (tiempo2 / 1000), m_projectilePos[i].y * factorDeEscalamientoDelTerreno, m_projectilePos[i].z);

			if (m_projectilePos[i].w == 0) {

				m_Cup->UpdateColliders(proWorldMatrix);
				for (auto &group : *m_Cup->GetGroupList())
				{
					group.Render(m_Direct3D->GetDeviceContext());
					result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), proWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
					if (!result)
					{
						return false;
					}
				}
			}
			else {
				m_Cup1->UpdateColliders(proWorldMatrix);
				for (auto &group : *m_Cup1->GetGroupList())
				{
					group.Render(m_Direct3D->GetDeviceContext());
					result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), proWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
					if (!result)
					{
						return false;
					}
				}
			}

			if (!m_Enemigo->Collides(m_Cup) && !m_Enemigo->Collides(m_Cup1)) {
				//principalWorldMatrix = tempprincipalWorldMatrix;
				//colisionada = false;
				//enemigoColisionado = false;

			}
			else {
			//	perdiste = true;
				enemigoColisionado = true;
				EnemigoWorld = proWorldMatrix;
				//->UpdateColliders(EnemigoWorld);
			}


			/*for (int i = 0; i < m_projectileCount; i++)
			{
				m_Cup1->UpdateColliders(proWorldMatrix);
				for (auto &group : *m_Cup1->GetGroupList())
				{
					group.Render(m_Direct3D->GetDeviceContext());
					result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), proWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
					if (!result)
					{
						return false;
					}
				}
			}*/
		}
	}
	
	D3DXMatrixScaling(&waterscale, 8, 1, 8);
	D3DXMatrixTranslation(&watertrans, 0, 50, 0);
	worldwater = watertrans * waterscale; 
	// Render the terrain buffers.
	m_Water->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	result = m_WaterShader->Render(m_Direct3D->GetDeviceContext(), m_Water->GetIndexCount(), worldwater, viewMatrix, projectionMatrix,
		m_Water->GetTextures(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(0.9, 0.3, 0.3, 1), 10);

	if (!result)
	{
		return false;
	}

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Render the text user interface elements.
	result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_FontShader, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	#pragma region seguir
	//	//Seguimiento de Enemigo 

	//xmueve = principalWorldMatrix._41 - transEnemigo._41;
	//zmueve = principalWorldMatrix._43 - transEnemigo._43;
	//	//for (int i = 0; i < eleCount; i++) {
	//		//if (inArea[i]) {
	//			//m_Camera->GetPosition()
	//	dx = principalWorldMatrix._41 - EnemigoWorld._41; 
	//	//dx = playerPos.x - elephantPos1[i].x;
	//	dy = principalWorldMatrix._42 - EnemigoWorld._42;
	//	dz = principalWorldMatrix._43 - EnemigoWorld._43;
	//	lenght = atan2(zmueve, xmueve);
		//speedX = vel;//; * cos(lenght);
		//speedZ = vel;//;  * sin(lenght);

		//if (principalWorldMatrix._41 != transEnemigo._41 || principalWorldMatrix._43 != transEnemigo._43) {
			
			//transEnemigo._41 += vel;
			//transEnemigo._43 += vel;
			//transEnemigo._42 = m_Terrain->GetHeightAtPosition(transEnemigo._41, transEnemigo._43) * factorDeEscalamientoDelTerreno;

			//D3DXMatrixTranslation(&transEnemigo, transEnemigo._41 += vel, m_Terrain->GetHeightAtPosition(transEnemigo._41 += vel, transEnemigo._43 +=vel) * factorDeEscalamientoDelTerreno, transEnemigo._43 += vel);
	//		/*if (elephantPos1[i].y < playerPos.y - 2)
	//			elephantPos1[i].y += 0.1;
	//		else
	//			elephantPos1[i].y -= 0.1;*/
			//EnemigoWorld = transEnemigo; 
	//	}
	//	//}
	////}
	//
	//
#pragma endregion

	// Turn off alpha blending after rendering the text.
	//m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}