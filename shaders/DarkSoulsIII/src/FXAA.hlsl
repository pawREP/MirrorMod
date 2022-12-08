// Game\shader\gxposteffect.shaderbnd.dcx

struct InputStruct {
	float4 param0 : POSITION;
	float2 param1 : TEXCOORD;
};

struct OutputStruct {
	float4 Position : SV_Position;
	float4 param1 : TEXCOORD0;
	float4 param2 : TEXCOORD1;
};

OutputStruct vs_main(in InputStruct IN)
{
	OutputStruct OUT = (OutputStruct)0;

	OUT.Position = IN.param0;
	OUT.Position.x = -OUT.Position.x;
	OUT.param1.xy = IN.param1;

	return OUT;
}
