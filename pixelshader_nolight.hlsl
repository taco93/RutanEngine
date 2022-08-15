cbuffer toggleBuffer : register(b0)
{
    bool hasAlbedoMap;
    bool hasNormalMap;
    bool hasAlphaMap;
    bool other2;
}

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 worldPos : WORLDPOS;
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;
    float4 worldPos : SV_Target1;
};

Texture2D objTexture : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

PS_OUTPUT main(PS_INPUT input) : SV_Target
{
    PS_OUTPUT output;

    float4 color = float4(1.0f, 0.0f, 0.0f, 0.0f);
    if (hasAlbedoMap)
    {
        color = objTexture.Sample(objSamplerState, input.texCoord);
    }
    output.color = color;
    output.worldPos = float4(input.worldPos, 0.0f);
    
    return output;
}