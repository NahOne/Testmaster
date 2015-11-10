#ifndef H_CONTEXT_H
#define H_CONTEXT_H
#include <Windows.h>
#include <d3d11.h>

class CContextManager
{
public:
	CContextManager();
	~CContextManager();
	int TakeRenderTarget();
	int Init(HWND hWnd, UINT width_application, UINT height_application);
	ID3D11Device* GetD3DDevice(){return m_D3DDevice;};
	ID3D11DeviceContext* GetDeviceContext(){return m_DeviceContext;};
	IDXGISwapChain* GetSwapChain(){return m_SwapChain;};
	ID3D11RenderTargetView* GetRenderTargetView(){return m_RenderTargetView;};
	void Render();	
	int CreateBackBuffer(HWND hWnd, UINT width, UINT height);
	
	
private:
	ID3D11Device *m_D3DDevice; // esta clase, el device, nos sirve para crear objetos de DirectX
	ID3D11DeviceContext *m_DeviceContext; // el contexto nos va a servir para usar objetos de DirectX
	IDXGISwapChain *m_SwapChain; // la cadena de swap
	ID3D11RenderTargetView* m_RenderTargetView;
	

	
};

#endif
