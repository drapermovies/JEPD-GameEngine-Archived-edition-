#include "d3dclass.h"

bool D3DClass::Initialize(int screen_width, int screen_height, bool vsync,
	HWND hwnd, bool fullscreen, float screen_depth,
	float screen_near)
{
	HRESULT result;
	IDXGIFactory* factory = nullptr;
	IDXGIAdapter* adapter = nullptr;
	IDXGIOutput* adapter_output = nullptr;
	unsigned int num_modes = 0, numerator = 0, denominator = 0;
	unsigned long long string_length;
	DXGI_MODE_DESC* display_mode_list = nullptr;
	DXGI_ADAPTER_DESC adapter_desc;
	int error = -1;
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	D3D_FEATURE_LEVEL feature_level;
	ID3D11Texture2D* back_buffer_ptr = nullptr;
	D3D11_TEXTURE2D_DESC depth_buffer_desc;
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
	D3D11_RASTERIZER_DESC raster_desc;
	D3D11_VIEWPORT viewport;
	float field_of_view = 0, screen_aspect = 0;

	m_vsync_enabled = vsync;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)& factory);
	if (FAILED(result))
	{
		return false;
	}

	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	result = adapter->EnumOutputs(0, &adapter_output);
	if (FAILED(result))
	{
		return false;
	}

	result = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &num_modes, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	display_mode_list = new DXGI_MODE_DESC[num_modes];
	if (!display_mode_list)
	{
		return false;
	}

	result = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list);
	if (FAILED(result))
	{
		return false;
	}

	for (int i = 0; i < num_modes; i++)
	{
		if (display_mode_list[i].Width == (unsigned int)screen_width)
		{
			if (display_mode_list[i].Height == (unsigned int)screen_height)
			{
				numerator = display_mode_list[i].RefreshRate.Numerator;
				denominator = display_mode_list[i].RefreshRate.Denominator;
			}
		}
	}

	result = adapter->GetDesc(&adapter_desc);
	if (FAILED(result))
	{
		return false;
	}

	m_videoCardMemory = (int)(adapter_desc.DedicatedVideoMemory / 1024 / 1024);

	/*error = wcstombs_s(&string_length, m_videoCardDescription, 128, adapter_desc.Description, 128);
	if (error != 0)
	{
		return false;
	}*/

	delete[] display_mode_list;
	display_mode_list = nullptr;

	adapter_output->Release();
	adapter_output = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));

	swap_chain_desc.BufferCount = 1;

	swap_chain_desc.BufferDesc.Width = screen_width;
	swap_chain_desc.BufferDesc.Height = screen_height;

	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (m_vsync_enabled)
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = numerator;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swap_chain_desc.OutputWindow = hwnd;

	//Multi Sampling OFF
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;

	if (fullscreen)
	{
		swap_chain_desc.Windowed = false;
	}
	else
	{
		swap_chain_desc.Windowed = true;
	}

	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swap_chain_desc.Flags = 4;

	feature_level = D3D_FEATURE_LEVEL_11_0;

	//Swap Chain & DirectX Device
	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		NULL, 0, &feature_level, 1, D3D11_SDK_VERSION, &swap_chain_desc,
		&m_swapChain, &m_device, nullptr, &m_deviceContext);
	if (FAILED(result)) { return false; }

	//Back Buffer Pointer
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& back_buffer_ptr);
	if (FAILED(result)) { return false; }

	result = m_device->CreateRenderTargetView(back_buffer_ptr, nullptr, &m_renderTargetView);
	if (FAILED(result)) { return false; }

	back_buffer_ptr->Release();
	back_buffer_ptr = nulptr;

	//Depth Buffer
	ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));

	depth_buffer_desc.Width = screen_width;
	depth_buffer_desc.Height = screen_height;
	depth_buffer_desc.MipLevels = 1;
	depth_buffer_desc.ArraySize = 1;
	depth_buffer_desc.format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_buffer_desc.SampleDesc.Count = 1;
	depth_buffer_desc.SampleDesc.Quality = 0;
	depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_buffer_desc.CPUAccessFlags = 0;
	depth_buffer_desc.MiscFlags = 0;

	result = m_device->CreateTexture2D(&depth_buffer_desc, nullptr, &m_depthStencilBuffer);
	if (FAILED(result)) { return false; }

	//Depth Stencil State
	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));

	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilReadMask = 0xFF;
	depth_stencil_desc.StencilWriteMask = 0xFF;

	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(&depth_stencil_desc, &m_depthStencilState);
	if (FAILED(result)) { return false; }

	m_deviceContex->OMSetDepthStencilState(m_depthStencilState, 1);

	//Depth Stencil View
	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_view_desc));

	depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;

	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depth_stencil_view_desc, &m_depthStencilView);
	if (FAILED(result)) { return false; }

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	//Raster Description
	raster_desc.AntialiasedLineEnable = false;
	raster_desc.CullMode = D3D11_CULL_BACK;
	raster_desc.DepthBias = 0;
	raster_desc.DepthBiasClamp = 0.0f;
	raster_desc.DepthClipEnable = true;
	raster_desc.FillMode = D3D11_FILL_SOLID;
	raster_desc.FrontCounterClockwise = false;
	raster_desc.MultisampleEnable = false;
	raster_desc.ScissorEnable = false;
	raster_desc.SlopeScaledDepthBias = 0.0f;

	result = m_device->CreateRasterizerState(&raster_desc, &m_rasterState);
	if (FAILED(result)) { return false; }

	m_deviceContext->RSSetState(m_rasterState);
}
