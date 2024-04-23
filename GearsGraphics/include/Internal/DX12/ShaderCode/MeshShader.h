#pragma once

namespace Glib::Internal::Graphics::ShaderCode
{
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

        cbuffer MaterialConstant : register(b2)
        {
            float4 MatAmbient;
            float4 MatDiffuse;
            float4 MatSpecular;
            float  MatShininess;
            float3 MatPadding;
        };

        cbuffer LightConstant : register(b4)
        {
            float4 LightAmbient;
            float4 LightDiffuse;
            float4 LightSpecular;
            float3 LightDirection;
            float  ShadowBias;
            float  MomentBias;
            float3 Padding;
        };

        Texture2D<float4> albedoTexture : register(t0);
        Texture2D<float4> normalTexture : register(t1);
        Texture2D<float3> shadowTexture : register(t2);

        SamplerState albedoSampler : register(s0);
        SamplerState shadowSampler : register(s1);

        struct VSInput
        {
            float4 position : POSITION;
            float3 normal : NORMAL;
            float2 uv : TEXCOORD;
            int4 bones : BONEINDEX;
            float4 weight : BONEWEIGHT;
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

        VSOutput VSmain(VSInput input)
        {
            float4 worldPosition = mul(World, input.position);
            float4 viewPosition = mul(View, worldPosition);
            VSOutput o;
            o.position = mul(Projection, viewPosition);
            o.view = viewPosition.xyz;
            o.light = mul(LightVP, worldPosition);
            o.uv = input.uv;
            o.normal = mul(World, float4(input.normal, 0.0f)).xyz;
            o.tangent = mul(World, float4(input.tangent.xyz, 0.0f)).xyz;
            o.binormal = cross(o.tangent, o.normal) * input.tangent.w;
            return o;
        }

        float CalculateVSM(float2 moments, float fragDepth, float depthBias, float momentBias)
        {
            float2 b = lerp(moments, float2(0.5f, 0.5f), momentBias);
            float fDepth = fragDepth - depthBias;
            float E_x2 = b.y;
            float Ex_2 = b.x * b.x;
            float variance = E_x2 - Ex_2;
            float mD = b.x - fDepth;
            float mD_2 = mD * mD;
            float p = variance / (variance + mD_2);
            return max(p, fDepth <= b.x);
        }

        float linstep(float min, float max, float v)
        {
            return saturate((v - min) / (max - min));
        }

        float4 PSmain(PSInput input) : SV_TARGET
        {
            float3 normal = normalize(normalTexture.Sample(albedoSampler, input.uv) * 2.0f - 1.0f);

            float3x3 tbn = float3x3(input.tangent, input.binormal, input.normal);
            bool useTangentSpace = !isnan(input.tangent) && !isnan(input.binormal) && dot(input.tangent, input.binormal) == 0.0f;
            float3 N = normalize(useTangentSpace ? mul(normal, tbn) : input.normal);

            float3 L = normalize(-LightDirection);
            float3 V = normalize(-input.view);
            float3 H = normalize(L + V);

            float3 lightPos = input.light.xyz / input.light.w;
            float2 shadowUV = (lightPos.xy + float2(1.0f, -1.0f)) * float2(0.5f, -0.5f);

            float bias = clamp(ShadowBias * tan(acos(dot(N, L))), 0, 0.01);
            float2 shadowMapValue = shadowTexture.Sample(shadowSampler, shadowUV).xy;
            float visibility = 1.0f;
            visibility = CalculateVSM(shadowMapValue, min(lightPos.z, 1.0f), bias, MomentBias);
            visibility = linstep(0.2f, 1.0f, visibility);

            float4 ambient  = LightAmbient * MatAmbient;
            float4 diffuse  = LightDiffuse * MatDiffuse * max(dot(N, L), 0.0f) * visibility;
            float4 specular = LightSpecular * MatSpecular * pow(max(dot(N, H), 0.0f), MatShininess) * visibility;
            float4 baseColor = albedoTexture.Sample(albedoSampler, input.uv);
            float4 color = (ambient + diffuse) * baseColor + specular;
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
            int4 bones : BONEINDEX;
            float4 weight : BONEWEIGHT;
        };

        struct VSOutput
        {
            float4 position : SV_POSITION;
        };

        typedef VSOutput PSInput;

        VSOutput VSmain(VSInput input)
        {
            VSOutput o;
            o.position = mul(LightVP, mul(World, input.position));
            return o;
        }

        float4 PSmain(PSInput input) : SV_TARGET
        {
            float4 depth = input.position;
            depth.xyz /= depth.w;
            return float4(depth.z, depth.z * depth.z, 0.0f, 1.0f);
        })"
    };
}
