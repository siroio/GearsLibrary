#pragma once

namespace Glib::Internal::Graphics::ShaderCode
{
    constexpr char MESH_SHADER[]
    {
        R"(
        cbuffer CameraConstant : register(b0)
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
            float4 MatSpeculer;
            float  MatShininess;
            float3 MatPadding;
        };

        cbuffer LightConstant : register(b4)
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
            o.normal = normalize(mul(World, float4(input.normal, 0.0f)).xyz);
            o.uv = input.uv;
            o.tangent = normalize(mul(World, float4(input.tangent.xyz, 0.0f)));
            o.binormal = cross(input.tangent.xyz, o.normal) * input.tangent.w;
            return o;
        }

        float random(float3 seed, int i)
        {
	        return frac(sin(dot(float4(seed, i), float4(12.9898, 78.233, 45.164, 94.673))) * 43758.5453);
        }

        float4 PSmain(PSInput input) : SV_TARGET
        {
            float3 normal = normalTexture.Sample(albedoSampler, input.uv) * 2.0f - 1.0f;
            
            float3 N = normalize(input.normal);

            float3 L = normalize(-LightDirection);
            float3 V = normalize(-input.view);
            float3 H = normalize(L + V);

            float3 posFromLightVP = input.light.xyz / input.light.w;
            float2 shadowUV = (posFromLightVP.xy + float2(1.0f, -1.0f)) * float2(0.5f, -0.5f);

            float bias = clamp(ShadowBias * tan(acos(dot(N, L))), 0, 0.01);
            float shadowMapValue = shadowTexture.Sample(shadowSampler, shadowUV).r;

            float depth = posFromLightVP.z;
            float depthSquare = shadowMapValue * shadowMapValue;
            float variance = clamp(shadowMapValue - depthSquare, 0.00001f, 1.0f);
            float md = depth - shadowMapValue;

            float litFactor = variance / (variance + md * md);
            litFactor = pow(litFactor, 6.0f);
            float shadowWeight = lerp(0.5f, 1.0f, litFactor);

            float shadowAcceptance = saturate(md / bias);
            float visibility = smoothstep(1.0f,  0.00001f, shadowAcceptance);
            float shadowValue = shadowWeight * visibility;

            float4 ambient  = LightAmbient * MatAmbient * visibility;
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
