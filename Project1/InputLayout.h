#pragma once

class InputLayout
{
public:
	InputLayout(const ComPtr<ID3D11Device>& device);
	~InputLayout();
public:
	ComPtr<ID3D11InputLayout> GetComPtr() { return _inputLayout; }
	void Create(const vector<D3D11_INPUT_ELEMENT_DESC>& descs, const ComPtr<ID3DBlob>& vsBlob);

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11InputLayout> _inputLayout;
};