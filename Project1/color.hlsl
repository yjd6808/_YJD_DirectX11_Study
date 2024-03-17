// ���̴� ������ �Լ��� GPU�� �����ϴ� �Լ��̴�.

// ���̴� ����(hlsl)�� �߰��ϸ� ������Ʈ �� �Ӽ��� ������ HLSL �ɼ��� �����ȴ�.
// �� �� ���̴� ���ϵ� ������ ����̶�� ���̴�.
// ���̴� ����(hlsl)�� �����ϵǸ� cso���Ϸ� ��������Եȴ�.
// �� ������ 2���� ������� �ε� �Ҽ� �ִ�.
// 1. �� ������ �Ź� ������Ʈ���� �����Ͽ� .cso ������ �̵��� �� ���� �ְ�
// 2. �ѹ� �̾��� .cso ������ �ε��ؼ� ����� ���� �ִ�.(Game::LoadSharderFromFile())

struct VS_INPUT
{
	// �� �̸��� �츮�� ������ InputLayout�� �ۼ��� �̸��� �����ָ� �ȴ�.
	float4 position : POSITION;
	float4 color: COLOR;
	// float2 uv: TEXCOORD;

};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	// float2 uv : TEXCOORD;
};

cbuffer TransformData : register(b0)
{
	row_major matrix matWorld;
	row_major matrix matView;
	row_major matrix matProjection;
}

// IA -> VS -> RS -> PS -> OM
// �츮�� Game.cpp�� CreateInputLayout���� ���� layout�� ����� �Ѿ�´�.
// �� �Լ��� ���� ������ ����ȴ�.
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	// WVP (World * View * Projection)�� �����Ѵ�.
	float4 position = mul(input.position, matWorld);
	position = mul(position, matView);
	position = mul(position, matProjection);

	output.position = position;
	output.color = input.color;
	// output.uv = input.uv;
	return output;
}

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);


// ��� �ȼ��� ���ؼ� ó���Ǵ� �ܰ�
// ����ó�� ������ �Ҷ�
// �� rgb���� *= 0.45f�� �����ָ� ���� ��ο����°� �� �� �ִ�. (���� ��� �ȼ����� ����Ǵ��� �˻��ϴ� �뵵)
float4 PS(VS_OUTPUT input) : SV_Target
{
	return input.color;
}

// �̷��� �ۼ��ϰ� ó�� ���带 �ϸ� main �������� ã�� ���ߴٴ� ������ ���.
// 1. ���� ������ ��Ŭ�� �� �Ӽ����� ����.
// 2. ���̴� ������ ���ؽ� ���̴��� ����� ���̱� ������ ShaderType�� VertexShader�� �������ش�.
// 3. ���̴� ���� 5.0���� ��������.(�ϴ� �ֱ׷��� ��)

// �����ϸ� default.cso ������ ��������� �ȴ�.
// �켱 ó�� �н��ϴ� �ܰ��̴� �Ź� ���嵵�ϰ� cso ���ϵ� �Ź� ���� �ڵ�� �ε��ؼ� �ڵ��ۼ��ϵ��� �Ѵ�.
// ���߿��� ������ ������ ���� �����ϰ� cso ������ �ε��ؼ� ����ϴ� ����� ���ϱ⵵ �Ѵ�.