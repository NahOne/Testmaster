#include <Windows.h>
#include <d3d11.h>
#include "ContextManager.h"
#include "Application.h"
#include "DebugRender.h"
#include "InputManagerImplementation.h"


#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "libxml2.lib")

#define APPLICATION_NAME	"VIDEOGAMETEST"
#define WIDTH_APPLICATION 800
#define HEIGHT_APPLICATION 600


CContextManager g_context;

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

  switch( msg )
  {
	case WM_DESTROY:
		{
		  PostQuitMessage( 0 );
		  return 0;
		}
		break;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED)
		{
			g_context.Resize(hWnd, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
		}
		return 0;
		break;
  }//end switch( msg )

  return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
  float l_PreviousTime, l_ElapsedTime;
  DWORD l_CurrentTime = timeGetTime();
  l_PreviousTime = l_CurrentTime;
	// Register the window class
  WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

  RegisterClassEx( &wc );

  // Calcular el tamano de nuestra ventana
	RECT rc = {
			0, 0, WIDTH_APPLICATION, HEIGHT_APPLICATION
		};
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE);


  // Create the application's window
  HWND hWnd = CreateWindow( APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wc.hInstance, NULL);


  // TODO Crear el contexto DIRECTX
  //CreateContext( hWnd, WIDTH_APPLICATION, HEIGHT_APPLICATION);
  
  g_context.CreateContext(hWnd, WIDTH_APPLICATION, HEIGHT_APPLICATION);
  g_context.CreateBackBuffer(hWnd, WIDTH_APPLICATION, HEIGHT_APPLICATION);

    // Añadir aquí el Init de la applicacioón
  ShowWindow( hWnd, SW_SHOWDEFAULT );

  g_context.InitStates();
  CDebugRender debugRender(g_context.GetDevice());
  CApplication application(&debugRender, &g_context);





  // TODO Crear el back buffer
  //CreateBackBuffer( hWnd, rc.right - rc.left, rc.bottom - rc.top );
  //context->CreateBackBuffer(hWnd, rc.right - rc.left, rc.bottom - rc.top );
 // EffectStruct shaders = context->LoadDebugEffect();
//  context->PrepararBuffer(shaders);
  CInputManagerImplementation *l_inputManager = new CInputManagerImplementation();
  CInputManager::SetCurrentInputManager(l_inputManager);
  l_inputManager->LoadCommandsFromFile("Data\\input.xml");
 



  UpdateWindow( hWnd );
  MSG msg;
  ZeroMemory( &msg, sizeof(msg) );


    // Añadir en el while la condición de salida del programa de la aplicación

  while( msg.message != WM_QUIT )
  {
    if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		switch (msg.message)
		{
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYDOWN:
			case WM_KEYUP:
				if (!l_inputManager->KeyEventReceived(msg.wParam, msg.lParam))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				break;
			default:
				TranslateMessage(&msg);
				DispatchMessage(&msg);
		}

	}
    else
    {
	   // Main loop: Añadir aquí el Update y Render de la aplicación principal
	   // TODO
		l_CurrentTime = timeGetTime();
		l_ElapsedTime = (float)(l_CurrentTime - l_PreviousTime);
		l_PreviousTime = (float) l_CurrentTime;
		l_inputManager->BeginFrame();
		l_inputManager->EndFrame();
		application.Update(l_ElapsedTime);

		application.Render();
    }
  }
  UnregisterClass( APPLICATION_NAME, wc.hInstance );

  // Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos
//  delete context;
  delete l_inputManager;
  return 0;
}