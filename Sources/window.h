#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <stdlib.h>
#include <glut.h>
#include <freeglut.h>
#include <algorithm> // min & max


#include <vector>
#include "vector3.h"

#include "imesh.h"

class Window
{
public:
	static GLuint make_mesh(const IndexedFaceSet& mesh);
	static void getNormal3V(const float& A1, const float& A2, const float& A3,
		const float& B1, const float& B2, const float& B3,
		const float& C1, const float& C2, const float& C3,
		float& n1, float& n2, float& n3);

	// global data
	static const unsigned MAX_FIGURES = 10;
	IndexedFaceSet indexedFaceSets[MAX_FIGURES];

	int figure;
	GLuint Mesh[MAX_FIGURES];
	static const float angle;

	void Init(int argc, char *argv[]);
	void Uninit();
	void MainLoop();
private:
	static const float RADIUS; //The radius of the sphere
	void myinit(void);
	static Window* windowInstance;
	static void drawCallback();
	void draw(void);
	void material(void);
	void light(void);
	static void reshape(int width, int height);
	static void keyCallback(unsigned char k, int x, int y);
	void key(unsigned char k, int x, int y);
};

#endif // __WINDOW_H__
