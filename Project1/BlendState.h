#pragma once

class BlendState
{
public:
	BlendState(const ComPtr<ID3D11Device> device);
	ComPtr<ID3D11BlendState> GetComPtr() { return _blendState; }

	void Create(D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc =
		{
			// false�� ������ ��� �ƹ��� ȿ���� ����
			false,							// BlendEnable

			// ������� ĳ���Ͱ� �� �տ� �մµ� ���ڷ� ī�޶� �������鼭 ĳ���͸� ���� ���� ��
			// ���� �����ؼ� �� �� �ְԲ� ������ ���� ����� �� �ִ�.
			D3D11_BLEND_SRC_ALPHA,			// SrcBlend
			D3D11_BLEND_INV_SRC1_ALPHA,		// DestBlend
			D3D11_BLEND_OP_ADD,				// BlendOp
			D3D11_BLEND_ONE,				// SrcBlendAlpha
			D3D11_BLEND_ZERO,				// DestBlendAlpha
			D3D11_BLEND_OP_ADD,				// BlendOpAlpha
			D3D11_COLOR_WRITE_ENABLE_ALL,	// RenderTargetWriteMask
		}
	);

	uint32 GetSampleMask() { return _sampleMask; }
	const float* GetBlendFactor() { return &_blendFactor; }
private:
	float _blendFactor = 0.f;
	uint32 _sampleMask = 0xffffffff;
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11BlendState> _blendState;
};

