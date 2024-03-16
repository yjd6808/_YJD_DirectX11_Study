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

	// 3개다 중요함.
	// D3D11_FILL_SOLID		-> 내부를 채우겠다.
	// D3D11_FILL_WIREFRAME -> 내부를 채우지 않는 용도로 사용할때 사용
	desc.FillMode = D3D11_FILL_SOLID;

	// D3D11_CULL_FRONT		-> 앞면을 컬링하겠다.
	// D3D11_CULL_BACK		-> 후면을 컬링하겠다.
	// 컬링은 보통 그리지 않게끔 스킵하는 걸 말한다. (카메라 영역을 벗어나는걸 안그린는 것도 컬링이고, 오클루젼 컬링, 무슨 컬링 다양하게 많다.)
	desc.CullMode = D3D11_CULL_BACK;

	// desc.CullMode의 앞면과 후면은 어떻게 판단하는 것인가?에 대한 설명이다.
	// 우리가 정점을 연결할 때 정점의 인덱스를 반시계 방향 혹은 시계 방향으로 연결할 수가 있었다.
	// 그리고 연결한 정점은 삼각형의 구조를 가진다.
	// 
	// 이 변수를 true로  설정하면 반시계 방향으로 연결한 정점들로 구성된 삼각형들이 앞면임을 의미한다.
	// 이 변수를 false로 설정하면   시계 방향으로 연결한 정점들로 구성된 삼각형들이 후면임을 의미한다.
	desc.FrontCounterClockwise = false;

	// 그외에 궁금한 것들은 문서를 찾아보면 된다.
	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}
