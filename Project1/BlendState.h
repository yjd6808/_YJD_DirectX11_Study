#pragma once

class BlendState
{
public:
	BlendState(const ComPtr<ID3D11Device> device);
	ComPtr<ID3D11BlendState> GetComPtr() { return _blendState; }

	void Create(D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc =
		{
			// false로 설정할 경우 아무런 효과가 없음
			false,							// BlendEnable

			// 예를들어 캐릭터가 벽 앞에 잇는데 벽뒤로 카메라가 지나가면서 캐릭터를 보고 싶을 때
			// 벽을 투과해서 볼 수 있게끔 설정할 때도 사용할 수 있다.
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

