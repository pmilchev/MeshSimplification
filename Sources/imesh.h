#ifndef __IMESH_H__
#define __IMESH_H__

#include <vector>
//#include <string>

class string;
//#include <algorithm> // min & max

//#include <iostream>
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

void error(const std::string & name);
void LoadOffFile(const std::string& name, IndexedFaceSet& result);
int simple_triangulation(const IndexedFaceSet& inMesh, IndexedFaceSet& outMesh);
int decimation1(const std::vector<Vector3>& inVertices, const std::vector<Triangle>& inTriangles,
	std::vector<Vector3>& outVertices, std::vector<Triangle>& outTriangles);
int decimation2(const std::vector<Vector3>& inVertices, const std::vector<Triangle>& inTriangles,
	std::vector<Vector3>& outVertices, std::vector<Triangle>& outTriangles);
int rearangeTrianglesInFan(const std::vector<Triangle>& inTriangles, const int& v, std::vector <int>& trianglesOrder);

int getVertRelative(int to, int offset, const Triangle& intriangle);
float analyzeBoundaryVertex(const std::vector<Vector3>& inVertices,
	int v, int v0, int v1);
float analizeSimpleVertex(const std::vector<Vector3>& inVertices, const std::vector<Triangle>& inTriangles,
	int ve, const std::vector<int>& faces);


enum VertexClass
{
	Undefined = -1,
	Complex = 0,
	Boundary = 1,
	Simple = 2
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