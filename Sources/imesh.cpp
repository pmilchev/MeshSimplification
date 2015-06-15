#include "imesh.h"

#include <algorithm> // min & max
#include <fstream>

#include <deque>


using std::vector;
using std::string;
using std::deque;
using std::ifstream;
using std::min;
using std::max;
using std::cout;
using std::endl;

void error(const string & name)
{
	cout << "\"" << name << "\"" << " is not *.off text file" << endl;
}

void LoadOffFile(const string& name, IndexedFaceSet& result)
{
	vector<Vector3> vertices;
	vector<Triangle> triangles;

	string line;
	ifstream myfile(name);
	int state = 0;
	int vcount = 0;
	int tcount = 0;
	int ecount = 0;
	int t = 0;
	float ft = 0.0;

	// step 0
	getline(myfile, line);
	if (line.compare("OFF") != 0) return error(name);

	// step 1
	myfile >> vcount >> tcount >> ecount;
	vertices.resize(vcount);
	triangles.resize(tcount);

	// step 2
	for (int i = 0; i < vcount; i++){
		float v1, v2, v3;
		myfile >> v1 >> v2 >> v3;
		vertices[i] = Vector3(v1, v2, v3);
	}

	// step 3
	for (int i = 0; i < tcount; i++){
		myfile >> t;
		triangles[i].vertices.resize(t);
		for (int j = 0; j < t; j++){
			myfile >> triangles[i].vertices[j];
		}
		string dummy;
		getline(myfile, dummy);
	}

	// step 4 : centering the mesh
	float minx, miny, minz, maxx, maxy, maxz;
	float dx = 0, dy = 0, dz = 0;
	if (vertices.size()){
		minx = maxx = vertices[0].f[0];
		miny = maxy = vertices[0].f[1];
		minz = maxz = vertices[0].f[2];
	}

	for (unsigned i = 1; i < vertices.size(); i++){
		minx = min(minx, vertices[i].f[0]);
		maxx = max(maxx, vertices[i].f[0]);
		miny = min(miny, vertices[i].f[1]);
		maxy = max(maxy, vertices[i].f[1]);
		minz = min(minz, vertices[i].f[2]);
		maxz = max(maxz, vertices[i].f[2]);
	}

	dx = (maxx + minx) / 2.0f;
	dy = (maxy + miny) / 2.0f;
	dz = (maxz + minz) / 2.0f;
	for (unsigned i = 0; i < vertices.size(); i++){
		vertices[i].x -= dx;
		vertices[i].y -= dy;
		vertices[i].z -= dz;
	}

	result.m_Vertices = vertices;
	result.m_Triangles = triangles;
}

int simple_triangulation(const IndexedFaceSet& inMesh,
	IndexedFaceSet& outMesh)
{
	outMesh.m_Vertices.clear();
	unsigned long long size = inMesh.GetTriangles().size();
	vector<int> v; v.resize(3);
	int c[3] = { 255, 0, 0 };
	int isAlreadyTriangulated = 1;
	for (unsigned long long i = 0; i < size; i++){
		v[0] = inMesh.GetTriangles()[i].vertices[0];
		for (unsigned k = 2; k < inMesh.GetTriangles()[i].vertices.size(); k++){
			v[1] = inMesh.GetTriangles()[i].vertices[k - 1];
			v[2] = inMesh.GetTriangles()[i].vertices[k];
			outMesh.GetTriangles().push_back(Triangle(v, c));
			isAlreadyTriangulated = 0;
		}
	}
	outMesh.GetVertices() = inMesh.GetVertices();
	return isAlreadyTriangulated;
}

