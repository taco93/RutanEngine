cbuffer lightBuffer : register(b0)
{
    float3 ambientColor;
    float ambientStrength;
    float3 lightColor;
    float lightStrength;
    float3 lightPos;
    float attenuationA;
    float attenuationB;
    float attenuationC;
    float padding1;
    float padding2;
}

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 texCoord : TEXCOORD;
};

Texture2D albedoTexture : register(t0);
Texture2D posTexture : register(t1);
Texture2D normalTexture : register(t2);

SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_Target
{
    float3 finalColor = float3(1.0f, 0.0f, 0.0f);
    float alpha = 1.0f;
    
    // G Buffer
    float3 sampleColor = albedoTexture.Sample(objSamplerState, input.texCoord);
    float3 worldPos = posTexture.Sample(objSamplerState, input.texCoord);
    float3 normal = normalTexture.Sample(objSamplerState, input.texCoord);
    
    float3 ambientLight = ambientColor * ambientStrength;
    
    float3 appliedLight = ambientLight;
    
    float3 vecToLight = normalize(lightPos - worldPos);
    
    float3 diffuseIntensity = max(dot(vecToLight, normal), 0);
    
    float distanceToLight = distance(lightPos, worldPos);
    
    float attenuationFactor = 1 / (attenuationA + attenuationB * distanceToLight + attenuationC * pow(distanceToLight, 2));
    
    diffuseIntensity *= attenuationFactor;
    
    float3 diffuseLight = diffuseIntensity * lightStrength * lightColor;
    
    appliedLight += diffuseLight;
    
    finalColor = sampleColor * appliedLight;
    
    return float4(finalColor, alpha);
}