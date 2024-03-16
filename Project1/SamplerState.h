#pragma once

class SamplerState
{
public:
	SamplerState(const ComPtr<ID3D11Device> device);
	ComPtr<ID3D11SamplerState> GetComPtr() { return _samplerState; }
	void Create();
private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11SamplerState> _samplerState;
};