int decimation1(const vector<Vector3>& inVertices, const vector<Triangle>& inTriangles,
	vector<Vector3>& outVertices, vector<Triangle>& outTriangles)
{
	// cluster sampling
	float minx, miny, minz, maxx, maxy, maxz;
	float dx = 0, dy = 0, dz = 0;
	if (inVertices.size()){
		minx = maxx = inVertices[0].f[0];
		miny = maxy = inVertices[0].f[1];
		minz = maxz = inVertices[0].f[2];
	}
	else return 1;
	for (unsigned i = 1; i < inVertices.size(); i++){
		minx = min(minx, inVertices[i].f[0]);
		maxx = max(maxx, inVertices[i].f[0]);
		miny = min(miny, inVertices[i].f[1]);
		maxy = max(maxy, inVertices[i].f[1]);
		minz = min(minz, inVertices[i].f[2]);
		maxz = max(maxz, inVertices[i].f[2]);
	}
	dx = 0.01f*(maxx - minx);
	dy = 0.01f*(maxy - miny);
	dz = 0.01f*(maxz - minz);

	vector <int> in2out; in2out.resize(inVertices.size(), -1);
	vector <int> outCounter;
	outVertices.resize(0);
	float x1, x2, y1, y2, z1, z2;
	for (unsigned i = 0; i < inVertices.size(); i++){
		if (in2out[i] != -1) continue;

		outVertices.push_back(inVertices[i]);
		outCounter.push_back(1);
		in2out[i] = outCounter.size() - 1;

		x1 = floor(inVertices[i].f[0] / dx) * dx; x2 = x1 + dx;
		y1 = floor(inVertices[i].f[1] / dy) * dy; y2 = y1 + dy;
		z1 = floor(inVertices[i].f[2] / dz) * dz; z2 = z1 + dz;
		for (unsigned j = i + 1; j < inVertices.size(); j++){
			if ((x1 < inVertices[j].f[0]) && (inVertices[j].f[0] <= x2) &&
				(y1 < inVertices[j].f[1]) && (inVertices[j].f[1] <= y2) &&
				(z1 < inVertices[j].f[2]) && (inVertices[j].f[2] <= z2))
			{
				int t = outVertices.size() - 1;
				in2out[j] = t;
				outVertices[t].f[0] += inVertices[j].f[0];
				outVertices[t].f[1] += inVertices[j].f[1];
				outVertices[t].f[2] += inVertices[j].f[2];
				outCounter[t] += 1;
			}
		}
	}
	// davame realni stojnosti na novite vyrhove
	for (unsigned i = 0; i < outVertices.size(); i++){
		outVertices[i].f[0] /= outCounter[i];
		outVertices[i].f[1] /= outCounter[i];
		outVertices[i].f[2] /= outCounter[i];
	}

	int a, b, c;
	int color[3];
	for (unsigned i = 0; i < inTriangles.size(); i++){
		a = in2out[inTriangles[i].vertices[0]];
		b = in2out[inTriangles[i].vertices[1]];
		c = in2out[inTriangles[i].vertices[2]];
		vector<int> newVert; newVert.push_back(a); newVert.push_back(b); newVert.push_back(c);
		color[0] = inTriangles[i].color[0];
		color[1] = inTriangles[i].color[1];
		color[2] = inTriangles[i].color[2];
		if (a != b && b != c && c != a){
			outTriangles.push_back(Triangle(newVert, color));
		}
	}



	return 0;
}

int getVertRelative(int to, int offset, const Triangle& intriangle)
{
	int start = -1;
	int result = -1;
	for (unsigned i = 0; i < intriangle.vertices.size(); i++){
		if (intriangle.vertices[i] == to){
			start = i;
		}
	}
	result = intriangle.vertices[((start + offset) % intriangle.vertices.size())];
	return result;
}


