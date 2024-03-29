﻿#pragma once

class Transform;

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

	void Update();
	void Render(shared_ptr<Pipeline> pipeline);
private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;

	shared_ptr<Geometry<VertexColorData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;

	shared_ptr<VertexShader> _vertexShader;
	shared_ptr<RasterizerState> _rasterizerState;
	shared_ptr<PixelShader> _pixelShader;
	shared_ptr<Texture> _texture;
	shared_ptr<SamplerState> _samplerState;
	shared_ptr<BlendState> _blendState;

	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;

	shared_ptr<Transform> _transform;
	shared_ptr<Transform> _parentTransform;
};

