cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix worldInverse;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer NodeBuffer : register(b1)
{
    matrix nodeTransform[512];
};

cbuffer BoneBuffer : register(b2)
{
    matrix boneTransform[512];
}

struct VertexInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
    uint nodeIndex : BLENDINDICES0;
    float4 weights : BLENDWEIGHT;
    uint4 boneIndex : BLENDINDICES1;
};

float4 main(VertexInputType input) : SV_POSITION
{
    float4 output = (float4) 0;
    
    // Calculate the position of the vertex against the world, view, and projection matrices.
   
    matrix finalOffsetMatrix = mul(mul(boneTransform[input.boneIndex.x], nodeTransform[input.boneIndex.x]), input.weights.x) +
                               mul(mul(boneTransform[input.boneIndex.y], nodeTransform[input.boneIndex.y]), input.weights.y) +
                               mul(mul(boneTransform[input.boneIndex.z], nodeTransform[input.boneIndex.z]), input.weights.z) +
                               mul(mul(boneTransform[input.boneIndex.w], nodeTransform[input.boneIndex.w]), input.weights.w);
    
    float4 resultPosition = mul(float4(input.position, 1.0f), finalOffsetMatrix);
    
    output = mul(resultPosition, viewMatrix);
    output = mul(output, projectionMatrix);
    
    return output;
}