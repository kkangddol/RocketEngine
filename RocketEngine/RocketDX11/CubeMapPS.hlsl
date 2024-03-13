struct VS_OUTPUT_Sky
{
    float4 Pos : SV_POSITION;
    float3 TexCoord : TEXCOORD0;
};

float4 main(VS_OUTPUT_Sky input) : SV_TARGET
{
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
	//return cubeMap.Sample(samLinear, input.TexCoord);
}