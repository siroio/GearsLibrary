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

        SamplerState albedoSampler : register(s0);
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

        VSOutput VSmain(VSInput input)
        {
            float4x4 ModeView = mul(View, World);
            float4 worldPosition = mul(World, input.position);
            float4 viewPosition = mul(View, worldPosition);
            float3 normal = normalize(mul((float3x3)World, input.normal));
            VSOutput o;
            o.position = mul(Projection, viewPosition);
            o.view = viewPosition.xyz;
            o.light = mul(LightVP, worldPosition);
            o.normal = normal;
            o.uv = input.uv;
            o.tangent = normalize(mul((float3x3)World, input.tangent.xyz));
            o.binormal = cross(normal, o.tangent) * input.tangent.w;
            return o;
        }

        float random(float3 seed, int i)
        {
	        return frac(sin(dot(float4(seed, i), float4(12.9898, 78.233, 45.164, 94.673))) * 43758.5453);
        }

        float4 PSmain(PSInput input) : SV_TARGET
        {
            float3 normal = normalTexture.Sample(albedoSampler, input.uv).xyz * 2.0f - 1.0f;
            
            float3 N = normalize(input.tangent   * normal.x
                                + input.binormal * normal.y
                                + input.normal   * normal.z);

            float3 L = normalize(-LightDirection);
            float3 V = normalize(-input.view);
            float3 H = normalize(L + V);

            float3 posFromLightVP = input.light.xyz / input.light.w;
            float2 shadowUV = (posFromLightVP.xy + float2(1.0f, -1.0f)) * float2(0.5f, -0.5f);

            float bias = clamp(ShadowBias * tan(acos(dot(N, L))), 0, 0.01);
            float visibility = 1.0f;

            float2 poissonDisk[16] =
            {
                { -0.94201624,  -0.39906216 },
                { 0.94558609,  -0.76890725 },
                { -0.094184101, -0.92938870 },
                { 0.34495938,   0.29387760 },
                { -0.20290487,   0.58051342 },
                { 0.67258888,  -0.13125129 },
                { -0.55066599,   0.77551898 },
                { 0.04716250,   0.97484398 },
                { 0.71373935,   0.26432821 },
                { 0.02300153,  -0.66625530 },
                { 0.82045635,   0.95136894 },
                { -0.67458926,  -0.75558874 },
                { 0.34873644,  -0.37458623 },
                { -0.94071815,   0.84484976 },
                { -0.50315495,  -0.19719433 },
                { 0.98379229,  -0.18368212 }
            };

            for (int i = 0; i < 4; i++)
            {
                int idx = int(16.0 * random(input.position.xyy, i)) % 16;
                float sampleVal = shadowTexture.Sample(shadowSampler, float3((shadowUV + poissonDisk[i] / 700.0f).xy, posFromLightVP.z - bias));
                visibility -= 0.2 * (1.0f - sampleVal);
            }

            float4 ambient  = MatAmbient * LightAmbient;
            float4 diffuse  = LightDiffuse * MatDiffuse * max(dot(N, L), 0.0f) * visibility;
            float4 speculer = LightSpeculer * MatSpeculer * pow(max(dot(N, H), 0.0f), MatShininess) * visibility;
            float4 baseColor = albedoTexture.Sample(albedoSampler, input.uv);
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

        VSOutput VSmain(VSInput input)
        {
            VSOutput o;
            o.position = mul(LightVP, mul(input.position, World));
            return o;
        }

        float4 PSmain(PSInput input) : SV_TARGET
        {
            float4 lightVP = input.position;
            lightVP.xyz /= lightVP.w;
            return float4(lightVP.z, lightVP.z * lightVP.z, 0.0f, 1.0f);
        })"
    };
}
