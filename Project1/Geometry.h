#pragma once

// Geometry를 구성할 때 필요한 것들은 버텍스와 인덱스 정보들이다.

template <typename TVertex>
class Geometry
{
public:
	Geometry() {}
	~Geometry() {}

	uint32 GetVertexCount() { return static_cast<uint32>(_vertices.size()); }
	void* GetVertexData() { return _vertices.data(); }
	const vector<TVertex>& GetVertices() { return _vertices; }

	uint32 GetIndexCount() { return static_cast<uint32>(_indices.size()); }
	void* GetIndexData() { return _indices.data(); }
	const vector<uint32>& GetIndices() { return _indices; }

	void AddVertex(const TVertex& vertex) { _vertices.push_back(vertex); }
	void AddVertices(const vector<TVertex>& vertices) { _vertices.insert(_vertices.end(), vertices.begin(), vertices.end()); }
	void SetVertices(const vector<TVertex>& vertices) { _vertices = vertices; }

	void AddIndex(const uint32& index) { _indices.push_back(index); }
	void AddIndicies(const vector<uint32>& indices) { _indices.insert(_indices.end(), indices.begin(), indices.end()); }
	void SetIndicies(const vector<uint32>& indices) { _indices = indices; }
private:
	vector<TVertex> _vertices;
	vector<uint32> _indices;
};