int rearangeTrianglesInFan(const vector<Triangle>& inTriangles, const int& v, vector <int>& trianglesOrder)
{
	int result = VertexClass::Complex;
	deque <int> newTrianglesOrder;
	vector <int> usedTrianglesOrder;
	usedTrianglesOrder.resize(trianglesOrder.size(), 0);
	int leftmostV, rightmostV, leftmostT, rightmostT;

	newTrianglesOrder.push_back(trianglesOrder[0]);
	leftmostT = newTrianglesOrder.front();
	rightmostT = newTrianglesOrder.back();
	leftmostV = getVertRelative(v, 2, inTriangles[leftmostT]);
	rightmostV = getVertRelative(v, 1, inTriangles[rightmostT]);
	usedTrianglesOrder[0] = 1;

	// obhojdame tyrsejki obratno na chasownikowata posoka/nalqwo

	int oldLeftmostV;
	do{
		oldLeftmostV = leftmostV;
		for (unsigned i = 0; i < trianglesOrder.size(); i++){
			if (!usedTrianglesOrder[i] &&
				leftmostV == getVertRelative(v, 1, inTriangles[trianglesOrder[i]]))
			{
				leftmostT = trianglesOrder[i];
				leftmostV = getVertRelative(v, 2, inTriangles[leftmostT]);
				usedTrianglesOrder[i] = 1;
				newTrianglesOrder.push_front(leftmostT);
			}
		}
	} while (oldLeftmostV != leftmostV);

	int oldRightmostV;
	do{
		oldRightmostV = rightmostV;
		for (unsigned i = 0; i < trianglesOrder.size(); i++){
			if (!usedTrianglesOrder[i] &&
				rightmostV == getVertRelative(v, 2, inTriangles[trianglesOrder[i]]))
			{
				rightmostT = trianglesOrder[i];
				rightmostV = getVertRelative(v, 1, inTriangles[rightmostT]);
				usedTrianglesOrder[i] = 1;
				newTrianglesOrder.push_back(rightmostT);
			}
		}
	} while (oldRightmostV != rightmostV);

	// ako sa ostanali neobhodeni triygylnici -> VertexClass::Complex
	for (unsigned i = 0; i < usedTrianglesOrder.size(); i++){
		if (usedTrianglesOrder[i] == 0)
		{
			return VertexClass::Complex;
		}
	}

	// nowopodredenite triygylnici
	trianglesOrder = vector<int>(newTrianglesOrder.begin(), newTrianglesOrder.end());
	if (leftmostV == rightmostV){
		result = VertexClass::Simple;
	}
	else{
		result = VertexClass::Boundary;
	}

	return result;
}


