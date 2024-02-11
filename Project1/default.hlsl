// 셰이더 파일의 함수는 GPU가 실행하는 함수이다.

// 셰이더 파일(hlsl)을 추가하면 프로젝트 → 속성을 들어가보면 HLSL 옵션이 생성된다.
// 즉 이 셰이더 파일도 컴파일 대상이라는 뜻이다.
// 셰이더 파일(hlsl)은 컴파일되면 cso파일로 만들어지게된다.
// 이 파일은 2가지 방식으로 로딩 할수 있다.
// 1. 이 파일을 매번 프로젝트에서 빌드하여 .cso 파일을 뽑도록 할 수도 있고
// 2. 한번 뽑아진 .cso 파일을 로드해서 사용할 수도 있다.(Game::LoadSharderFromFile())

struct VS_INPUT
{
	// 이 이름은 우리가 이전에 InputLayout에 작성한 이름과 맞춰주면 된다.
	float4 position : POSITION;
	// float4 color: COLOR;
	float2 uv: TEXCOORD;

};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	// float4 color : COLOR;
	float2 uv : TEXCOORD;
};

cbuffer TransformData : register(b0)
{
	float4 offset;
}

// IA -> VS -> RS -> PS -> OM
// 우리가 Game.cpp의 CreateInputLayout에서 만든 layout이 여기로 넘어온다.
// 이 함수는 정점 단위로 시행된다.
VS_OUTPUT VS(VS_INPUT input)
{
	// 다다음주쯤에는 여러 수학을 활용해서 프로젝션등 처리를 해주지만 우선 처음 배우는 단계이니 그냥 반환한다.
	VS_OUTPUT output;
	output.position = input.position + offset;
	// output.color = input.color;
	output.uv = input.uv;
	return output;
}

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);


// 모든 픽셀에 대해서 처리되는 단계
// 조명처리 같은걸 할때
// 각 rgb색상에 *= 0.45f를 곱해주면 많이 어두워지는걸 알 수 있다. (정말 모든 픽셀마다 적용되는지 검사하는 용도)
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = texture0.Sample(sampler0, input.uv);
	return color;
}

// 이렇게 작성하고 처음 빌드를 하면 main 진입점을 찾지 못했다는 오류가 뜬다.
// 1. 먼저 파일의 우클릭 → 속성으로 들어간다.
// 2. 셰이더 파일을 버텍스 셰이더로 사용할 것이기 때문에 ShaderType을 VertexShader로 변경해준다.
// 3. 셰이더 모델은 5.0으로 변경하자.(일단 왜그런지 모름)

// 빌드하면 default.cso 파일이 만들어지게 된다.
// 우선 처음 학습하는 단계이니 매번 빌드도하고 cso 파일도 매번 실제 코드로 로딩해서 코드작성하도록 한다.
// 나중에는 컴파일 과정은 굳이 생략하고 cso 파일을 로드해서 사용하는 방식을 취하기도 한다.