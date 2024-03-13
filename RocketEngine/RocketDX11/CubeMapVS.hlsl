TextureCube cubeMap	: register(t0);

struct VS_INPUT
{
	
};

struct VS_OUTPUT_Sky
{
    float4 Pos : SV_POSITION;
    float3 TexCoord : TEXCOORD0;
};

float4 main(VS_INPUT input) : SV_POSITION
{
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}