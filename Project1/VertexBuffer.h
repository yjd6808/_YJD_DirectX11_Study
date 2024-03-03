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

		// 메모리 데이터를 GPU에 메모리에 전달하기 위함
		// 버텍스 버퍼
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		// 이 버퍼는 어떤 용도로 사용할 것인가?
		// D3D11_USAGE_DEFAULT: GPU만 읽고 쓸 수 있다.
		// D3D11_USAGE_IMMUTABLE: GPU만 읽을 수 있다.
		// D3D11_USAGE_DYNAMIC: CPU는 쓸 수 있고 GPU는 읽을수만 있다.
		// immutable로 세팅한 이유는 우리가 처음 다이렉트X를 초기화하면서 도형을 버퍼에 기록해놓고 중간에 변경하지 않을 것이기 때문
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 우리가 이 버퍼를 버텍스 버퍼로 사용하겠다는 용도를 적어준다.
		desc.ByteWidth = sizeof(Vertex) * _count; // 버텍스 버퍼의 크기를 알려준다.

		// _vertexBuffer로 어떤 버퍼의 데이터를 복사해올 것인가?
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices.data(); // _vertices의 데이터를 복사해주기위함

		// desc와 data를 바탕으로 GPU가 다루는 버텍스 버퍼를 생성한다.
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

