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
	void CreateInputLayout();

	void CreateVS();
	void CreatePS();

	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateBlendState();
	void CreateSRV(); //Shader Resource View

	void CreateConstantBuffer();

	void LoadShaderFromFile(const wstring& path, const string& entryMethodName, const string& version, ComPtr<ID3DBlob>& blob);
private:
	shared_ptr<Graphics> _graphics;
private:
	// 도형
	// [ CPU <-> RAM ]: 아직까지는 RAM에 저장된 정보이다.
	// [ GPU <-> VRAM ]

	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;

	// VS(버텍스 셰이더)
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3DBlob> _vsBlob;

	// RS(Resterizer State)
	ComPtr<ID3D11RasterizerState> _rasterizerState;

	// PS(픽셀 셰이더)
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3DBlob> _psBlob;

	// SRV
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView;

	ComPtr<ID3D11SamplerState> _samplerState;
	ComPtr<ID3D11BlendState> _blendState;
private:
	// 상수버퍼
	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer;

	Vec3 _localPosition = { 0.f, 0.f, 0.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localScale = { 1.f, 1.f, 1.f };
};