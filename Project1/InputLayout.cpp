#include "pch.h"
#include "InputLayout.h"

InputLayout::InputLayout(const ComPtr<ID3D11Device>& device)
	: _device(device)
{
}

InputLayout::~InputLayout() {
}

void InputLayout::Create(const vector<D3D11_INPUT_ELEMENT_DESC>& descs, const ComPtr<ID3DBlob>& vsBlob) {
	uint32 count = static_cast<uint32>(descs.size());
	HRESULT hr = _device->CreateInputLayout(descs.data(), count, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
	CHECK(hr);
}

