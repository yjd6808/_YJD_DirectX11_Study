/*
 * 작성자: 윤정도
 * 생성일: 2/3/2024 9:23:29 PM
 * =====================
 *
 */


#pragma once

class Game
{
public:
	Game();
	~Game();

	void Init(HWND hwnd);
	void Render();
	void Update();

private:
	void RenderBegin();
	void RenderEnd();
private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
private:
	HWND _hwnd{};
	uint32 _width{};
	uint32 _height{};


private:
	// DirectX

	/*
	 * COM 객체라고 부르는 녀석들
	 * I는 인터페이스 약자
	 * ID3D11Device* _device;
	 * ID3D11DeviceContext* _deviceContext;
	 * 1. 레퍼런스 카운팅 방식을 사용한다.
	 * _device->AddRef();
	 * _device->Release();
	 * 
	 * 2. 실수로 레퍼런스 카운팅을 잘못 계산한경우 어떻게 될까?
	 * 메모리 누수 문제가 발생할 수 있다.
	 * 따라서 수동으로 레퍼런스 카운트를 관리하는건 부적절하다.
	 * 스마트 포인터방식으로 관리하도록 해야한다.
	 * 이를 위해 ComPtr이라는 스마트 포인터 객체를 제공해준다.
	 * ComPtr클래스는 wrl헤더의 Microsoft::WRL 네임스페이스에 속해있다.
	 *
	 * 그렇다면 중요한건 ID3D11Device와 ID3D11DeviceContext는 무슨일을 하는것인가?
	 * 우리가 사용하는 GPU 모델은 몇백개에 달한다.
	 * 각 GPU마다 스펙이 달라서 우리가 신경써서 개발하는건 불가능하다.
	 * 즉 ID3D11Device와 ID3D11DeviceContext는 개발자가 GPU 모델 상관없이 손쉽게 대화할 수 있도록
	 * 다이렉트X가 제공해주는 인터페이스이다.
	 * 이 2가지 인터페이스는 개발자가 사용하는 다이렉트X를 총괄하는 총 사령관이라고 생각하면된다.
	 * ID3D11Device의 함수를 쭉 살펴보면 Create으로 시작하는 함수가 많다. -> 즉 유닛 생산을 전문으로 담당하는 녀석
	 * ID3D11DeviceContext: 이미 만들어진 리소스를 가지고 렌더링 파이프라인에 연결할 때 사용하는 녀석. -> 유닛에게 명령을 내리거나 하라고 시킬 때 사용
	 */

	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;

	// DirectX Graphics Infrastructure
	// 다이렉트X는 그래픽 기술이 발전함에따라 버전이 9, 11, 12와 같이 진화했다.
	// 이때 발전이 느린 공통된 기능들이 존재하는데 이런 기능들을 DXGI로 빼서 관리한다는 개념이다.
	// 이중 제일 중요하게 사용되는게 스왑체인이라는 녀석이다
	// 블로그: https://minusi.tistory.com/entry/DXGI-%EA%B0%9C%EC%9A%94DirectX-Graphics-Infrastructure-Overview
	// MS문서: https://learn.microsoft.com/ko-kr/windows/win32/direct3ddxgi/d3d10-graphics-programming-guide-dxgi
	// IDXGISwapChain는 더블버퍼링 처리를 위해 쓴다
	// 더블버퍼링: 싱글버퍼링일 경우 화면을 출력할 때 변하는 내용이 있으면 해당 장면을 지우고 새로 그리는데, 이때 화면이 깜빡거리는 현상이 발생한다.
	// 이 깜빡임을 없애기 위해 미리 그려둔 화면을 띄워주는 방식을 더블버퍼링이라고 한다.
	// 이때 사용되는게 전면 버퍼와 후면 버퍼가 있다.
	// 전면 버퍼는 실제로 우리가 화면상에 보는 그래픽 정보가 들어가있는 버퍼이고
	// 후면 버퍼는 계속 수정 작업이 진행중인 버퍼를 의미한다.
	// 우리가 만들 스왑 체인은
	// [ 전면 ] <-- [ 후면 ]
	// [ 전면 ] <-- [ 후면 ]
	// [ 전면 ] <-- [ 후면 ]
	// 처럼 후면에 있는 버퍼의 데이터를 전면 버퍼로 고속복사하여 화면상에 보여주는 방식을 취하는 스왑체인 방식을 사용한다.
	// 이 방식 말고도
	// [ 전면 ] <-- [ 후면 ]
	// [ 후면 ] --> [ 전면 ]
	// [ 전면 ] <-- [ 후면 ]
	// 처럼 전면이 후면이 되었다가 왔다갔다하면서 보여주는 방식도 존재한다. (이런게 있다는걸 알아두자.)
	
	ComPtr<IDXGISwapChain> _swapChain;

	// RenderTargetView
	// 스왑체인에 후면 버퍼를 만들어 달라고 해서, 더블 버퍼링 지원까지는 완료했다.
	// 후면 버퍼에다가 그림을 그려달라고 요청을 해야하는데 그 후면 버퍼를 가리키는 녀석이다.
	// https://learn.microsoft.com/en-us/windows/uwp/graphics-concepts/render-target-view--rtv-
	ComPtr<ID3D11RenderTargetView> _renderTargetView;
};