#pragma once

namespace Glib::Internal::Graphics::ShaderCode
{
    //TODO: フォンシェーディング実装
    constexpr char MESH_SHADER[]
    {
        R"(cbuffer CameraConstant : register(b0)
        {
            float4x4 View;
            float4x4 Projection;
            float4x4 LightVP;
        };

        cbuffer MeshConstant : register(b1)
        {
            float4x4 Model;
        }

        cbuffer Material : register(b2)
        {
            float4 Ambient;
            float4 Diffuse;
            float4 Speculer;
            float Shininess;
            float3 padding;
        }

        cbuffer DirectionlLight : register(b4)
        {
            float4 Ambient;
            float4 Diffuse;
            float4 Speculer;
            float3 LightDirection;
            float ShadowBias;
        }

        Texture2D<float4> albedoTexture : register(t0);
        Texture2D<float4> normalTexture : register(t1);
        Texture2D<float3> shadowMap : register(t2);

        SamplerState sampler : register(s0);
        SamplerState shadowSampler : register(s1);

        struct VSInput
        {
            float4 position : POSITION;
            float3 normal : NORMAL;
            float2 uv : TEXCOORD;
            int4 bones : BONE_NO;
            float4 weight : BONE_WEIGHT;
            float4 tangent : TANGENT;
        };

        struct VSOutput
        {
            float4 position : SV_POSITION;
		    float3 view : POSITION0;
		    float4 light : POSITION1;
		    float3 normal : NORMAL;
		    float2 uv : TEXCOORD;
		    float3 tangent : TANGENT;
		    float3 binormal : BINORMAL;
        }

        typedef VSOutput PSInput;

        VSOutput VS(VSInput input)
        {
            VSOutput o;
            o.binormal = input.tangent.w * corss((o.tangent, o.normal);
            return o;
        }

        float4 PS(PSInput input) : SV_TARGET
        {})"
    };

    constexpr char MESH_SHADOW_SHADER[]{ R"()" };
}
