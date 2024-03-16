#pragma once

struct PipelineInfo
{
	shared_ptr<InputLayout> InputLayout;
	shared_ptr<VertexShader> VertexShader;
	shared_ptr<PixelShader> PixelShader;
	shared_ptr<RasterizerState> RasterizerState;
	shared_ptr<BlendState> BlendState;
	D3D11_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

class Pipeline
{
public:
	Pipeline(const ComPtr<ID3D11DeviceContext>& deviceContext);

	void Update(PipelineInfo  info);

	void SetVertexBuffer(shared_ptr<VertexBuffer> buffer);
	void SetIndexBuffer(shared_ptr<IndexBuffer> buffer);

	template <typename T>
	void SetConstantBuffer(uint32 slot, ShaderScope scope, shared_ptr<ConstantBuffer<T>> buffer) {
		if (scope & SS_VertexShader)
			_deviceContext->VSSetConstantBuffers(slot, 1, buffer->GetComPtr().GetAddressOf());

		if (scope & SS_PixelShader)
			_deviceContext->PSSetConstantBuffers(slot, 1, buffer->GetComPtr().GetAddressOf());
	}

	void SetTexture(uint32 slot, ShaderScope scope, shared_ptr<Texture> texture);
	void SetSamplerState(uint32 slot, uint32 scope, shared_ptr<SamplerState> samplerState);

	void Draw(uint32 vertexCount, uint32 startVertexLocation);
	void DrawIndexed(uint32 indexCount, uint32 startIndexLocation, int32 baseIndexLocation);
private:
	ComPtr<ID3D11DeviceContext> _deviceContext;
};

