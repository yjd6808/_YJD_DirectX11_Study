#include "pch.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState(const ComPtr<ID3D11Device> device)
	: _device(device)
{
}

void RasterizerState::Create() {
	// https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_rasterizer_desc
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// 3���� �߿���.
	// D3D11_FILL_SOLID		-> ���θ� ä��ڴ�.
	// D3D11_FILL_WIREFRAME -> ���θ� ä���� �ʴ� �뵵�� ����Ҷ� ���
	desc.FillMode = D3D11_FILL_SOLID;

	// D3D11_CULL_FRONT		-> �ո��� �ø��ϰڴ�.
	// D3D11_CULL_BACK		-> �ĸ��� �ø��ϰڴ�.
	// �ø��� ���� �׸��� �ʰԲ� ��ŵ�ϴ� �� ���Ѵ�. (ī�޶� ������ ����°� �ȱ׸��� �͵� �ø��̰�, ��Ŭ���� �ø�, ���� �ø� �پ��ϰ� ����.)
	desc.CullMode = D3D11_CULL_BACK;

	// desc.CullMode�� �ո�� �ĸ��� ��� �Ǵ��ϴ� ���ΰ�?�� ���� �����̴�.
	// �츮�� ������ ������ �� ������ �ε����� �ݽð� ���� Ȥ�� �ð� �������� ������ ���� �־���.
	// �׸��� ������ ������ �ﰢ���� ������ ������.
	// 
	// �� ������ true��  �����ϸ� �ݽð� �������� ������ ������� ������ �ﰢ������ �ո����� �ǹ��Ѵ�.
	// �� ������ false�� �����ϸ�   �ð� �������� ������ ������� ������ �ﰢ������ �ĸ����� �ǹ��Ѵ�.
	desc.FrontCounterClockwise = false;

	// �׿ܿ� �ñ��� �͵��� ������ ã�ƺ��� �ȴ�.
	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}
