#include "pch.h"
#include "Shader.h"

Shader::Shader(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

Shader::~Shader() {
}

void Shader::LoadShaderFromFile(const wstring& path, const string& entryMethodName, const string& version) {

	_path = path;
	_entryMethodName = entryMethodName;

	// d3dcompiler.lib이 제공해주는 함수
	// 디버그 용도이고, 최적화는 건너뛰겠다.
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	// path: 파일 명ㅋ
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
		_blob.GetAddressOf(),
		nullptr
	);
	CHECK(hr);
}

VertexShader::VertexShader(ComPtr<ID3D11Device> device) : Shader(device) {}
VertexShader::~VertexShader() {}

void VertexShader::Create(const wstring& path, const string& entryMethodName, const string& version) {
	LoadShaderFromFile(path, entryMethodName, version);
	HRESULT hr = _device->CreateVertexShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}

PixelShader::PixelShader(ComPtr<ID3D11Device> device) : Shader(device) {}
PixelShader::~PixelShader() {}

void PixelShader::Create(const wstring& path, const string& entryMethodName, const string& version) {
	LoadShaderFromFile(path, entryMethodName, version);
	HRESULT hr = _device->CreatePixelShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}
