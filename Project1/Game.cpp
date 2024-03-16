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
	_graphics = make_shared<Graphics>(hwnd);
	ComPtr<ID3D11Device> device = _graphics->GetDevice();
	ComPtr<ID3D11DeviceContext> deviceContext = _graphics->GetDeviceContext();

	_vertexBuffer = make_shared<VertexBuffer>(device);
	_indexBuffer = make_shared<IndexBuffer>(device);
	_inputLayout = make_shared<InputLayout>(device);
	_geometry = make_shared<Geometry<VertexTextureData>>();
	_vertexShader = make_shared<VertexShader>(device);
	_pixelShader = make_shared<PixelShader>(device);
	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(device, deviceContext);
	_texture = make_shared<Texture>(device);

	CreateGeometry();		// 도형을 만든다.
	_vertexShader->Create(L"default.hlsl", "VS", "vs_5_0");
	_inputLayout->Create(VertexTextureData::Descs, _vertexShader->GetBlob());
	_pixelShader->Create(L"default.hlsl", "PS", "ps_5_0");
	
	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();

	_texture->Create(L"apple.png");
	_constantBuffer->Create();
}

void Game::Render() {

	// 준비작업을 먼저한다.
	_graphics->RenderBegin();

	// 여기서 여러가지 물체들을 그린다.
	{
		// IA단계에서 할일
		uint32 stride = sizeof(VertexTextureData);
		uint32 offset = 0;

		auto spDeviceContext = _graphics->GetDeviceContext();

		// 렌더링 파이프라인 그림에서 VertexBuffer를 IA와 연결하는 단계를 수행해준다.
		spDeviceContext->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
		spDeviceContext->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
		// VertexBuffer는 데이터 쪼가리일 뿐이므로 inputLayout으로 어떻게 묘사되어있는지 알려줘야한다.
		spDeviceContext->IASetInputLayout(_inputLayout->GetComPtr().Get());
		// 정점을 어떻게 연결할지, 보통 삼각형으로 연결하므로 거의 항상 D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST를 전달한다.
		spDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// VS단계에서 할일
		// 우리가 셰이더 파일 읽어와서 메모리에 들고있는데 이 데이터를 가지고 작업을 진행해줘
		spDeviceContext->VSSetShader(_vertexShader->GetComPtr().Get(), nullptr, 0);
		spDeviceContext->VSSetConstantBuffers(0, 1, _constantBuffer->GetComPtr().GetAddressOf());


		// RS단계에서 할일
		spDeviceContext->RSSetState(_rasterizerState.Get());

		// PS단계에서 할일
		spDeviceContext->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);
		spDeviceContext->PSSetShaderResources(0, 1, _texture->GetComPtr().GetAddressOf());
		spDeviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

		// OM
		spDeviceContext->OMSetBlendState(_blendState.Get(), nullptr, 0xffffffff);

		// 물체를 그려달라고 요청한다.
		spDeviceContext->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
	}

	// 그림을 다 그렸으니 제출한다.
	_graphics->RenderEnd();
}

void Game::Update() {

	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	_transformData.MatWorld = matScale * matRotation * matTranslation;
	_constantBuffer->CopyData(_transformData);
}

void Game::CreateGeometry() {
	GeometryHelper::CreateRectangle(_geometry);

	_vertexBuffer->Create(_geometry->GetVertices());
	_indexBuffer->Create(_geometry->GetIndices());
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

	HRESULT hr = _graphics->GetDevice()->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
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

	_graphics->GetDevice()->CreateSamplerState(&desc, _samplerState.GetAddressOf());
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
	_graphics->GetDevice()->CreateBlendState(&desc, _blendState.GetAddressOf());
}
