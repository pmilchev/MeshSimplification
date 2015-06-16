#include <string>
#include <iostream>

#include "imesh.h"
#include "vector3.h"
#include "window.h"

using std::cout;
using std::endl;
using std::string;

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