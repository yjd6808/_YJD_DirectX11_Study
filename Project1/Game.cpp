/*
 * 작성자: 윤정도
 * 생성일: 2/3/2024 9:23:38 PM
 * =====================
 *
 */


#include "pch.h"
#include "Game.h"


Game::Game() {
	
}

Game::~Game() {
	
}

void Game::Init(HWND hwnd) {
	_hwnd = hwnd;
	_width = GMinSizeX;
	_height = GMinSizeY;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewPort();

	CreateGeometry();		// 도형을 만든다.
	CreateVS();				
	CreateInputLayout();
	CreatePS();

	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();
	CreateSRV();

	CreateConstantBuffer();
}

void Game::Render() {

	// 준비작업을 먼저한다.
	RenderBegin();

	// 여기서 여러가지 물체들을 그린다.
	{
		// IA단계에서 할일
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		// 렌더링 파이프라인 그림에서 VertexBuffer를 IA와 연결하는 단계를 수행해준다.
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		// VertexBuffer는 데이터 쪼가리일 뿐이므로 inputLayout으로 어떻게 묘사되어있는지 알려줘야한다.
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		// 정점을 어떻게 연결할지, 보통 삼각형으로 연결하므로 거의 항상 D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST를 전달한다.
		_deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// VS단계에서 할일
		// 우리가 셰이더 파일 읽어와서 메모리에 들고있는데 이 데이터를 가지고 작업을 진행해줘
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
		_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());


		// RS단계에서 할일
		_deviceContext->RSSetState(_rasterizerState.Get());

		// PS단계에서 할일
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
		_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

		// OM
		_deviceContext->OMSetBlendState(_blendState.Get(), nullptr, 0xffffffff);

		// 물체를 그려달라고 요청한다.
		// _deviceContext->Draw(_vertices.size(), 0);
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}

	// 그림을 다 그렸으니 제출한다.
	RenderEnd();
}

void Game::Update() {

	// _transformData.offset.x += 0.003f;
	// _transformData.offset.y += 0.003f;

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	// Map으로 뚜겅을 연다. 이제 constantBuffer에 데이터를 쓸 수 있다.
	// constantBuffer의 메모리 위치를 의미하는 녀석이 subResource이다.
	_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

	// CPU -> GPU 데이터쓰기
	::memcpy(subResource.pData, &_transformData, sizeof(TransformData));

	// Unamp으로 뚜껑을 닫는다. 더이상 수정 불가능하다.
	_deviceContext->Unmap(_constantBuffer.Get(), 0);
}

void Game::RenderBegin() {

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

void Game::RenderEnd() {

	// Present: 제출하다.
	// [ 후면 ] --> [ 전면 ]
	// 후면 버퍼에 입력한 것들을 전면 버퍼에 복사해준 후 화면에 보여달라!
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);


}

void Game::CreateDeviceAndSwapChain() {

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
	// If the project is in a debug build, enable the debug layer.
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

void Game::CreateRenderTargetView() {
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

void Game::SetViewPort() {
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
}

void Game::CreateGeometry() {
	HRESULT hr;
	// 버텍스 정보
	{
		constexpr float UV = 2.0f;

		_vertices.resize(4);

		// 정점 설정 순서는 반시계 방향
		// 13
		// 02
		_vertices[0].Position = { -0.5f, -0.5f, 0.0f };
		// _vertices[0].Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		_vertices[0].UV = { 0.0f, UV };

		_vertices[1].Position = { -0.5, 0.5f, 0 };
		// _vertices[1].Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		_vertices[1].UV = { 0.0f, 0.0f };

		_vertices[2].Position = { 0.5f, -0.5f, 0 };
		// _vertices[2].Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		_vertices[2].UV = { UV, UV };

		_vertices[3].Position = { 0.5f, 0.5f, 0 };
		// _vertices[3].Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		_vertices[3].UV = { UV, 0.0f };
	}
	

	// 메모리 데이터를 GPU에 메모리에 전달하기 위함
	// 버텍스 버퍼
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		// 이 버퍼는 어떤 용도로 사용할 것인가?
		// D3D11_USAGE_DEFAULT: GPU만 읽고 쓸 수 있다.
		// D3D11_USAGE_IMMUTABLE: GPU만 읽을 수 있다.
		// D3D11_USAGE_DYNAMIC: CPU는 쓸 수 있고 GPU는 읽을수만 있다.
		// immutable로 세팅한 이유는 우리가 처음 다이렉트X를 초기화하면서 도형을 버퍼에 기록해놓고 중간에 변경하지 않을 것이기 때문
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 우리가 이 버퍼를 버텍스 버퍼로 사용하겠다는 용도를 적어준다.
		desc.ByteWidth = sizeof(Vertex) * _vertices.size(); // 버텍스 버퍼의 크기를 알려준다.

		// _vertexBuffer로 어떤 버퍼의 데이터를 복사해올 것인가?
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data(); // _vertices의 데이터를 복사해주기위함

		// desc와 data를 바탕으로 GPU가 다루는 버텍스 버퍼를 생성한다.
		hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		CHECK(hr);
	}


	// 인덱스 정보
	{
		_indices = { 0, 1, 2, 2, 1, 3 };	// 시계 방향을 골랐으면, 똑같이 유지해줘야한다.
	}

	// 인덱스 버퍼
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;	// 우리가 이 버퍼를 버텍스 버퍼로 사용하겠다는 용도를 적어준다.
		desc.ByteWidth = sizeof(uint32) * _indices.size(); // 버텍스 버퍼의 크기를 알려준다.

		// _vertexBuffer로 어떤 버퍼의 데이터를 복사해올 것인가?
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data(); // _vertices의 데이터를 복사해주기위함

		// desc와 data를 바탕으로 GPU가 다루는 버텍스 버퍼를 생성한다.
		hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		CHECK(hr);
	}
}

