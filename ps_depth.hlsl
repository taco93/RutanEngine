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
    float4 normal : SV_Target2;
};

Texture2D objTexture : register(t0);
Texture2D normalMapTexture : register(t1);
Texture2D alphaMapTexture : register(t2);

SamplerState objSamplerState : SAMPLER : register(s0);

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    PS_OUTPUT output;
    
    if (hasNormalMap)
    {
        float3 normalMap = normalMapTexture.Sample(objSamplerState, input.texCoord).rgb;
        
        normalMap = (2.0f * normalMap) - 1.0f;
        
        input.tangent = normalize(input.tangent - dot(input.tangent, input.normal) * input.normal);
        
        float3 biTangent = cross(input.normal, input.tangent);
        
        float3x3 TBN = float3x3(input.tangent, biTangent, input.normal);
        input.normal = normalize(mul(normalMap, TBN));
    }
    
    float4 color = float4(1.0f, 0.0f, 0.0f, 0.0f);
    
    if (hasAlbedoMap)
    {
        color = objTexture.Sample(objSamplerState, input.texCoord);
    }
    
    output.color = color;
    output.normal = float4(input.normal, 0.0f);
    output.worldPos = float4(input.worldPos, 0.0f);
    
    return output;
}