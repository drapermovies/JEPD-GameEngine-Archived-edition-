#pragma once
#include <directxmath.h>

struct MatrixBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct VertexType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texture;
};

struct ConstBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct PixelBufferType
{
	DirectX::XMFLOAT4 pixel_colour;
};