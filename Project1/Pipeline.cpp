#include "pch.h"
#include "Pipeline.h"

Pipeline::Pipeline(const ComPtr<ID3D11DeviceContext>& deviceContext)
	: _deviceContext(deviceContext)
{
}

void Pipeline::Update(PipelineInfo info) {

	// VertexBuffer�� ������ �ɰ����� ���̹Ƿ� inputLayout���� ��� ����Ǿ��ִ��� �˷�����Ѵ�.
	_deviceContext->IASetInputLayout(info.InputLayout->GetComPtr().Get());
	// ������ ��� ��������, ���� �ﰢ������ �����ϹǷ� ���� �׻� D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST�� �����Ѵ�.
	_deviceContext->IASetPrimitiveTopology(info.Topology);

	// VS�ܰ迡�� ����
	// �츮�� ���̴� ���� �о�ͼ� �޸𸮿� ����ִµ� �� �����͸� ������ �۾��� ��������
	if (info.VertexShader)
		_deviceContext->VSSetShader(info.VertexShader->GetComPtr().Get(), nullptr, 0);

	// RS�ܰ迡�� ����
	if (info.RasterizerState)
		_deviceContext->RSSetState(info.RasterizerState->GetComPtr().Get());

	// PS�ܰ迡�� ����
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
	// ������ ����ؼ� �׸��� ���
	_deviceContext->Draw(vertexCount, startVertexLocation);
}

void Pipeline::DrawIndexed(uint32 indexCount, uint32 startIndexLocation, int32 baseIndexLocation) {
	// �ε����� ����ؼ� ������ �����ϴ� ���
	_deviceContext->DrawIndexed(indexCount, startIndexLocation, baseIndexLocation);
}

