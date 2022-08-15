#pragma once
#include <Simplemath.h>

using namespace DirectX::SimpleMath;

struct Vertex {
	Vertex()
		:pos(0.f, 0.f, 0.f), texCoord(0.f, 0.f), normal(0.0f, 0.0f, 0.0f), tangent(0.0f, 0.0f, 0.0f) {}
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz, float bx, float by, float bz)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}
	Vertex(Vector3 pos, Vector2 texCoord, Vector3 normal, Vector3 tangent, Vector3 biTangent)
		:pos(pos), texCoord(texCoord), normal(normal), tangent(tangent) {}

	Vector3 pos;
	Vector2 texCoord;
	Vector3 normal;
	Vector3 tangent;
};

struct SimpleVertex
{
	SimpleVertex()
		:pos(0.f, 0.f), texCoord(0.f, 0.f) {}
	SimpleVertex(float x, float y, float u, float v)
		:pos(x, y),  texCoord(u, v) {}

	Vector2 pos;
	Vector2 texCoord;
};