SamplerState SampleType;

Texture2D Position : register(t0);
Texture2D BaseColor : register(t1);
Texture2D Normal : register(t2);

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(PixelInput input) : SV_TARGET
{
    float3 posW = Position.Sample(SampleType, input.uv).rgb;
    float3 albedo = BaseColor.Sample(SampleType, input.uv).rgb;
    float3 normal = Normal.Sample(SampleType, input.uv).rgb;
    
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}