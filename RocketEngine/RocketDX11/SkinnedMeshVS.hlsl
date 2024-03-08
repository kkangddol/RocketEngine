cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix worldInverse;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer : register(b1)
{
    float3 cameraPosition;
    float padding;
};

cbuffer NodeBuffer : register(b2)
{
    matrix nodeTransform[256];
};

cbuffer BoneBuffer : register(b3)
{
    matrix boneTransform[256];
}

struct VertexInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    uint nodeIndex : BLENDINDICES0;
    float4 weights : BLENDWEIGHT;
    uint4 boneIndex : BLENDINDICES1;
};
    
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDiretion : TEXCOORD1;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output = (PixelInputType) 0;
    
    // Calculate the position of the vertex against the world, view, and projection matrices.
   
    matrix finalOffsetMatrix = mul(boneTransform[input.boneIndex.x], input.weights.x) +
                               mul(boneTransform[input.boneIndex.y], input.weights.y) +
                               mul(boneTransform[input.boneIndex.z], input.weights.z) +
                               mul(boneTransform[input.boneIndex.w], input.weights.w);
    
    float4 resultPosition = mul(float4(input.position, 1.0f), finalOffsetMatrix);
    
 //       mul(input.weights.x, mul(float4(input.position, 1.0f), boneTransform[input.boneIndex.x])) +
 //       mul(input.weights.y, mul(float4(input.position, 1.0f), boneTransform[input.boneIndex.y])) +
 //       mul(input.weights.z, mul(float4(input.position, 1.0f), boneTransform[input.boneIndex.z])) +
 //       mul(input.weights.w, mul(float4(input.position, 1.0f), boneTransform[input.boneIndex.w]));
    
    matrix nodeTransformMatrix = nodeTransform[input.nodeIndex];
    
    output.position = mul(resultPosition, mul(nodeTransformMatrix, worldMatrix));
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    // offsetMatrix�� node matrix�� ���� matrix�� ����ġ�� ���ؾߵɰŰ����� �ϴ� ����
    output.normal = mul(input.normal, (float3x3) transpose(worldInverse));
    output.normal = normalize(output.normal);
    
    float4 worldPosition = mul(resultPosition, mul(nodeTransformMatrix, worldMatrix));
    
    output.viewDiretion = cameraPosition.xyz - worldPosition.xyz;
    
    output.viewDiretion = normalize(output.viewDiretion);
    
    return output;
}