#pragma once

class Texture
{
public:
	Texture(const ComPtr<ID3D11Device>& device);
	ComPtr<ID3D11ShaderResourceView> GetComPtr() { return _shaderResourceView; }
	void Create(const wstring& path);
private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
};

