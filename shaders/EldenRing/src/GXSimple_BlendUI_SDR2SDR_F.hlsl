// Game\shader\gxshader.shaderbnd.dcx

SamplerState LinearClampSampler : register(s1);
Texture2D<float4> HDRScene : register(t0);
Texture2D<float4> UIScene : register(t1);

cbuffer DisplayMappingData : register(b0) {
	float outputGammaForSDR : packoffset(c0.x);
	int noUIBlend : packoffset(c0.y);
	int rangeAdj : packoffset(c0.z);
	int enableDithering : packoffset(c0.w);
	float noiseIntensity : packoffset(c1.x);
	float noiseScale : packoffset(c1.y);
	float uiMaxLumScale : packoffset(c1.z);
	float uiMaxLumScaleRecp : packoffset(c1.w);
	float uiMaxNitsNormalizedLinear : packoffset(c2.x);
	float4x3 mtxColorConvert : packoffset(c3.x);
};

struct InputStruct {
	float4 Position : SV_Position;
	float4 param1 : TEXCOORD0;
	float2 param2 : TEXCOORD1;
};

struct OutputStruct {
	float4 Target0 : SV_Target0;
};

OutputStruct BlendUI_SDR2SDR(in InputStruct IN)
{
	OutputStruct OUT = (OutputStruct)0;

	float2 xMirrorSample = IN.param2.xy;
	xMirrorSample.x = 1 - xMirrorSample.x;
	float4 r0 = HDRScene.Sample(LinearClampSampler, xMirrorSample.xy );
	float4 r1 = UIScene.Sample(LinearClampSampler, IN.param2.xy );
	
	r0.xyz = r0.xyz * r1.www + r1.xyz;
	r0.xyz = log(r0.xyz);
	r0.xyz = r0.xyz * outputGammaForSDR.xxx;
	OUT.Target0.xyz = exp(r0.xyz);
	OUT.Target0.w = 1;

	return OUT;
}
