
cbuffer _Globals : register(b0) {
	float4x4 gVC_WorldViewClipMtx : packoffset(c0.x);
	float4 gVC_ScreenSize : packoffset(c12.x);
	float4 gVC_NoiseParam : packoffset(c68.x);
	float4 DL_FREG_007 : packoffset(c69.x);
	float4 DL_FREG_008 : packoffset(c70.x);
	float4 DL_FREG_009 : packoffset(c71.x);
	float4 DL_FREG_010 : packoffset(c72.x);
	float4 DL_FREG_011 : packoffset(c73.x);
	float4 DL_FREG_012 : packoffset(c74.x);
	float4 DL_FREG_013 : packoffset(c75.x);
	float4 DL_FREG_014 : packoffset(c76.x);
	float4 DL_FREG_015 : packoffset(c77.x);
	float4 DL_FREG_016 : packoffset(c78.x);
	float4 DL_FREG_017 : packoffset(c79.x);
	float4 DL_FREG_018 : packoffset(c80.x);
	float4 DL_FREG_019 : packoffset(c81.x);
	float4 DL_FREG_020 : packoffset(c82.x);
	float4 DL_FREG_021 : packoffset(c83.x);
	float4 DL_FREG_022 : packoffset(c84.x);
	float4 DL_FREG_023 : packoffset(c85.x);
	float4 DL_FREG_024 : packoffset(c86.x);
	float4 DL_FREG_025 : packoffset(c87.x);
	float4 DL_FREG_026 : packoffset(c88.x);
	float4 DL_FREG_027 : packoffset(c89.x);
	float4 DL_FREG_028 : packoffset(c90.x);
	float4 DL_FREG_029 : packoffset(c91.x);
	float4 DL_FREG_030 : packoffset(c92.x);
	float4 DL_FREG_031 : packoffset(c93.x);
	float4 DL_FREG_032 : packoffset(c94.x);
	float4 DL_FREG_033 : packoffset(c95.x);
	float4 DL_FREG_034 : packoffset(c96.x);
	float4 DL_FREG_035 : packoffset(c97.x);
	float4 DL_FREG_036 : packoffset(c98.x);
	float4 DL_FREG_037 : packoffset(c99.x);
	float4 DL_FREG_038 : packoffset(c100.x);
	float4 DL_FREG_039 : packoffset(c101.x);
	float4 DL_FREG_040 : packoffset(c102.x);
	float4 DL_FREG_041 : packoffset(c103.x);
	float4 DL_FREG_042 : packoffset(c104.x);
	float4 DL_FREG_043 : packoffset(c105.x);
	float4 DL_FREG_044 : packoffset(c106.x);
	float4 DL_FREG_045 : packoffset(c107.x);
	float4 DL_FREG_046 : packoffset(c108.x);
	float4 DL_FREG_047 : packoffset(c109.x);
	float4 DL_FREG_048 : packoffset(c110.x);
	float4 DL_FREG_049 : packoffset(c111.x);
	float4 DL_FREG_050 : packoffset(c112.x);
	float4 DL_FREG_051 : packoffset(c113.x);
	float4 DL_FREG_052 : packoffset(c114.x);
	float4 DL_FREG_053 : packoffset(c115.x);
	float4 DL_FREG_054 : packoffset(c116.x);
	float4 DL_FREG_055 : packoffset(c117.x);
	float4 DL_FREG_056 : packoffset(c118.x);
	float4 DL_FREG_057 : packoffset(c119.x);
	float4x4 DL_FREG_058 : packoffset(c120.x);
	float4x4 DL_FREG_062 : packoffset(c124.x);
	float4 DL_FREG_066 : packoffset(c128.x);
	float4 DL_FREG_067 : packoffset(c129.x);
	float4 DL_FREG_068 : packoffset(c130.x);
	float4 DL_FREG_069 : packoffset(c131.x);
	float4 DL_FREG_070 : packoffset(c132.x);
	float4 DL_FREG_071 : packoffset(c133.x);
	float4 DL_FREG_072 : packoffset(c134.x);
	float4 DL_FREG_073 : packoffset(c135.x);
	float4x4 DL_FREG_074 : packoffset(c136.x);
	float4 DL_FREG_078 : packoffset(c140.x);
	uint4 gFC_FrameIndex : packoffset(c141.x);
};

struct InputStruct {
	uint VertexID : SV_VertexID;
};

struct OutputStruct {
	float4 Position : SV_Position;
	float4 param1 : TEXCOORD;
};

OutputStruct EditedShaderVS(in InputStruct IN)
{
	OutputStruct OUT = (OutputStruct)0;
	float4 r0 = 0;
	float4 r1 = 0;

	r0.x = IN.VertexID.x >> 1;
	r0.y = r0.x;
	r1.w = r0.x;
	r1.y = -r0.y + 1.0;
	r0.x = IN.VertexID.x & 1;
	r1.xz = r0.xx;

	OUT.Position.xyzw = float4(0.0,0.0,0.0,1.0);
	OUT.Position.xy = r1.xy * float2(2.0,2.0) + float2(-1.0,-1.0);
	OUT.Position.x *= -1;
	OUT.param1.zw = r1.zz * gVC_NoiseParam.xx + gVC_NoiseParam.zz;
	OUT.param1.xy = r1.zw;

	return OUT;
}
