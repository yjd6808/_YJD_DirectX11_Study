/*
 * 작성자: 윤정도
 * 생성일: 2/12/2024 9:31:09 AM
 * =====================
 *
 */


#pragma once

class Game
{
public:
	void Render();
	void Update();
	void Init(HWND hwnd);
private:
	void RenderBegin();
	void RenderEnd();
private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();
private:
	void LoadShaderFromFile(
		const wstring& path, 
		const string& entryMethodName, 
		const string& version, 
		ComPtr<ID3DBlob>& blob
	);

	void CreateVS();
	void CreateInputLayout();
	void CreatePS();
	void CreateGeometry();
private:
	HWND _hwnd;
	uint32 _width;
	uint32 _height;

	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	ComPtr<IDXGISwapChain> _swapChain;
	ComPtr<ID3D11RenderTargetView> _renderTargetView;
	D3D11_VIEWPORT _viewport;
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };

	ComPtr<ID3DBlob> _vsBlob;
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11InputLayout> _inputLayout;
	ComPtr<ID3DBlob> _psBlob;
	ComPtr<ID3D11PixelShader> _pixelShader;

	vector<Vertex> _vertices;
	ComPtr<ID3D11Buffer> _vertexBuffer;
};