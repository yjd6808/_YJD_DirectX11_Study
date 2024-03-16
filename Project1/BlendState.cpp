#include "pch.h"
#include "BlendState.h"

BlendState::BlendState(const ComPtr<ID3D11Device> device)
	: _device(device)
{
}

void BlendState::Create(D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc) {
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// 아래는 다 복붙
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0] = renderTargetBlendDesc;

	HRESULT hr = _device->CreateBlendState(&desc, _blendState.GetAddressOf());
	CHECK(hr);
}
