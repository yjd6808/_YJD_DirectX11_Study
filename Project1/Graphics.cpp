#include "pch.h"
#include "Graphics.h"

Graphics::Graphics(HWND hwnd) {
	_hwnd = hwnd;
	_width = GMinSizeX;
	_height = GMinSizeY;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewPort();
}

Graphics::~Graphics() {
}

void Graphics::RenderBegin() {
	// ���ҽ��� ������ ������������ �����ֱ� ���� _deviceContext�� ����ؾ��Ѵ�.
	// IA(Input Assembler) - VS(Vertex Shader) - RS(Rasterizer Stage) - PS(Pixel Shader) - OM(Output Merger)
	// ���λ縦 ���ؼ� � Stage�� �۾��� ��û�ߴ��� �� �� �ִ�.

	// ���⿡�ٰ� �׸��� �׷��ּ���!
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);

	// �ʱ� ���� _clearColor �����̿���!
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);

	// �츮�� �ٶ� ȭ�� ��������.
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Graphics::RenderEnd() {
	// Present: �����ϴ�.
	// [ �ĸ� ] --> [ ���� ]
	// �ĸ� ���ۿ� �Է��� �͵��� ���� ���ۿ� �������� �� ȭ�鿡 �����޶�!
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Graphics::CreateDeviceAndSwapChain() {
	// ����ü�� ���� �����ϴ�
	// �̰� Ÿ�� ���� �� ����ü�� ���´�. ó�� ������ �� �׷��� �ϳ��ϳ� �İ���� ����.
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		// ���� ũ��� �翬�� ȭ��ũ��� �����ϰ� �����ִ°� �ո����̴�.
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;

		// RefreshRate: ȭ�� �ֻ����� ���õ� �ɼ�
		desc.BufferDesc.RefreshRate.Numerator = 60;		
		desc.BufferDesc.RefreshRate.Denominator = 1;

		// ����/�ĸ� ������ �� �ȼ��� ���� �÷��� ������ �ǹ��Ѵ�.
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// �ϴ� �Ѿ��.
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		// ��Ƽ ���ø��� �����Ѵٸ� 1�� ����
		// ���ø��� �츮�� ������ ����ų� Ư�� �ؽ��ĸ� ����� �� ���� �ȼ������� ǥ���Ǳ� ������
		// �츮�� ���ϴµ��� ��Ȯ�� ���� �׷����� �ʰ� ��� �������� �׷����� �ȴ�. �̷������� ���̱� ���ؼ�.
		// �ֺ� ����� �����ؼ� ����� �� �ֵ��� 
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;


		// �� ���۴� ��� ����� ���ΰ�? ���������� ȭ���� �׸� �� ������� �뵵�� ����ϰڴ�.
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;

		desc.Windowed = true;
		desc.OutputWindow = _hwnd;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	// https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-devices-layers
	// If the project is in a debug build, enable the debug layer.
	// �� �ɼ��� �ְ� �Ǹ� D3D ���� �߻��� Output Window�� ���� �޽��� ����� ����
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// ComPtr<T>::Get: T*�� ���� �� ����Ѵ�.
	// ComPtr<T>::GetAddressOf: T**�� ���� �� ����Ѵ�.

	// https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdeviceandswapchain
	// ���� �����͸� �޴°� ���� _device, _deviceContext, _swapChain�� ��� ä���شٴ°� �� �� �ִ�.
	// �ſ� �߿��� �Լ���� ��!
	HRESULT hr =  ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,	// �ϵ������ �׷��� ī�带 ����ϰڴ�.
		nullptr,
		creationFlags,
		nullptr,					// 9, 10, 11�� �ش��ϴ� ����� �����ؾ��Ѵ�. ��� 
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	CHECK(hr);
}

void Graphics::CreateRenderTargetView() {
	HRESULT hr;
	ID3D11Texture2D* pBackBuffer = nullptr;

	// ����ü�ο��� 0��° ����۸� ã�Ƽ� pBackBuffer�� �����Ѵ�.
	// __uuidof�� COM ������Ʈ�� Ÿ�� ������ id�� �ִµ� �� ������ ���� �� ����ϴ°Ŵ�.
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	CHECK(hr);

	hr = _device->CreateRenderTargetView(pBackBuffer, nullptr, _renderTargetView.GetAddressOf());
	pBackBuffer->Release();
	CHECK(hr);
}

void Graphics::SetViewPort() {
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
}
