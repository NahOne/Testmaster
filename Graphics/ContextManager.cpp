#include "ContextManager.h"


#pragma comment(lib,"d3d11.lib")
//*CContextManager CContextManager::m_instanciaUnica = 0;

CContextManager::CContextManager()
{
	m_D3DDevice=nullptr; // esta clase, el device, nos sirve para crear objetos de DirectX
	m_DeviceContext=nullptr; // el contexto nos va a servir para usar objetos de DirectX
	m_SwapChain=nullptr; // la cadena de swap 
}


CContextManager::~CContextManager()
{
	m_D3DDevice->Release();
	m_DeviceContext->Release();
	m_SwapChain->Release();
	m_RenderTargetView->Release();
}

int CContextManager::Init(HWND hWnd, UINT width_application, UINT height_application)
{
	// Tendremos que crear y rellenar una estructura de este tipo
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	// o
	//DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Windowed = TRUE;
	// TODO:
	desc.BufferDesc.Width=width_application;
	desc.BufferDesc.Height=height_application;
	desc.BufferDesc.RefreshRate.Numerator=1;
	desc.BufferDesc.RefreshRate.Denominator=60;
	desc.OutputWindow=hWnd;
	desc.SampleDesc.Count=1;
	desc.SampleDesc.Quality=0;
	//desc. ????

	// Que DirectX queremos
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);


	
	/*
	
	D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
	UINT               numLevelsRequested = 1;
	D3D_FEATURE_LEVEL  FeatureLevelsSupported;

		if( FAILED (hr = D3D11CreateDeviceAndSwapChain( NULL, 
					D3D_DRIVER_TYPE_HARDWARE, 
					NULL, 
					0,
					&FeatureLevelsRequested, 
					numFeatureLevelsRequested, 
					D3D11_SDK_VERSION, 
					&sd, 
					&g_pSwapChain, 
					&g_pd3dDevice, 
					&FeatureLevelsSupported,
					&g_pImmediateContext )))
		{
		return hr;
	}*/

	HRESULT hr;
	if (FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &desc, &m_SwapChain, &m_D3DDevice, NULL, &m_DeviceContext)))
	{
		return S_FALSE;
	}
	return hr;
}

void CContextManager::Render()
{
	// Limpiar el framebuffer:
   float color[4] = {1,0.5,0,1};
   m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, color);
   //m_DeviceContext->ClearDepthStencilView();

// definir a que target vamos a pintar:
   m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, nullptr);

// definir el viewport:
   D3D11_VIEWPORT viewport={0,0,800,600,0,1};
   m_DeviceContext->RSSetViewports(1, &viewport);
	   
// TODO renderizar aqui

// presentar lo pintado:
   m_SwapChain->Present(0,0);

}

int CContextManager::CreateBackBuffer(HWND hWnd, UINT width, UINT height)
{
	ID3D11Texture2D* l_DepthStencil;
	ID3D11DepthStencilView*	l_DepthStencilView;
		
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	// TODO:
	//desc.Width;
	//desc.Height;
	//desc.MipLevels;   <- 1
	//desc.ArraySize;   <- 1
	//desc.SampleDesc.Count;
	//desc.SampleDesc.Quality;
	HRESULT hr = m_D3DDevice->CreateTexture2D(&desc, NULL, &l_DepthStencil);
	if (FAILED(hr))
		return hr;
			
	D3D11_DEPTH_STENCIL_VIEW_DESC desc2;
	desc2.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	desc2.Texture2D.MipSlice = 0;
	// TODO
	//desc.Format;
	hr = m_D3DDevice->CreateDepthStencilView(l_DepthStencil, &desc2, &l_DepthStencilView);
	if (FAILED(hr))
		return hr;
	
}

int CContextManager::TakeRenderTarget()
{
	ID3D11Texture2D *pBackBuffer;
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
		return FALSE;
	HRESULT hr = m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView);
	pBackBuffer->Release();
}


/*{
	float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
			+0.0f, +0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
			+0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,
		};


	ID3D11Buffer* l_VertexBuffer;

	D3D11_BUFFER_DESC desc = {}
	desc.Usage=D3D11_USAGE_DEFAULT;
	desc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	// TODO
	// desc.ByteWidth;
	// desc.CPUAccessFlags;
	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem=vertices;
	// ID3D11DeviceContext::CreateBuffer
}

/***************************************************************************************
**  				Finalmente podremos pintar dicho buffer       		              **
***************************************************************************************/
/*{
	static CEffectParameters Parameters;
	
	Parameters.m_World.SetIdentity();
	Parameters.m_View.SetIdentity();
	Parameters.m_Projection.SetIdentity();
	Parameters.m_BaseColor = CColor(1,1,1,1);

	//   ID3D11DeviceContext::IASetVertexBuffers;
	//   ID3D11DeviceContext::IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//   ID3D11DeviceContext::IASetInputLayout;
	//   ID3D11DeviceContext::VSSetShader;
	//   ID3D11DeviceContext::UpdateSubresource(  ConstantBuffer  , 0, NULL, &Parameters, 0, 0 );
	//   ID3D11DeviceContext::VSSetConstantBuffers;
	//   ID3D11DeviceContext::PSSetShader;

	//   ID3D11DeviceContext::Draw(3, 0);
}*/