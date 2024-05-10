float4 main(float4 input : SV_POSITION) : SV_TARGET
{
    float4 output = (float) 0;
    
    float depth = input.z / input.w;
    
    output = float4(depth, depth, depth, 1.0f);
    
    return output;
}