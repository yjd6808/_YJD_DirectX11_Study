#include "pch.h"
#include "GeometryHelper.h"

void GeometryHelper::CreateRectangle(shared_ptr<Geometry<VertexColorData>> geometry, Color color) {
	vector<VertexColorData> vertices;
	vertices.resize(4);

	vertices[0].Position = Vec3{ -0.5f, -0.5f, 0.0f };
	vertices[0].Color = color;

	vertices[1].Position = Vec3{ -0.5, 0.5f, 0 };
	vertices[1].Color = color;

	vertices[2].Position = Vec3{ 0.5f, -0.5f, 0 };
	vertices[2].Color = color;

	vertices[3].Position = Vec3{ 0.5f, 0.5f, 0 };
	vertices[3].Color = color;

	geometry->SetVertices(vertices);

	const vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndicies(indices);
}

void GeometryHelper::CreateRectangle(shared_ptr<Geometry<VertexTextureData>> geometry) {
	constexpr float UV = 1.0f;

	vector<VertexTextureData> vertices;
	vertices.resize(4);

	vertices[0].Position = Vec3{ -0.5f, -0.5f, 0.0f };
	vertices[0].UV = Vec2{ 0.0f, UV };

	vertices[1].Position = Vec3{ -0.5, 0.5f, 0 };
	vertices[1].UV = Vec2{ 0.0f, 0.0f };

	vertices[2].Position = Vec3{ 0.5f, -0.5f, 0 };
	vertices[2].UV = Vec2{ UV, UV };

	vertices[3].Position = Vec3{ 0.5f, 0.5f, 0 };
	vertices[3].UV = Vec2{ UV, 0.0f };

	geometry->SetVertices(vertices);

	const vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndicies(indices);
}