void Game::CreateInputLayout() {

	// GPU에서 봤을 때는 아직 VertexBuffer정보도 데이터쪼가리일 뿐이다.
	// 어떤 녀석인지 묘사해주기 위함.

	// Vertex::Position 멤버가 12바이트를 차지하고 있으므로 Color는 12
	D3D11_INPUT_ELEMENT_DESC layouts[] = {
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA },
		// { "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA }
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 12,	D3D11_INPUT_PER_VERTEX_DATA }
	};
	const int32 count = sizeof(layouts) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	_device->CreateInputLayout(layouts, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}

void Game::CreateVS() {

	// Default.hlsl을 컴파일해서 _vsBlob에 담아준다
	LoadShaderFromFile(L"default.hlsl", "VS", "vs_5_0", _vsBlob);
	HRESULT hr = _device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreatePS() {
	// Default.hlsl을 컴파일해서 _psBlob에 담아준다
	LoadShaderFromFile(L"default.hlsl", "PS", "ps_5_0", _psBlob);
	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreateRasterizerState() {

	// https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_rasterizer_desc
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// 3개다 중요함.
	// D3D11_FILL_SOLID		-> 내부를 채우겠다.
	// D3D11_FILL_WIREFRAME -> 내부를 채우지 않는 용도로 사용할때 사용
	desc.FillMode = D3D11_FILL_SOLID;

	// D3D11_CULL_FRONT		-> 앞면을 컬링하겠다.
	// D3D11_CULL_BACK		-> 후면을 컬링하겠다.
	// 컬링은 보통 그리지 않게끔 스킵하는 걸 말한다. (카메라 영역을 벗어나는걸 안그린는 것도 컬링이고, 오클루젼 컬링, 무슨 컬링 다양하게 많다.)
	desc.CullMode = D3D11_CULL_BACK;	

	// desc.CullMode의 앞면과 후면은 어떻게 판단하는 것인가?에 대한 설명이다.
	// 우리가 정점을 연결할 때 정점의 인덱스를 반시계 방향 혹은 시계 방향으로 연결할 수가 있었다.
	// 그리고 연결한 정점은 삼각형의 구조를 가진다.
	// 
	// 이 변수를 true로  설정하면 반시계 방향으로 연결한 정점들로 구성된 삼각형들이 앞면임을 의미한다.
	// 이 변수를 false로 설정하면   시계 방향으로 연결한 정점들로 구성된 삼각형들이 후면임을 의미한다.
	desc.FrontCounterClockwise = false;

	// 그외에 궁금한 것들은 문서를 찾아보면 된다.

	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}

void Game::CreateSamplerState() {

	// https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/ns-d3d11-d3d11_sampler_desc?redirectedfrom=MSDN
	// SamplerState는 UV 좌표를 1을 넘어서는 부분을 어떻게 채워줄지 정해주는 녀석이라고 생각하면 된다.
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.BorderColor[0] = 1;	// R
	desc.BorderColor[1] = 0;	// G
	desc.BorderColor[2] = 0;	// B
	desc.BorderColor[3] = 1;	// A

	// 아래는 다 복붙(이런게 있다 정도 아직 세세히 알 필요는 없음)
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxAnisotropy = 16;
	desc.MaxLOD = FLT_MAX;
	desc.MinLOD = FLT_MIN;
	desc.MipLODBias = 0.0f;

	_device->CreateSamplerState(&desc, _samplerState.GetAddressOf());
}

void Game::CreateBlendState() {
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// 아래는 다 복붙
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = false;	// false로 설정할 경우 아무런 효과가 없음

	// 예를들어 캐릭터가 벽 앞에 잇는데 벽뒤로 카메라가 지나가면서 캐릭터를 보고 싶을 때
	// 벽을 투과해서 볼 수 있게끔 설정할 때도 사용할 수 있다.
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC1_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	_device->CreateBlendState(&desc, _blendState.GetAddressOf());
}

void Game::CreateSRV() {
	// Shader에 리소스로 사용할 수 있는 뷰다. (Shader Resource View)
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(L"apple.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView.GetAddressOf());
	CHECK(hr);
}

void Game::CreateConstantBuffer() {
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC; // CPU_WRITE + GPU_READ
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(TransformData);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& entryMethodName, const string& version, ComPtr<ID3DBlob>& blob) {

	// d3dcompiler.lib이 제공해주는 함수
	// 디버그 용도이고, 최적화는 건너뛰겠다.
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	// path: 파일 명
	// entryMethodName: 진입 함수 명
	// verison: 셰이더 모델 버전
	// blob: 셰이더 컴파일 결과가 저장될 Blob객체
	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryMethodName.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr
	);
	CHECK(hr);
}
