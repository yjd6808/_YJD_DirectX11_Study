#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const ComPtr<ID3D11Device>& device);
	~VertexBuffer();

	ComPtr<ID3D11Buffer> GetComPtr() { return _vertexBuffer; }
	uint32 GetStride() { return _stride; }
	uint32 GetOffset() { return _offset; }
	uint32 GetCount() { return _count; }

	template <typename T>
	void Create(const vector<T>& vertices) {
		_stride = sizeof(T);
		_count = static_cast<uint32>(vertices.size());

		// �޸� �����͸� GPU�� �޸𸮿� �����ϱ� ����
		// ���ؽ� ����
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		// �� ���۴� � �뵵�� ����� ���ΰ�?
		// D3D11_USAGE_DEFAULT: GPU�� �а� �� �� �ִ�.
		// D3D11_USAGE_IMMUTABLE: GPU�� ���� �� �ִ�.
		// D3D11_USAGE_DYNAMIC: CPU�� �� �� �ְ� GPU�� �������� �ִ�.
		// immutable�� ������ ������ �츮�� ó�� ���̷�ƮX�� �ʱ�ȭ�ϸ鼭 ������ ���ۿ� ����س��� �߰��� �������� ���� ���̱� ����
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// �츮�� �� ���۸� ���ؽ� ���۷� ����ϰڴٴ� �뵵�� �����ش�.
		desc.ByteWidth = sizeof(Vertex) * _count; // ���ؽ� ������ ũ�⸦ �˷��ش�.

		// _vertexBuffer�� � ������ �����͸� �����ؿ� ���ΰ�?
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices.data(); // _vertices�� �����͸� �������ֱ�����

		// desc�� data�� �������� GPU�� �ٷ�� ���ؽ� ���۸� �����Ѵ�.
		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		CHECK(hr);
	}

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11Buffer> _vertexBuffer;

	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};

