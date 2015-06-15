#ifndef __IMESH_H__
#define __IMESH_H__

#include <vector>
#include <algorithm> // min & max
#include "vector3.h"

// used structures
struct Triangle
{
	std::vector<int> vertices;
	int         color[3];
	Triangle(){ color[0] = color[1] = color[2] = 1; };
	Triangle(std::vector<int> vertices, int color[3]){ this->vertices = vertices; this->color[0] = color[0]; this->color[1] = color[1]; this->color[2] = color[2]; };
};

class IMesh
{
};

class IndexedFaceSet : public IMesh
{
public:
	const std::vector<Vector3>& GetVertices() const { return m_Vertices; }
	const std::vector<Triangle>& GetTriangles() const { return m_Triangles; }

	std::vector<Vector3>& GetVertices() { return m_Vertices; }
	std::vector<Triangle>& GetTriangles() { return m_Triangles; }
	//private:
	std::vector<Vector3> m_Vertices;
	std::vector<Triangle> m_Triangles;
};

struct Edge
{
	Edge(int _v0 = 0, int _v1 = 0)
	{
		n = 0;
		set(_v0, _v1);
	}

	void set(int _v0, int _v1)
	{
		v0 = std::max(_v0, _v1);
		v1 = std::min(_v0, _v1);
	}

	bool operator==(const Edge & other) const
	{
		return v0 == other.v0 && v1 == other.v1;
	}

	bool operator<(const Edge & other) const
	{
		return (v0 < other.v0 || (v0 == other.v0 && v1 < other.v1));
	}

	int v0, v1;
	int n;
};

#endif //__IMESH_H__