////////////////////////////////////////////////////////////////////////////////
// Filename: glow_ps.hlsl
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D colorTexture : register(t0);
Texture2D glowTexture : register(t1);
SamplerState SampleType;

cbuffer GlowBuffer
{
	float glowStrength;
	float3 padding;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 GlowPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float4 glowColor;
	float4 color;


	// �� �ؽ�ó ��ǥ ��ġ���� ���÷��� ����Ͽ� �ؽ�ó���� �ȼ� ������ ���ø��մϴ�.
	textureColor = colorTexture.Sample(SampleType, input.tex);

	// ���� �ؽ�ó�� ���ø��մϴ�.
	glowColor = glowTexture.Sample(SampleType, input.tex);

	// ������ ���̿� ���� ������ ���� �ؽ�ó ������ �߰��մϴ�.
    color = saturate(textureColor + (glowColor * glowStrength));

    return color;
}