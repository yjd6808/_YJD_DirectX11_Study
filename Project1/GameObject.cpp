#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) 
	: _device(device)
	, _deviceContext(deviceContext)
{
	_geometry = make_shared<Geometry<VertexColorData>>();
	GeometryHelper::CreateRectangle(_geometry, Color{1.0f, 0.0f, 0.0f, 1.0f});

	_vertexBuffer = make_shared<VertexBuffer>(device);
	_vertexBuffer->Create(_geometry->GetVertices());

	_indexBuffer = make_shared<IndexBuffer>(device);
	_indexBuffer->Create(_geometry->GetIndices());

	_vertexShader = make_shared<VertexShader>(device);
	_vertexShader->Create(L"color.hlsl", "VS", "vs_5_0");

	_inputLayout = make_shared<InputLayout>(device);
	_inputLayout->Create(VertexColorData::Descs, _vertexShader->GetBlob());
	
	_pixelShader = make_shared<PixelShader>(device);
	_pixelShader->Create(L"color.hlsl", "PS", "ps_5_0");

	_rasterizerState = make_shared<RasterizerState>(device);
	_rasterizerState->Create();

	_blendState = make_shared<BlendState>(device);
	_blendState->Create();

	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(device, deviceContext);
	_constantBuffer->Create();

	_texture = make_shared<Texture>(device);
	_texture->Create(L"apple.png");
	
	_samplerState = make_shared<SamplerState>(device);
	_samplerState->Create();

	_transform = make_shared<Transform>();
	_parentTransform = make_shared<Transform>();
	_parentTransform->AddChild(_transform);
	_transform->SetParent(_parentTransform);
}

void GameObject::Update() {
	_transform->GetRotation();
	Vec3 rot = _transform->GetRotation();
	rot.y += 0.008f;
	_transform->SetRotation(rot);

	_transformData.MatWorld = _transform->GetWorldMatrix();
	_constantBuffer->CopyData(_transformData);
}

void GameObject::Render(shared_ptr<Pipeline> pipeline) {
	PipelineInfo info;
	info.InputLayout = _inputLayout;
	info.VertexShader = _vertexShader;
	info.PixelShader = _pixelShader;
	info.RasterizerState = _rasterizerState;
	info.BlendState = _blendState;

	pipeline->Update(info);

	pipeline->SetVertexBuffer(_vertexBuffer);
	pipeline->SetIndexBuffer(_indexBuffer);
	pipeline->SetConstantBuffer(0, SS_VertexShader, _constantBuffer);
	pipeline->SetTexture(0, SS_PixelShader, _texture);
	pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);

	pipeline->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
}
