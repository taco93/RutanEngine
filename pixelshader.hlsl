#define POINT_LIGHT 0
#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2

struct Light
{
    float3 ambientColor;
    float ambientStrength;
    float3 color;
    float lightStrength;
    float3 position;
    float attenuationA;
    float attenuationB;
    float attenuationC;
    uint type;
    float spotAngle;
    float3 direction;
    float padding;
};

StructuredBuffer<Light> lights : register(t3);

cbuffer CameraParams : register(b0)
{
    float3 eyePos;
    float padding;
}

struct LightingResult
{
    float4 Diffuse;
    float4 Specular;
};


float DoAttenuation(Light light, float d)
{
    return 1.0f / (light.attenuationA + light.attenuationB * d + light.attenuationC * d * d);
}


float4 DoDiffuse(Light light, float3 L, float3 N)
{
    float NdotL = max(0, dot(N, L));
    
    return float4(light.color * NdotL, 1.0f);
}

float4 DoSpecular(Light light, float3 V, float3 L, float3 N)
{
    // Phong lighting.
    float3 R = normalize(reflect(-L, N));
    float RdotV = max(0, dot(R, V));
 
    // Blinn-Phong lighting
    float3 H = normalize(L + V);
    float NdotH = max(0, dot(N, H));
 
    return float4(light.color * pow(RdotV, 2), 1.0f);
}

LightingResult DoPointLight(Light light, float3 V, float3 P, float3 N)
{
    LightingResult result;
    
    float3 L = (light.position - P);
    float distance = length(L);
    L = L / distance;
    
    float attenuation = DoAttenuation(light, distance);
    
    result.Diffuse = DoDiffuse(light, L, N) * attenuation * light.lightStrength;
    result.Specular = DoSpecular(light, V, L, N) * attenuation * light.lightStrength;
    
    return result;
}

float DoSpotCone(Light light, float3 L)
{
    float minCos = cos(light.spotAngle);
    float maxCos = (minCos + 1.0f) / 2.0f;
    float cosAngle = dot(light.direction, -L);
    return smoothstep(minCos, maxCos, cosAngle);
}

LightingResult DoSpotLight(Light light, float3 V, float3 P, float3 N)
{
    LightingResult result;
    
    float3 L = (light.position - P);
    float distance = length(L);
    L = L / distance;
    
    float attenuation = DoAttenuation(light, distance);
    float spotIntensity = DoSpotCone(light, L);
    
    result.Diffuse = DoDiffuse(light, L, N) * attenuation * spotIntensity;
    result.Specular = DoSpecular(light, V, L, N) * attenuation * spotIntensity;
    
    return result;
}

LightingResult ComputeLighting(float3 P, float3 N)
{
    uint count = 0;
    uint stride = 0;
    
    lights.GetDimensions(count, stride);
    float3 V = normalize(eyePos - P);
    
    LightingResult totalResult = { { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } };
    
    for (int i = 0; i < count; i++)
    {
        LightingResult result = { { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } };
        Light light = lights[i];
        
        switch (light.type)
        {
            case POINT_LIGHT:
            {
                    result = DoPointLight(light, V, P, N);
                    break;
                }
            case SPOT_LIGHT:
            {
                    result = DoSpotLight(light, V, P, N);
                    break;
                }

        }
        
        totalResult.Diffuse += result.Diffuse;
        totalResult.Specular += result.Specular;
    }
    
    totalResult.Diffuse = saturate(totalResult.Diffuse);
    totalResult.Specular = saturate(totalResult.Specular);
    
    return totalResult;
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
    // G Buffer
    float4 sampleColor = albedoTexture.Sample(objSamplerState, input.texCoord);
    float3 worldPos = posTexture.Sample(objSamplerState, input.texCoord);
    float3 normal = normalTexture.Sample(objSamplerState, input.texCoord);
    
    LightingResult lit = ComputeLighting(worldPos, normal);
    
    float4 ambient = float4(1.0f, 1.0f, 1.0f, 1.0f) * lit.Diffuse;
    float4 diffuse = lit.Diffuse;
    float4 specular = lit.Specular;
    
    float4 finalColor = (ambient + diffuse + specular) * sampleColor;
      
    return finalColor;
}