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

	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
				NULL, 0, &feature_level, 1, D3D11_SDK_VERSION, &swap_chain_desc,
								&m_swapChain, &m_device, nullptr, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}
}
