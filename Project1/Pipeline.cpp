#include "pch.h"
#include "Pipeline.h"

Pipeline::Pipeline(const ComPtr<ID3D11DeviceContext>& deviceContext)
	: _deviceContext(deviceContext)
{
}

void Pipeline::Update(PipelineInfo info) {

	// VertexBuffer는 데이터 쪼가리일 뿐이므로 inputLayout으로 어떻게 묘사되어있는지 알려줘야한다.
	_deviceContext->IASetInputLayout(info.InputLayout->GetComPtr().Get());
	// 정점을 어떻게 연결할지, 보통 삼각형으로 연결하므로 거의 항상 D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST를 전달한다.
	_deviceContext->IASetPrimitiveTopology(info.Topology);

	// VS단계에서 할일
	// 우리가 셰이더 파일 읽어와서 메모리에 들고있는데 이 데이터를 가지고 작업을 진행해줘
	if (info.VertexShader)
		_deviceContext->VSSetShader(info.VertexShader->GetComPtr().Get(), nullptr, 0);

	// RS단계에서 할일
	if (info.RasterizerState)
		_deviceContext->RSSetState(info.RasterizerState->GetComPtr().Get());

	// PS단계에서 할일
	if (info.PixelShader)
		_deviceContext->PSSetShader(info.PixelShader->GetComPtr().Get(), nullptr, 0);

	// OM
	if (info.BlendState)
		_deviceContext->OMSetBlendState(
			info.BlendState->GetComPtr().Get(),
			info.BlendState->GetBlendFactor(), 
			info.BlendState->GetSampleMask()
		);
}

void Pipeline::SetVertexBuffer(shared_ptr<VertexBuffer> buffer) {
	uint32 stride = buffer->GetStride();
	uint32 offset = buffer->GetOffset();

	_deviceContext->IASetVertexBuffers(0, 1, buffer->GetComPtr().GetAddressOf(), &stride, &offset);
}

void Pipeline::SetIndexBuffer(shared_ptr<IndexBuffer> buffer) {
	_deviceContext->IASetIndexBuffer(buffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Pipeline::SetTexture(uint32 slot, ShaderScope scope, shared_ptr<Texture> texture) {
	if (scope & SS_VertexShader)
		_deviceContext->VSSetShaderResources(slot, 1, texture->GetComPtr().GetAddressOf());

	if (scope & SS_PixelShader)
		_deviceContext->PSSetShaderResources(slot, 1, texture->GetComPtr().GetAddressOf());
}

void Pipeline::SetSamplerState(uint32 slot, uint32 scope, shared_ptr<SamplerState> samplerState) {
	if (scope & SS_VertexShader)
		_deviceContext->VSSetSamplers(slot, 1, samplerState->GetComPtr().GetAddressOf());

	if (scope & SS_PixelShader)
		_deviceContext->PSSetSamplers(slot, 1, samplerState->GetComPtr().GetAddressOf());
}

void Pipeline::Draw(uint32 vertexCount, uint32 startVertexLocation) {
	// 정점만 사용해서 그리는 경우
	_deviceContext->Draw(vertexCount, startVertexLocation);
}

void Pipeline::DrawIndexed(uint32 indexCount, uint32 startIndexLocation, int32 baseIndexLocation) {
	// 인덱스를 사용해서 정점을 연결하는 경우
	_deviceContext->DrawIndexed(indexCount, startIndexLocation, baseIndexLocation);
}

