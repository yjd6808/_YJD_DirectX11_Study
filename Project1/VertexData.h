#pragma once

struct VertexColorData
{
	Vec3 Position = { 0, 0, 0 };
	Color Color = { 0, 0, 0, 0 };

	static vector<D3D11_INPUT_ELEMENT_DESC> Descs;
};

struct VertexTextureData
{
	Vec3 Position = { 0, 0, 0 };
	Vec2 UV = { 0, 0 };

	static vector<D3D11_INPUT_ELEMENT_DESC> Descs;
};