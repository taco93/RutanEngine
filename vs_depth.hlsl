#pragma pack_matrix( row_major )

cbuffer perObjectBuffer : register(b0)
{
    float4x4 wvpMatrix;
    float4x4 worldMatrix;
}

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 worldPos : WORLDPOS;
};


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(float4(input.pos, 1.0f), wvpMatrix);
    output.texCoord = input.texCoord;
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.tangent = mul(input.tangent, (float3x3) worldMatrix);
    output.worldPos = mul(float4(input.pos, 1.0f), worldMatrix).xyz;
    
    return output;
}