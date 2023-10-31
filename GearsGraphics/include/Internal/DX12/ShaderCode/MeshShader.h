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
            float4x4 World;
        };

        cbuffer Material : register(b2)
        {
            float4 MatAmbient;
            float4 MatDiffuse;
            float4 MatSpeculer;
            float  MatShininess;
            float3 MatPadding;
        };

        cbuffer DirectionlLight : register(b4)
        {
            float4 LightAmbient;
            float4 LightDiffuse;
            float4 LightSpeculer;
            float3 LightDirection;
            float  ShadowBias;
        };

        Texture2D<float4> albedoTexture : register(t0);
        Texture2D<float4> normalTexture : register(t1);
        Texture2D<float3> shadowTexture : register(t2);

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
        };

        typedef VSOutput PSInput;

        VSOutput VSMain(VSInput input)
        {
            float4x4 ModeView = mul(View, World);
            float4 worldPosition = mul(World, input.position);
            float4 viewPosition = mul(View, worldPosition);
            float3 normal = normalize(mul((float3x3)World, input.normal));
            VSOutput o;
            o.position = mul(Projection, viewPosition);
            output.view = viewPosition.xyz;
            output.light = mul(LightVP, worldPosition);
            output.normal = normal;
            output.uv = input.uv;
            output.tangent = normalize(mul((float3x3)World, input.tangent.xyz));
            output.binormal = cross(normal, output.tangent) * input.tangent.w;
            return o;
        }

        float4 PSMain(PSInput input) : SV_TARGET
        {
            float3 normalColor = normalTexture.Sample(sampler, input.uv).xyz * 2.0f - 1.0f;
            
            float3 N = normalize(input.tangent   * normalColor.x
                                + input.binormal * nomral.y
                                + input.normal   * normal.z);

            float3 L = normalize(-LightDirection);
            float3 V = normalize(-input.view);
            float3 H = normalize(L + V);

            float3 fromLightVP = input.light.xyz / input.light.w;


            float4 ambient  = MatAmbient * LightAmbient;
            float4 diffuse  = LightDiffuse * MatDiffuse * max(dot(N, L), 0.0f);
            float4 speculer = LightSpeculer * MatSpeculer * pow(max(dot(N, H), 0.0f), MatShininess);
            float4 baseColor = albedoTexture.Sample(sampler, input.uv);
            float4 color = (ambient + diffuse) * baseColor + speculer;
            return float4(color.rgb, baseColor.a);
        })"
    };

    constexpr char MESH_SHADOW_SHADER[]
    {
        R"(cbuffer CameraConstant : register(b0)
        {
            float4x4 View;
            float4x4 Projection;
            float4x4 LightVP;
        };

        cbuffer MeshConstant : register(b1)
        {
            float4x4 World;
        };

        struct VSInput
        {
            float4 position : POSITION;
            float3 normal : NORMAL;
            float2 uv : TEXCOORD;
            int4 bones : BONE_NO;
            float4 weight : BONE_WEIGHT;
        };

        struct VSOutput
        {
            float4 position : SV_POSITION;
        };

        typedef VSOutput PSInput;

        VSOutput VSMain(VSInput)
        {
            VSOutput o;
            o.position = mul(LightVP, mul(input.position, World));
            return o;
        }

        float4 PSMain(PSInput) : SV_TARGET
        {
            float4 lightVP = input.position;
            lightVP.xyz /= lightVP.w;
            return float4(lightVP.z, lightVP.z * lightVP.z, 0.0f, 1.0f);
        })"
    };
}