int decimation2(const vector<Vector3>& inVertices, const vector<Triangle>& inTriangles,
	vector<Vector3>& outVertices, vector<Triangle>& outTriangles)
{
	if (inVertices.size() < 5) return 0;
	// ----------------------------------------------------------------------------------------------------------------------
	// Steps of the algorithm "Decimation of Triangle Meshes - William J. Schroeder, Jonathan A. Zarge, William E. Lorensen":
	// 1. characterize the local vertex geometry and topology,
	// 2. evaluate the decimation criteria, and
	// 3. triangulate the resulting hole.
	// ----------------------------------------------------------------------------------------------------------------------
	vector<Vector3> inV = inVertices;
	vector<Triangle> inT = inTriangles;

	//	while (inV.size() > (0.9*inVertices.size())){ // start the LOOP
	//	int cnt = max(1, (int)(0.02f * inVertices.size()));
	int cnt = 1;
	while (cnt--) {

		// 1. clasification 
		vector< int > vertexClass;
		// clasification preparation
		//  a) za wseki wryh pazim chast ot koi triygylnici e
		//  b) za wseki edge pazim broj triygylnici w koito uchastwa
		//  ->ako wyrhowete uchastwat w zatworeno wetrilo ot triygylnici + drug triygylnik = VertexClass::Complex
		//  ->ako edge-owete uchastvat w poweche ot 2 triygylnika vertex-ite koito gi systawqt = VertexClass::Complex
		//  =>wsichki drugi sa kandidati
		vector< vector < int > > vertexFromTriangle;// [#vertex] = {#tri1,#tri2 ...};
		//		vector< vector < int > > trianglesForEdge;  // [#minOfEdgeVertex][#minOfEdgeVertex] = {countOfTriangles};

		// init
		vertexClass.resize(inV.size(), VertexClass::Undefined);
		vertexFromTriangle.resize(inV.size());

		vector< Edge > edges;
		vector< Edge > edges1;

		for (unsigned i = 0; i < inT.size(); i++){
			//  a) za wseki wryh pazim chast ot koi triygylnici e
			vertexFromTriangle[inT[i].vertices[0]].push_back(i);
			vertexFromTriangle[inT[i].vertices[1]].push_back(i);
			vertexFromTriangle[inT[i].vertices[2]].push_back(i);
			//  b) za wseki edge pazim broj triygylnici w koito uchastwa

			for (unsigned j = 0; j < 3; j++)
			{
				Edge e(inT[i].vertices[j], inT[i].vertices[(j + 1) % 3]);
				edges.push_back(e);
			}

		}

		int totalEdges = 0;
		sort(edges.begin(), edges.end());
		unsigned ei = 0;
		while (ei < edges.size())
		{
			const Edge & e = edges[ei];
			unsigned j = ei + 1;
			while (j < edges.size() && e == edges[j]) j++;
			for (unsigned k = ei; k < j; k++) edges[k].n = j - ei;
			ei = j;
			totalEdges++;
		}

		edges1.resize(totalEdges);
		ei = 0;
		int eidx = 0;
		while (ei < edges.size())
		{
			edges1[eidx++] = edges[ei];
			ei += edges[ei].n;
		}

		// pass 1 ->ako edge-owete uchastvat w poweche ot 2 triygylnika vertex-ite koito gi systawqt = VertexClass::Complex
		for (unsigned ei = 0; ei < edges1.size(); ei++)
		{
			if (edges1[ei].n > 2)
			{
				vertexClass[edges1[ei].v0] = VertexClass::Complex;
				vertexClass[edges1[ei].v1] = VertexClass::Complex;
			}
		}

		int dummy = 0;

		// pass 2 -> prepodrejdane na triygylnicite wyw wetrilo
		for (unsigned i = 0; i < inV.size(); i++){
			if (vertexClass[i] == VertexClass::Undefined){
				vertexClass[i] = rearangeTrianglesInFan(inT, i, vertexFromTriangle[i]);
			}
		}

		// 2. evaluate the decimation criteria
		float minDist = 1000000;
		int   removeV = -1;
		for (unsigned i = 0; i < vertexClass.size(); i++){
			switch (vertexClass[i]){
			case VertexClass::Complex:
				break;
			case VertexClass::Simple:
			{
				float dist = fabs(analizeSimpleVertex(inV, inT, i, vertexFromTriangle[i]));
				if (dist < minDist){
					minDist = dist;
					removeV = i;
				}
			}
				break;
			case VertexClass::Boundary:
			{
				int v0 = getVertRelative(i, 2, inT[vertexFromTriangle[i][0]]);
				int v1 = getVertRelative(i, 1, inT[vertexFromTriangle[i][vertexFromTriangle[i].size() - 1]]);
				float dist = fabsf(analyzeBoundaryVertex(inV, i, v0, v1));
				if (dist < minDist){
					minDist = dist;
					removeV = i;
				}
			}
				break;
			}
		}

		if (removeV == -1)
		{
			removeV = 1;
		}

		if (vertexClass[removeV] == VertexClass::Boundary)
		{
			vector<int> & faces = vertexFromTriangle[removeV];
			vector<int> verts;
			verts.resize(faces.size() + 1);
			verts[0] = getVertRelative(removeV, 2, inT[faces[0]]);
			for (unsigned i = 0; i < faces.size(); i++)
			{
				verts[i + 1] = getVertRelative(removeV, 1, inT[faces[i]]);
			}

			unsigned sz = inT.size();
			outTriangles.clear();
			outTriangles.resize(sz - 1);
			int idx = 0;
			for (unsigned i = 0; i < sz; i++)
			{
				unsigned j;
				for (j = 0; j < faces.size(); j++)
				{
					if (i == faces[j]) break;
				}
				if (j < faces.size()) continue;

				outTriangles[idx++] = inT[i];
			}

			int v0 = verts[0];
			for (unsigned i = 2; i < verts.size(); i++)
			{
				outTriangles[idx].vertices.push_back(verts[0]);
				outTriangles[idx].vertices.push_back(verts[i]);
				outTriangles[idx].vertices.push_back(verts[i - 1]);
				idx++;
			}
		}
		else
		{
			int sz = inT.size();
			outTriangles.clear();
			outTriangles.resize(sz - 2);
			int idx = 0;
			for (int i = 0; i < sz; i++)
			{
				unsigned j;
				for (j = 0; j < vertexFromTriangle[removeV].size(); j++)
				{
					if (i == vertexFromTriangle[removeV][j]) break;
				}
				if (j < vertexFromTriangle[removeV].size()) continue;

				outTriangles[idx++] = inT[i];
			}

			int v0 = getVertRelative(removeV, 1, inT[vertexFromTriangle[removeV][0]]);
			for (unsigned i = 2; i < vertexFromTriangle[removeV].size(); i++)
			{
				int v1 = getVertRelative(removeV, 1, inT[vertexFromTriangle[removeV][i]]);
				int v2 = getVertRelative(removeV, 2, inT[vertexFromTriangle[removeV][i]]);

				outTriangles[idx].vertices.push_back(v0);
				outTriangles[idx].vertices.push_back(v1);
				outTriangles[idx].vertices.push_back(v2);
				idx++;
			}
		}

		for (unsigned i = 0; i < outTriangles.size(); i++)
		{
			if (outTriangles[i].vertices[0] >= removeV) outTriangles[i].vertices[0]--;
			if (outTriangles[i].vertices[1] >= removeV) outTriangles[i].vertices[1]--;
			if (outTriangles[i].vertices[2] >= removeV) outTriangles[i].vertices[2]--;
		}

		outVertices.clear();
		outVertices.resize(inV.size() - 1);
		int idx = 0;
		for (unsigned i = 0; i < inV.size(); i++)
		{
			if (i == removeV) continue;
			outVertices[idx++] = inV[i];
		}

		int * degs = new int[outVertices.size()];
		memset(degs, 0, sizeof(int) * outVertices.size());
		for (unsigned i = 0; i < outTriangles.size(); i++)
		{
			degs[outTriangles[i].vertices[0]]++;
			degs[outTriangles[i].vertices[1]]++;
			degs[outTriangles[i].vertices[2]]++;
		}

		for (unsigned i = 0; i < outVertices.size(); i++)
		{
			if (degs[i] == 0)
			{
				degs[i] = 0;
			}
		}

		delete[] degs;

	}// LOOP

	return 0;
}

