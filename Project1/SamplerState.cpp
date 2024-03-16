#include "pch.h"
#include "SamplerState.h"

SamplerState::SamplerState(const ComPtr<ID3D11Device> device)
	: _device(device)
{
}

void SamplerState::Create() {
	// https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/ns-d3d11-d3d11_sampler_desc?redirectedfrom=MSDN
	// SamplerState는 UV 좌표를 1을 넘어서는 부분을 어떻게 채워줄지 정해주는 녀석이라고 생각하면 된다.
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.BorderColor[0] = 1;	// R
	desc.BorderColor[1] = 0;	// G
	desc.BorderColor[2] = 0;	// B
	desc.BorderColor[3] = 1;	// A

	// 아래는 다 복붙(이런게 있다 정도 아직 세세히 알 필요는 없음)
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxAnisotropy = 16;
	desc.MaxLOD = FLT_MAX;
	desc.MinLOD = FLT_MIN;
	desc.MipLODBias = 0.0f;

	HRESULT hr = _device->CreateSamplerState(&desc, _samplerState.GetAddressOf());
	CHECK(hr);
}
