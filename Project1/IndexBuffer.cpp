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
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;	// �츮�� �� ���۸� ���ؽ� ���۷� ����ϰڴٴ� �뵵�� �����ش�.
	desc.ByteWidth = _stride * _count; // ���ؽ� ������ ũ�⸦ �˷��ش�.

	// _vertexBuffer�� � ������ �����͸� �����ؿ� ���ΰ�?
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = indices.data(); // _vertices�� �����͸� �������ֱ�����

	// desc�� data�� �������� GPU�� �ٷ�� ���ؽ� ���۸� �����Ѵ�.
	HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
	CHECK(hr);
}