float analizeSimpleVertex(const vector<Vector3>& inVertices, const vector<Triangle>& inTriangles,
	int ve, const vector<int>& faces)
{
	int sz = faces.size();

	Vector3 N(0.0f, 0.0f, 0.0f);
	Vector3 B(0.0f, 0.0f, 0.0f);
	float totalArea = 0.0f;

	for (int i = 0; i < sz; i++)
	{
		Vector3 v[3] = {
			inVertices[inTriangles[faces[i]].vertices[0]],
			inVertices[inTriangles[faces[i]].vertices[1]],
			inVertices[inTriangles[faces[i]].vertices[2]]
		};
		Vector3 c = (v[0] + v[1] + v[2]) / 3.0f;
		Vector3 e0 = v[1] - v[0];
		Vector3 e1 = v[2] - v[0];
		float area = (e0 ^ e1).len() * 0.5f;
		totalArea += area;
		B += c * area;

		Vector3 n = e0 ^ e1;
		n.normalize();
		N += area * n;
	}
	B /= totalArea;
	N.normalize();
	Vector3 V = inVertices[ve];
	float result = fabsf(N * (V - B));
	return result;
}

float analyzeBoundaryVertex(const vector<Vector3>& inVertices,
	int v, int v0, int v1)
{
	Vector3 verts[3] = {
		inVertices[v],
		inVertices[v0],
		inVertices[v1]
	};
	float area = ((verts[1] - verts[0]) ^ (verts[2] - verts[0])).len() * 0.5f;
	float l = (verts[0] - verts[1]).len();
	return 2.0f * area / l;
}