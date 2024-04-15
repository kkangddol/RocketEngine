Texture2D baseColorTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D metallicTexture : register(t2);
Texture2D roughnessTexture : register(t3);
Texture2D ambientOcclusionTexture : register(t4);
SamplerState SampleType : register(s0);

cbuffer PBRBuffer : register(b0)
{
    float metallic;
    float roughness;
    
    int useNormalMap;
    int useMetallicMap;
    int useRoughnessMap;
    int useAOMap;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

struct DeferredOutput
{
    float4 position : SV_TARGET0;
    float4 baseColor : SV_TARGET1;
    float4 normal : SV_TARGET2;
    float4 metallic : SV_Target3;
    float4 roughness : SV_Target4;
    float4 ambientOcclusion : SV_Target5;
};

DeferredOutput main(PixelInputType input)
{
    DeferredOutput output;
    
    output.position = input.worldPosition;
    output.baseColor = baseColorTexture.Sample(SampleType, input.tex);
    output.normal = (float4(input.normal, 1.0f) * (1 - useNormalMap))
                    + (normalTexture.Sample(SampleType, input.tex) * useNormalMap);
    
    output.metallic = (float4(metallic, metallic, metallic, 1.0f) * (1 - useMetallicMap))
                    + (metallicTexture.Sample(SampleType, input.tex) * useMetallicMap);
    
    output.roughness = float4(roughness, roughness, roughness, 1.0f) * (1 - useRoughnessMap)
                    + (roughnessTexture.Sample(SampleType, input.tex) * useRoughnessMap);
    
    output.ambientOcclusion = float4(0.0f, 0.0f, 0.0f, 0.0f) * (1 - useAOMap)
                            + (ambientOcclusionTexture.Sample(SampleType, input.tex) * useAOMap);
 
    return output;
}
