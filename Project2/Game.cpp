/*
 * 작성자: 윤정도
 * 생성일: 2/12/2024 9:31:19 AM
 * =====================
 *
 */

#include "Pch.h"
#include "Game.h"

void Game::Render() {
	RenderBegin();

	{
		uint32 stride = sizeof Vertex;
		uint32 offset = 0;

		// IA
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);

		// RS

		// PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());

		// OM

		// 마지막
		// _deviceContext->Draw(_vertices.size(), 0);
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}

	RenderEnd();
}

void Game::Update() {
}

void Game::Init(HWND hwnd) {
	_hwnd = hwnd;
	_width = GMinSizeX;
	_height = GMinSizeY;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();

	CreateVS();
	CreateInputLayout();
	CreatePS();
	CreateGeometry();
	CreateSRV();
}

void Game::RenderBegin() {
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd() {
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Game::CreateDeviceAndSwapChain() {

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;

		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;

		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;

		desc.Windowed = true;
		desc.OutputWindow = _hwnd;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		nullptr,
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

	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	CHECK(hr);

	hr = _device->CreateRenderTargetView(pBackBuffer, nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);

	pBackBuffer->Release();
}

void Game::SetViewport() {
	_viewport.Width = float(GMinSizeX);
	_viewport.Height = float(GMinSizeY);
	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
}

void Game::LoadShaderFromFile(	const wstring& path, 
								const string& entryMethodName, 
								const string& version, 
								ComPtr<ID3DBlob>& blob) {
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

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


void Game::CreateVS() {
	LoadShaderFromFile(L"default.hlsl", "VS", "vs_5_0", _vsBlob);
	HRESULT hr = _device->CreateVertexShader(
		_vsBlob->GetBufferPointer(), 
		_vsBlob->GetBufferSize(), 
		nullptr, 
		_vertexShader.GetAddressOf()
	);
	CHECK(hr);
}

void Game::CreatePS() {
	LoadShaderFromFile(L"default.hlsl", "PS", "ps_5_0", _psBlob);
	HRESULT hr = _device->CreatePixelShader(
		_psBlob->GetBufferPointer(), 
		_psBlob->GetBufferSize(),
		nullptr, 
		_pixelShader.GetAddressOf()
	);
	CHECK(hr);
}

void Game::CreateInputLayout() {
	D3D11_INPUT_ELEMENT_DESC layouts[]{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,	D3D11_INPUT_PER_VERTEX_DATA	},
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	12,	D3D11_INPUT_PER_VERTEX_DATA	}
	};

	const int32 count = sizeof layouts / sizeof D3D11_INPUT_ELEMENT_DESC;
	HRESULT hr = _device->CreateInputLayout(
		layouts, 
		count, 
		_vsBlob->GetBufferPointer(), 
		_vsBlob->GetBufferSize(), 
		_inputLayout.GetAddressOf()
	);
	CHECK(hr);
}

void Game::CreateGeometry() {
	HRESULT hr;
	{
		_vertices.resize(4);

		// 13
		// 02
		_vertices[0].Position	= { -0.5f, -0.5f, 0.0f };
		_vertices[0].UV			= { 0.0f, 1.0f };

		_vertices[1].Position	= { -0.5f, 0.5f, 0.0f };
		_vertices[1].UV			= { 0.0f, 0.0f };

		_vertices[2].Position	= { 0.5f, -0.5f, 0.0f };
		_vertices[2].UV			= { 1.0f, 1.0f };

		_vertices[3].Position	= { 0.5f, 0.5f, 0.0f };
		_vertices[3].UV			= { 1.0f, 0.0f };
	}

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = sizeof(Vertex) * _vertices.size();

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data();

		hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		CHECK(hr);
	}

	{
		_indices = { 0, 1, 2, 2, 1, 3 };
	}

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = sizeof(uint32) * _indices.size();

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data();

		hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		CHECK(hr);
	}
}

void Game::CreateSRV() {
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(L"apple.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(
		_device.Get(),
		img.GetImages(), 
		img.GetImageCount(), 
		md, 
		_shaderResourceView.GetAddressOf()
	);
	CHECK(hr);
}

