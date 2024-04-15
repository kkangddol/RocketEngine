#include "PBRFunction.hlsli"

SamplerState SampleType;

Texture2D Position : register(t0);
Texture2D BaseColor : register(t1);
Texture2D Normal : register(t2);
Texture2D Metallic : register(t3);
Texture2D Roughness : register(t4);
Texture2D AmbientOcclusion : register(t5);

cbuffer LightBuffer : register(b0)
{
    float3 lightDirection;
    float padding;
};

cbuffer viewBuffer : register(b1)
{
    float3 viewPosition;
    float padding1;
}

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(PixelInput input) : SV_TARGET
{ 
    float3 posW = Position.Sample(SampleType, input.uv).rgb;
    float3 baseColor = BaseColor.Sample(SampleType, input.uv).rgb;
    float3 normal = Normal.Sample(SampleType, input.uv).rgb;
    float3 metallic = Metallic.Sample(SampleType, input.uv).rgb;
    float3 roughness = Roughness.Sample(SampleType, input.uv).rgb;
    float3 ambientOcclusion = AmbientOcclusion.Sample(SampleType, input.uv).rgb;
    
    float3 lightDir = -lightDirection;
    float lightIntensity = saturate(dot(normal.xyz, lightDir));
    
    float3 viewDir = normalize(viewPosition - posW);
    float3 halfVector = normalize(lightDir + viewDir);
    
    float nDotL = max(dot(normal, lightDir), 0.0f);
    float nDotV = max(dot(normal, viewDir), 0.0f);
    float lDotH = max(dot(lightDir, viewDir), 0.0f);
    float nDotH = max(dot(normal, halfVector), 0.0f);
    
    float3 specularColor = baseColor * metallic.x;
    
    
    float4 outputColor = float4((Disney_Diffuse(roughness.x ,baseColor, nDotL, nDotV, lDotH)
                            + Specular_BRDF(roughness.x, specularColor, nDotH, nDotV, nDotL, lDotH))
                            * saturate(baseColor * lightIntensity) * dot(normal, lightDir),1.0f);
    
    // float4 outputColor = saturate(float4(baseColor, 1.0f) * lightIntensity);
        
    return outputColor;
}