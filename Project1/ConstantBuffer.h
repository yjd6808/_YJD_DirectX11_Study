#pragma once

template <typename T>
class ConstantBuffer
{
public:
	ConstantBuffer(
		const ComPtr<ID3D11Device>& device, 
		const ComPtr<ID3D11DeviceContext>& deviceContext)
	: _device(device)
	, _deviceContext(deviceContext)
	{}
	~ConstantBuffer() {}

	ComPtr<ID3D11Buffer> GetComPtr() { return _constantBuffer; }

	void Create() {
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC; // CPU_WRITE + GPU_READ
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
		CHECK(hr);
	}

	void CopyData(const T& data) {
		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		// Map으로 뚜겅을 연다. 이제 constantBuffer에 데이터를 쓸 수 있다.
		// constantBuffer의 메모리 위치를 의미하는 녀석이 subResource이다.
		_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

		// CPU -> GPU 데이터쓰기
		::memcpy(subResource.pData, &data, sizeof(TransformData));

		// Unamp으로 뚜껑을 닫는다. 더이상 수정 불가능하다.
		_deviceContext->Unmap(_constantBuffer.Get(), 0);
	}
private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	ComPtr<ID3D11Buffer> _constantBuffer;
};

