#include "Shader_Defines.hlsli"

cbuffer PerFrameInfo : register(b0)
{
    float4x4 g_ViewMatrix;
    float4x4 g_ProjMatrix;
};

struct VS_IN
{
    float3 vWorldPos    : TEXCOORD0;
    float4 vColor       : COLOR0;
};

struct VS_OUT
{
    float3 vWorldPos    : TEXCOORD0;
    float4 vColor       : COLOR0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    Out.vWorldPos = In.vWorldPos;
    Out.vColor = In.vColor;
    
    return Out;
}

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vColor : COLOR0;
};

[maxvertexcount(4)]
void GS_MAIN(point VS_OUT In[1], inout TriangleStream<GS_OUT> Triangles)
{    
    GS_OUT Out[4];
    float size = 10.0f;
    Out[0].vPosition = float4(In[0].vWorldPos + float3( size,  size, 0.0f), 1.0f);
    Out[1].vPosition = float4(In[0].vWorldPos + float3(-size,  size, 0.0f), 1.0f);
    Out[2].vPosition = float4(In[0].vWorldPos + float3(-size, -size, 0.0f), 1.0f);
    Out[3].vPosition = float4(In[0].vWorldPos + float3( size, -size, 0.0f), 1.0f);
    
    [unroll]
    for (uint i = 0; i < 4; ++i)
    {
        Out[i].vPosition = mul(Out[i].vPosition, g_ViewMatrix);
        Out[i].vPosition = mul(Out[i].vPosition, g_ProjMatrix);
        Out[i].vColor = In[0].vColor;
    }
    
    Triangles.Append(Out[1]);
    Triangles.Append(Out[2]);
    Triangles.Append(Out[0]);
    Triangles.Append(Out[3]);
    
    Triangles.RestartStrip();
}

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(GS_OUT In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = In.vColor;
    
    return Out;
}
struct PS_OUT2
{
    float4 vColor : SV_TARGET0;
    float  fColor : SV_TARGET1;
};

PS_OUT2 PS_MAIN2(GS_OUT In)
{
    PS_OUT2 Out = (PS_OUT2) 0;
    Out.vColor = In.vColor;
    
    return Out;
}

technique11 DefaultTechnique
{
    pass PASS_BASE
    {
        SetRasterizerState(RS_NonCull);
        SetDepthStencilState(DSS_NOWRITE, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass PASS_WBOIT
    {
        SetRasterizerState(RS_NonCull);
        SetDepthStencilState(DSS_NOWRITE, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN2();
    }
}