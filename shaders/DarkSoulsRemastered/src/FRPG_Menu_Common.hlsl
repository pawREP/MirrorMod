
cbuffer _Globals : register(b0) {
	float4x4 gVC_WorldViewClipMtx : packoffset(c0.x);
	float4 gVC_FontSharpParam : packoffset(c4.x);
	float4 gFC_FontSharpParam : packoffset(c5.x);
};

struct InputStruct {
	int2 param0 : POSITION;
	float4 param1 : COLOR;
	int2 param2 : TEXCOORD;
};

struct OutputStruct {
	float4 Position : SV_Position;
	float4 param1 : COLOR;
	float2 param2 : TEXCOORD;
};

OutputStruct EditedShaderVS(in InputStruct IN)
{
	OutputStruct OUT = (OutputStruct)0;
	float3 r0 = float3(IN.param0.xy, 0.0) * float3(0.125, 0.125, 0.0);
	r0.z = 1;

	float4x4 trans = transpose(gVC_WorldViewClipMtx);
	trans[0].z = trans[0].w;
	trans[1].z = trans[1].w;
	trans[2].z = trans[2].w;
	trans[3].z = trans[3].w;

	OUT.Position.x = dot(r0.xyz, trans[0].xyz);
	OUT.Position.y = dot(r0.xyz, trans[1].xyz);
	OUT.Position.z = dot(r0.xyz, trans[2].xyz);
	OUT.Position.w = dot(r0.xyz, trans[3].xyz);

	OUT.param1 = IN.param1;
	OUT.param2.xy = IN.param2 * float2(0.0002441, 0.0002441);

	if (
		//lockon
		length(IN.param2.xy - int2(1184, 520)) == 0 ||
		length(IN.param2.xy - int2(1484, 520)) == 0 ||
		length(IN.param2.xy - int2(1184, 1120)) == 0 ||
		length(IN.param2.xy - int2(1484, 1120)) == 0 ||
		//black bar
		length(IN.param2.xy - int2(2108, 2344)) == 0 ||
		length(IN.param2.xy - int2(2540, 2344)) == 0 ||
		length(IN.param2.xy - int2(2108, 2376)) == 0 ||
		length(IN.param2.xy - int2(2540, 2376)) == 0 ||
		//yellow
		length(IN.param2.xy - int2(2108, 2288)) == 0 ||
		length(IN.param2.xy - int2(2540, 2288)) == 0 ||
		length(IN.param2.xy - int2(2108, 2320)) == 0 ||
		length(IN.param2.xy - int2(2540, 2320)) == 0 ||
		//red
		length(IN.param2.xy - int2(2108, 2232)) == 0 ||
		length(IN.param2.xy - int2(2540, 2232)) == 0 ||
		length(IN.param2.xy - int2(2108, 2264)) == 0 ||
		length(IN.param2.xy - int2(2540, 2264)) == 0
		) {
		OUT.Position.x = -OUT.Position.x;
	}

	return OUT;
}
