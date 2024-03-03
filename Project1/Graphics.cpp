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
	// 리소스와 렌더링 파이프라인을 묶어주기 위해 _deviceContext를 사용해야한다.
	// IA(Input Assembler) - VS(Vertex Shader) - RS(Rasterizer Stage) - PS(Pixel Shader) - OM(Output Merger)
	// 접두사를 통해서 어떤 Stage에 작업을 요청했는지 알 수 있다.

	// 여기에다가 그림을 그려주세요!
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);

	// 초기 색은 _clearColor 색상이에요!
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);

	// 우리가 바라볼 화면 정보에요.
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Graphics::RenderEnd() {
	// Present: 제출하다.
	// [ 후면 ] --> [ 전면 ]
	// 후면 버퍼에 입력한 것들을 전면 버퍼에 복사해준 후 화면에 보여달라!
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Graphics::CreateDeviceAndSwapChain() {
	// 구조체가 정말 복잡하다
	// 이걸 타고 들어가면 또 구조체가 나온다. 처음 공부할 땐 그렇게 하나하나 파고들진 말자.
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		// 버퍼 크기는 당연히 화면크기와 동일하게 맞춰주는게 합리적이다.
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;

		// RefreshRate: 화면 주사율과 관련된 옵션
		desc.BufferDesc.RefreshRate.Numerator = 60;		
		desc.BufferDesc.RefreshRate.Denominator = 1;

		// 전면/후면 버퍼의 각 픽셀의 색상 컬러의 포맷을 의미한다.
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// 일단 넘어가자.
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		// 멀티 샘플링을 사용안한다면 1로 설정
		// 샘플링은 우리가 도형을 만들거나 특정 텍스쳐를 출력할 때 선이 픽셀단위로 표현되기 때문에
		// 우리가 원하는데로 정확한 선이 그려지지 않고 계단 형식으로 그려지게 된다. 이런현상을 줄이기 위해서.
		// 주변 색상과 보간해서 출력할 수 있도록 
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;


		// 이 버퍼는 어떻게 사용할 것인가? 최종적으로 화면을 그릴 때 결과물의 용도로 사용하겠다.
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
	// 이 옵션을 주게 되면 D3D 오류 발생시 Output Window에 오류 메시지 출력을 해줌
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// ComPtr<T>::Get: T*를 얻을 때 사용한다.
	// ComPtr<T>::GetAddressOf: T**를 얻을 때 사용한다.

	// https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdeviceandswapchain
	// 더블 포인터를 받는걸 봐서 _device, _deviceContext, _swapChain을 모두 채워준다는걸 알 수 있다.
	// 매우 중요한 함수라는 것!
	HRESULT hr =  ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,	// 하드웨어인 그래픽 카드를 사용하겠다.
		nullptr,
		creationFlags,
		nullptr,					// 9, 10, 11에 해당하는 기능을 지원해야한다. 라는 
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

	// 스왑체인에서 0번째 백버퍼를 찾아서 pBackBuffer에 저장한다.
	// __uuidof는 COM 오브젝트는 타입 저마다 id가 있는데 그 정보를 얻을 때 사용하는거다.
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
