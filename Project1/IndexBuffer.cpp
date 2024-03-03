#include "pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const ComPtr<ID3D11Device>& device)
	: _device(device)
{
}

IndexBuffer::~IndexBuffer() {
}

void IndexBuffer::Create(const vector<uint32>& indices) {

	_stride = sizeof(uint32);
	_count = static_cast<uint32>(indices.size());

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;	// 우리가 이 버퍼를 버텍스 버퍼로 사용하겠다는 용도를 적어준다.
	desc.ByteWidth = _stride * _count; // 버텍스 버퍼의 크기를 알려준다.

	// _vertexBuffer로 어떤 버퍼의 데이터를 복사해올 것인가?
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = indices.data(); // _vertices의 데이터를 복사해주기위함

	// desc와 data를 바탕으로 GPU가 다루는 버텍스 버퍼를 생성한다.
	HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
	CHECK(hr);
}
