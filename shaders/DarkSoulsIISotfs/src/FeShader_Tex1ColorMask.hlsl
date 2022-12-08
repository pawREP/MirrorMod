
cbuffer cbFeShader : register(b8) {
	float4x4 g_TransformMatrixT : packoffset(c0.x);
	float3x3 g_MaskuvTransformMatrix : packoffset(c4.x);
	float3x3 g_MaskuvTransformMatrix2 : packoffset(c7.x);
};

struct InputStruct {
	float4 param0 : POSITION;
	float4 param1 : COLOR;
	float2 param2 : TEXCOORD;
};

struct OutputStruct {
	float4 Position : SV_Position;
	float4 param1 : TEXCOORD0;
	float4 param2 : TEXCOORD1;
	float4 param3 : COLOR;
};

OutputStruct vs_main(in InputStruct IN)
{
	OutputStruct OUT = (OutputStruct)0;
	OUT.Position = mul(IN.param0, g_TransformMatrixT);

	float2 tex0 = float2(0.61166, 0.06423);
	float2 tex1 = float2(0.97807, 0.06423);
	float2 tex2 = float2(0.61166, 0.10833);
	float2 tex3 = float2(0.97807, 0.10833);

	float2 tex4 = float2(0.60938, 0.01719);
	float2 tex5 = float2(0.97832, 0.01719);
	float2 tex6 = float2(0.60938, 0.06172);
	float2 tex7 = float2(0.97832, 0.06172);

	if (
		(length(IN.param2 - tex0) < 0.001) ||
		(length(IN.param2 - tex1) < 0.001) ||
		(length(IN.param2 - tex2) < 0.001) ||
		(length(IN.param2 - tex3) < 0.001) ||

		(length(IN.param2 - tex4) < 0.001) ||
		(length(IN.param2 - tex5) < 0.001) ||
		(length(IN.param2 - tex6) < 0.001) ||
		(length(IN.param2 - tex7) < 0.001)) {
		OUT.Position.x = -OUT.Position.x;
	}

	float3 dr = (float3)0;
	dr.xy = mul(IN.param0.xyz, g_MaskuvTransformMatrix).xy;
	dr.x += g_MaskuvTransformMatrix._m02;
	dr.y += g_MaskuvTransformMatrix._m12;
	OUT.param1.zw = dr.xy;

	OUT.param1.xy = IN.param2.xy;

	OUT.param2.xy = 0;
	OUT.param3 = IN.param1;

	return OUT;
}
