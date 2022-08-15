struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT main(uint vI : SV_VERTEXID)
{
    VS_OUTPUT output;          
    output.texCoord = float2(vI % 2, vI % 4 / 2);
    output.pos = float4((output.texCoord.x - 0.5f) * 2, -(output.texCoord.y - 0.5f) * 2, 0, 1);
    
    return output;
}