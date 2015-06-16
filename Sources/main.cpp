//#include <math.h>
//#include <stdlib.h>
//#include <glut.h>
//#include <freeglut.h>

//#include <vector>
#include <string>
#include <iostream>
//#include <algorithm>

#include "imesh.h"
#include "vector3.h"
#include "window.h"

using std::cout;
using std::endl;
using std::string;

// declaration of functions
//void getNormal3V(const float& A1, const float& A2, const float& A3,
//	const float& B1, const float& B2, const float& B3,
//	const float& C1, const float& C2, const float& C3,
//	float& n1, float& n2, float& n3);

//bool in(const vector<int> & v, int x)
//{
//	for (unsigned i = 0; i < v.size(); i++) if (x == v[i]) return true;
//	return false;
//}

int main(int argc, char *argv[])
{
	Window window;
	window.Init(argc, argv);

	string filename;
	if (argc == 2){
		filename = argv[1];
	}
	else
	{
		cout << "you have to give at least one argument: *.exe filename";
		return 1;
	}
	cout << filename << endl;
	IndexedFaceSet initialMesh;

	LoadOffFile(filename, initialMesh);

	window.figure = 0;
	window.Mesh[0] = Window::make_mesh(initialMesh);
	simple_triangulation(initialMesh, window.indexedFaceSets[1]);

	window.Mesh[1] = Window::make_mesh(window.indexedFaceSets[1]);

	decimation1(window.indexedFaceSets[1].GetVertices(), window.indexedFaceSets[1].GetTriangles(),
		window.indexedFaceSets[2].GetVertices(), window.indexedFaceSets[2].GetTriangles());
	window.Mesh[2] = Window::make_mesh(window.indexedFaceSets[2]);

	window.indexedFaceSets[3] = window.indexedFaceSets[1];
	//indexedFaceSets[3].GetVertices() = indexedFaceSets[1].GetVertices();
	//indexedFaceSets[3].GetTriangles() = indexedFaceSets[1].GetTriangles();

	window.Mesh[3] = window.Mesh[1];

	window.MainLoop();

	window.Uninit();
	return 0;
}