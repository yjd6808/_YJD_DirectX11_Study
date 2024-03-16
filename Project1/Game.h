/*
 * 작성자: 윤정도
 * 생성일: 2/3/2024 9:23:29 PM
 * =====================
 *
 */


#pragma once

class Game
{
public:
	Game();
	~Game();

	void Init(HWND hwnd);
	void Render();
	void Update();
private:
	void CreateGeometry();
private:
	shared_ptr<Graphics> _graphics;
	shared_ptr<Pipeline> _pipeline;
private:
	// 도형
	// [ CPU <-> RAM ]: 아직까지는 RAM에 저장된 정보이다.
	// [ GPU <-> VRAM ]

	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;

	// VS(버텍스 셰이더)
	shared_ptr<VertexShader> _vertexShader;

	// RS(Resterizer State)
	shared_ptr<RasterizerState> _rasterizerState;

	// PS(픽셀 셰이더)
	shared_ptr<PixelShader> _pixelShader;

	// SRV
	shared_ptr<Texture> _texture;

	shared_ptr<SamplerState> _samplerState;
	shared_ptr<BlendState> _blendState;
private:
	// 상수버퍼
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;

	Vec3 _localPosition = { 0.f, 0.f, 0.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localScale = { 1.f, 1.f, 1.f };
};