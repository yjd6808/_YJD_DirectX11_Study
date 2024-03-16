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

	_rasterizerState = make_shared<RasterizerState>(device);
	_samplerState = make_shared<SamplerState>(device);
	_blendState = make_shared<BlendState>(device);

	_pipeline = make_shared<Pipeline>(deviceContext);

	CreateGeometry();		// 도형을 만든다.
	_vertexShader->Create(L"default.hlsl", "VS", "vs_5_0");
	_inputLayout->Create(VertexTextureData::Descs, _vertexShader->GetBlob());
	_pixelShader->Create(L"default.hlsl", "PS", "ps_5_0");
	
	_rasterizerState->Create();
	_samplerState->Create();
	_blendState->Create();

	_texture->Create(L"apple.png");
	_constantBuffer->Create();
}

void Game::Render() {

	// 준비작업을 먼저한다.
	_graphics->RenderBegin();

	// 여기서 여러가지 물체들을 그린다.
	{
		PipelineInfo info;
		info.InputLayout = _inputLayout;
		info.VertexShader = _vertexShader;
		info.PixelShader = _pixelShader;
		info.RasterizerState = _rasterizerState;
		info.BlendState = _blendState;

		_pipeline->Update(info);
		_pipeline->SetVertexBuffer(_vertexBuffer);
		_pipeline->SetIndexBuffer(_indexBuffer);
		_pipeline->SetConstantBuffer(0, SS_VertexShader, _constantBuffer);
		_pipeline->SetTexture(0, SS_PixelShader, _texture);
		_pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);
		_pipeline->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
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