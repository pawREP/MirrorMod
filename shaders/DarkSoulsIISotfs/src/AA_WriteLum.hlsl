
SamplerState SourceTexSampler_sampler : register(s0);
Texture2D<float4> SourceTexSampler_texture : register(t0);

struct InputStruct {
	float4 Position : SV_Position;
	float4 param1 : TEXCOORD0;
	float4 param2 : TEXCOORD1;
};

struct OutputStruct {
	float4 Target0 : SV_Target0;
};

OutputStruct ps_main(in InputStruct IN)
{
	OutputStruct OUT;
	IN.param1.x = 1-IN.param1.x;
	OUT.Target0 = SourceTexSampler_texture.Sample(SourceTexSampler_sampler, IN.param1);
    OUT.Target0.w = dot(float4(0.2989, 0.5866, 0.1145, 0.0000), OUT.Target0.xyzx);
	return OUT;
}
