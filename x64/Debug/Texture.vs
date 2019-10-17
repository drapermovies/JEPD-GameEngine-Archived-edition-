cbuffer MatrixBuffer
{
	matrix world_matrix;
	matrix view_matrix;
	matrix projection_matrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, world_matrix);
	output.position = mul(output.position, view_matrix);
	output.position = mul(output.position, projection_matrix);

	output.tex = input.tex;

	return output;
}