#pragma once

class Graphics
{
public:
	Graphics(HWND hwnd);
	~Graphics();
public:
	void RenderBegin();
	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }
private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewPort();
private:
	HWND _hwnd{};
	uint32 _width{};
	uint32 _height{};


private:
	// DirectX

	/*
	 * COM ��ü��� �θ��� �༮��
	 * I�� �������̽� ����
	 * ID3D11Device* _device;
	 * ID3D11DeviceContext* _deviceContext;
	 * 1. ���۷��� ī���� ����� ����Ѵ�.
	 * _device->AddRef();
	 * _device->Release();
	 *
	 * 2. �Ǽ��� ���۷��� ī������ �߸� ����Ѱ�� ��� �ɱ�?
	 * �޸� ���� ������ �߻��� �� �ִ�.
	 * ���� �������� ���۷��� ī��Ʈ�� �����ϴ°� �������ϴ�.
	 * ����Ʈ �����͹������ �����ϵ��� �ؾ��Ѵ�.
	 * �̸� ���� ComPtr�̶�� ����Ʈ ������ ��ü�� �������ش�.
	 * ComPtrŬ������ wrl����� Microsoft::WRL ���ӽ����̽��� �����ִ�.
	 *
	 * �׷��ٸ� �߿��Ѱ� ID3D11Device�� ID3D11DeviceContext�� �������� �ϴ°��ΰ�?
	 * �츮�� ����ϴ� GPU ���� ��鰳�� ���Ѵ�.
	 * �� GPU���� ������ �޶� �츮�� �Ű�Ἥ �����ϴ°� �Ұ����ϴ�.
	 * �� ID3D11Device�� ID3D11DeviceContext�� �����ڰ� GPU �� ������� �ս��� ��ȭ�� �� �ֵ���
	 * ���̷�ƮX�� �������ִ� �������̽��̴�.
	 * �� 2���� �������̽��� �����ڰ� ����ϴ� ���̷�ƮX�� �Ѱ��ϴ� �� ��ɰ��̶�� �����ϸ�ȴ�.
	 * ID3D11Device�� �Լ��� �� ���캸�� Create���� �����ϴ� �Լ��� ����. -> �� ���� ������ �������� ����ϴ� �༮
	 * ID3D11DeviceContext: �̹� ������� ���ҽ��� ������ ������ ���������ο� ������ �� ����ϴ� �༮. -> ���ֿ��� ����� �����ų� �϶�� ��ų �� ���
	 */

	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;

	// DirectX Graphics Infrastructure
	// ���̷�ƮX�� �׷��� ����� �����Կ����� ������ 9, 11, 12�� ���� ��ȭ�ߴ�.
	// �̶� ������ ���� ����� ��ɵ��� �����ϴµ� �̷� ��ɵ��� DXGI�� ���� �����Ѵٴ� �����̴�.
	// ���� ���� �߿��ϰ� ���Ǵ°� ����ü���̶�� �༮�̴�
	// ��α�: https://minusi.tistory.com/entry/DXGI-%EA%B0%9C%EC%9A%94DirectX-Graphics-Infrastructure-Overview
	// MS����: https://learn.microsoft.com/ko-kr/windows/win32/direct3ddxgi/d3d10-graphics-programming-guide-dxgi
	// IDXGISwapChain�� ������۸� ó���� ���� ����
	// ������۸�: �̱۹��۸��� ��� ȭ���� ����� �� ���ϴ� ������ ������ �ش� ����� ����� ���� �׸��µ�, �̶� ȭ���� �����Ÿ��� ������ �߻��Ѵ�.
	// �� �������� ���ֱ� ���� �̸� �׷��� ȭ���� ����ִ� ����� ������۸��̶�� �Ѵ�.
	// �̶� ���Ǵ°� ���� ���ۿ� �ĸ� ���۰� �ִ�.
	// ���� ���۴� ������ �츮�� ȭ��� ���� �׷��� ������ ���ִ� �����̰�
	// �ĸ� ���۴� ��� ���� �۾��� �������� ���۸� �ǹ��Ѵ�.
	// �츮�� ���� ���� ü����
	// [ ���� ] <-- [ �ĸ� ]
	// [ ���� ] <-- [ �ĸ� ]
	// [ ���� ] <-- [ �ĸ� ]
	// ó�� �ĸ鿡 �ִ� ������ �����͸� ���� ���۷� ��Ӻ����Ͽ� ȭ��� �����ִ� ����� ���ϴ� ����ü�� ����� ����Ѵ�.
	// �� ��� ����
	// [ ���� ] <-- [ �ĸ� ]
	// [ �ĸ� ] --> [ ���� ]
	// [ ���� ] <-- [ �ĸ� ]
	// ó�� ������ �ĸ��� �Ǿ��ٰ� �Դٰ����ϸ鼭 �����ִ� ��ĵ� �����Ѵ�. (�̷��� �ִٴ°� �˾Ƶ���.)

	ComPtr<IDXGISwapChain> _swapChain;

	// RenderTargetView
	// ����ü�ο� �ĸ� ���۸� ����� �޶�� �ؼ�, ���� ���۸� ���������� �Ϸ��ߴ�.
	// �ĸ� ���ۿ��ٰ� �׸��� �׷��޶�� ��û�� �ؾ��ϴµ� �� �ĸ� ���۸� ����Ű�� �༮�̴�.
	// ��, �츮�� ���������� �׸��� �׸� ����� �ǹ��Ѵ�.
	// https://learn.microsoft.com/en-us/windows/uwp/graphics-concepts/render-target-view--rtv-
	ComPtr<ID3D11RenderTargetView> _renderTargetView;

	// ����Ʈ
	D3D11_VIEWPORT _viewport;
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
};

