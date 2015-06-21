#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <stdlib.h>
#include <glut.h>
#include <freeglut.h>
#include <algorithm> // min & max


#include <vector>
#include "vector3.h"

#include "imesh.h"

//int decimation2(const vector<Vector3>& inVertices,
//	const vector<Triangle>& inTriangles,
//	vector<Vector3>& outVertices, 
//	vector<Triangle>& outTriangles);



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
	//{
	//	if (windowInstance == nullptr)
	//		windowInstance = this;

	//	glutInit(&argc, argv);

	//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//	glutInitWindowSize(640, 640);

	//	glutCreateWindow("Pavel Milchev - Decimation of triangular mesh");

	//	myinit();

	//	figure = 0;
	//}

	void Uninit();
	//{
	//	windowInstance = nullptr;
	//}

	void MainLoop();
	//{
	//	glutDisplayFunc(&Window::drawCallback);
	//	glutKeyboardFunc(&Window::keyCallback);
	//	glutReshapeFunc(reshape);

	//	glutMainLoop();
	//}
private:
	static const float RADIUS; //The radius of the sphere
	void myinit(void);
	//{
	//	glEnable(GL_DEPTH_TEST);
	//	glEnable(GL_LIGHTING);
	//	glEnable(GL_LIGHT0);
	//	glEnable(GL_NORMALIZE);
	//	glShadeModel(GL_SMOOTH);
	//	//Disable color materials, so that glMaterial calls work
	//	glDisable(GL_COLOR_MATERIAL);

	//	glDepthFunc(GL_LEQUAL);
	//	glEnable(GL_DEPTH_TEST);
	//}

	static Window* windowInstance;
	static void drawCallback();
	//{
	//	if (windowInstance != nullptr)
	//		windowInstance->draw();
	//}
	void draw(void);
	//{
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	//printf("Mesh[%d] v# = %d, t# = %d\n", figure, indexedFaceSets[figure].GetVertices().size(), indexedFaceSets[figure].GetTriangles().size());

	//	light();

	//	material();
	//	glCallList(Mesh[figure]);

	//	glFlush();
	//	glutSwapBuffers();
	//}

	void material(void);
	//{
	//	//The color of the sphere
	//	GLfloat materialColor[] = { 0.7f, 0.8f, 0.7f, 1.0f };
	//	//The specular (shiny) component of the material
	//	GLfloat materialSpecular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	//	//The color emitted by the material
	//	GLfloat materialEmission[] = { 0.05f, 0.05f, 0.05f, 1.0f };

	//	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
	//	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	//	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
	//	glMaterialf(GL_FRONT, GL_SHININESS, 12); //The shininess parameter
	//}

	void light(void);
	//{
	//	GLfloat lightPosition[] = { 4.0f, -3.5f, 0.0f, 1.0f };
	//	
	//	glPushMatrix();
	//	glLoadIdentity();
	//	glTranslatef(0.0f, 0.0f, -10.0f);
	//	glRotatef(angle, 0, 1, 0);

	//	GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	//	GLfloat lightColor[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	//	GLfloat lightPos[] = { 1.5f * RADIUS, 2.f * RADIUS, 1.5f * RADIUS, 1.0f };
	//	//Diffuse (non-shiny) light component
	//	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	//	//Specular (shiny) light component
	//	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//	glPopMatrix();
	//}

	static void reshape(int width, int height);
	//{
	//	glViewport(0, 0, (GLint)width, (GLint)height);
	//	glMatrixMode(GL_PROJECTION);
	//	glLoadIdentity();

	//	glMatrixMode(GL_PROJECTION);
	//	glLoadIdentity();
	//	glFrustum(-3.0, 3.0, -3.0, 3.0, 64, 256);

	//	glMatrixMode(GL_MODELVIEW);
	//	glLoadIdentity();
	//	glTranslatef(0.0, 0.0, -100);
	//}

	static void keyCallback(unsigned char k, int x, int y);
	//{
	//	if (windowInstance != nullptr)
	//		windowInstance->key(k,x,y);
	//}

	void key(unsigned char k, int x, int y);
	//{
	//	switch (k) {
	//	case 27:  /* Escape */
	//		exit(0);
	//	case 48:  // 0 
	//		figure = 0;
	//		glutPostRedisplay();
	//		break;
	//	case 49:  // 1
	//		figure = 1;
	//		glutPostRedisplay();
	//		break;
	//	case 50:  // 2
	//		figure = 2;
	//		glutPostRedisplay();
	//		break;
	//	case 51:  // 3
	//		figure = 3;
	//		glutPostRedisplay();
	//		break;
	//	case 115: // S
	//		glRotatef(5.0f, 1.0f, 0.0f, 0.0f);
	//		glutPostRedisplay();
	//		break;
	//	case 119: // W
	//		glRotatef(-5.0f, 1.0f, 0.0f, 0.0f);
	//		glutPostRedisplay();
	//		break;
	//	case 97: // A
	//		glRotatef(5.0f, 0.0f, 1.0f, 0.0f);
	//		glutPostRedisplay();
	//		break;
	//	case 100: // D
	//		glRotatef(-5.0f, 0.0f, 1.0f, 0.0f);
	//		glutPostRedisplay();
	//		break;
	//	case 113: // Q
	//		glScalef(0.8f, 0.8f, 0.8f);
	//		glutPostRedisplay();
	//		break;
	//	case 101: // E
	//		glScalef(1.25f, 1.25f, 1.25f);
	//		glutPostRedisplay();
	//		break;
	//	case 114: // R
	//	{
	//		figure = 3;

	//		int cnt = std::max(1, (int)(0.1f * indexedFaceSets[1].GetVertices().size()));
	//		int total = cnt;
	//		while (cnt--)
	//		{
	//			printf("%d out of %d\n", total - cnt, total);
	//			decimation2(indexedFaceSets[3].GetVertices(), indexedFaceSets[3].GetTriangles(),
	//				indexedFaceSets[4].GetVertices(), indexedFaceSets[4].GetTriangles());
	//			indexedFaceSets[3].GetVertices() = indexedFaceSets[4].GetVertices();
	//			indexedFaceSets[3].GetTriangles() = indexedFaceSets[4].GetTriangles();
	//		}
	//		Mesh[3] = make_mesh(indexedFaceSets[3]);

	//		glutPostRedisplay();
	//		break;
	//	}
	//	default:
	//		printf("%d\n", k);
	//	}
	//}

};

#endif // __WINDOW_H__
