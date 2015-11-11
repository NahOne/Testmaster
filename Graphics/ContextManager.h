#ifndef H_CONTEXT_H
#define H_CONTEXT_H
#include <Windows.h>
#include <d3d11.h>
#include "Math/Matrix44.h"


struct CEffectParameters
{
public:
	Mat44f				m_World;
	Mat44f				m_View;
	Mat44f				m_Projection;
	Vect4f				m_BaseColor;
	Vect4f				m_CameraRightVector;
	Vect4f				m_CameraUpVector;
	float				m_DebugRenderScale;

	uint8_t				offset[9];
};

struct EffectStruct
{
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* VertexLayout;
	ID3D11Buffer* ConstantBuffer;
};

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
	D3D11_BUFFER_DESC GetBufferDescription(){ return m_BufferDescription; };
	void Render();	
	int CreateBackBuffer(HWND hWnd, UINT width, UINT height);
	EffectStruct LoadDebugEffect();
	void PrepararBuffer(EffectStruct shader);
	void PintarBuffer(EffectStruct shader);
	
private:
	ID3D11Device *m_D3DDevice; // esta clase, el device, nos sirve para crear objetos de DirectX
	ID3D11DeviceContext *m_DeviceContext; // el contexto nos va a servir para usar objetos de DirectX
	IDXGISwapChain *m_SwapChain; // la cadena de swap
	ID3D11RenderTargetView* m_RenderTargetView;
	D3D11_BUFFER_DESC m_BufferDescription;
	ID3D11Buffer *m_VertexBuffer;

	D3D11_BUFFER_DESC m_desc = {};
	D3D11_SUBRESOURCE_DATA m_InitData = {};
	

	
};

#endif
