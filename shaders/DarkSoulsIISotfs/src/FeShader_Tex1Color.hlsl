
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
	float2 param1 : TEXCOORD;
	float4 param2 : COLOR;
};

OutputStruct vs_main(in InputStruct IN)
{
	OutputStruct OUT = (OutputStruct)0;
	OUT.Position = mul(IN.param0, g_TransformMatrixT);

	float2 tex0 = float2(0.92925, 0.07881);
	float2 tex1 = float2(0.97417, 0.07881);
	float2 tex2 = float2(0.92925, 0.16943);
	float2 tex3 = float2(0.97417, 0.16943);

	float2 tex4 = float2(0.81787, 0.13174);
	float2 tex5 = float2(0.86777, 0.13174);
	float2 tex6 = float2(0.81787, 0.22939);
	float2 tex7 = float2(0.86777, 0.22939);

	float2 tex8 = float2(0.59805, 0.10664);
	float2 tex9 = float2(0.98711, 0.10664);
	float2 tex10 = float2(0.59805, 0.18945);
	float2 tex11 = float2(0.98711, 0.18945);

	if (
		(length(IN.param2 - tex0) < 0.001) ||
		(length(IN.param2 - tex1) < 0.001) ||
		(length(IN.param2 - tex2) < 0.001) ||
		(length(IN.param2 - tex3) < 0.001) ||

		(length(IN.param2 - tex4) < 0.001) ||
		(length(IN.param2 - tex5) < 0.001) ||
		(length(IN.param2 - tex6) < 0.001) ||
		(length(IN.param2 - tex7) < 0.001) ||

		(length(IN.param2 - tex8) < 0.001) ||
		(length(IN.param2 - tex9) < 0.001) ||
		(length(IN.param2 - tex10) < 0.001) ||
		(length(IN.param2 - tex11) < 0.001)) {
		OUT.Position.x = -OUT.Position.x;
	}

	OUT.param1 = IN.param2;
	OUT.param2 = IN.param1;

	return OUT;
}
