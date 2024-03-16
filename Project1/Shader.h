#pragma once

// ShaderResource를 사용할 건데
enum ShaderScope
{
	SS_None = 0,
	SS_VertexShader = 1 << 0,
	SS_PixelShader = 1 << 1,
	SS_Both = SS_VertexShader | SS_PixelShader,
};

class Shader
{
public:
	Shader(ComPtr<ID3D11Device> device);
	virtual ~Shader();
	virtual void Create(const wstring& path, const string& entryMethodName, const string& version) = 0;

	ComPtr<ID3DBlob> GetBlob() { return _blob; }
	void LoadShaderFromFile(const wstring& path, const string& entryMethodName, const string& version);
protected:
	wstring _path;
	string _entryMethodName;

	ComPtr<ID3D11Device> _device;
	ComPtr<ID3DBlob> _blob;
};

class VertexShader : public Shader
{
public:
	VertexShader(ComPtr<ID3D11Device> device);
	~VertexShader() override;
	
	void Create(const wstring& path, const string& entryMethodName, const string& version) override;

	ComPtr<ID3D11VertexShader> GetComPtr() { return _vertexShader; }
private:
	ComPtr<ID3D11VertexShader> _vertexShader;
};

class PixelShader : public Shader
{
public:
	PixelShader(ComPtr<ID3D11Device> device);
	~PixelShader() override;

	void Create(const wstring& path, const string& entryMethodName, const string& version) override;

	ComPtr<ID3D11PixelShader> GetComPtr() { return _pixelShader; }
private:
	ComPtr<ID3D11PixelShader> _pixelShader;
};


