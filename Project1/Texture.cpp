#include "pch.h"
#include "Texture.h"

Texture::Texture(const ComPtr<ID3D11Device>& device)
	: _device(device)
{}

void Texture::Create(const wstring& path) {
	TexMetadata md;
	ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(
		_device.Get(), 
		img.GetImages(), 
		img.GetImageCount(), 
		md, 
		_shaderResourceView.GetAddressOf()
	);
	CHECK(hr);
}
