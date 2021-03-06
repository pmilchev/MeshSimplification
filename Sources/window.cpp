#include "window.h"

#include "vector3.h"
#include <algorithm> // max

const float Window::RADIUS = 4.0f;
const float Window::angle = -60.f;

void Window::getNormal3V(const float& A1, const float& A2, const float& A3,
	const float& B1, const float& B2, const float& B3,
	const float& C1, const float& C2, const float& C3,
	float& n1, float& n2, float& n3)
{
	// calculates the normal vector n(n1,n2,n3) as a result
	// by given points A(A1,A2,A3), B(B1,B2,B3), C(C1,C2,C3) in anticlockwise order
	float a1, a2, a3, b1, b2, b3;
	a1 = B1 - A1;
	a2 = B2 - A2;
	a3 = B3 - A3;

	b1 = C1 - B1;
	b2 = C2 - B2;
	b3 = C3 - B3;

	n1 = a2*b3 - a3*b2;
	n2 = a3*b1 - a1*b3;
	n3 = a1*b2 - a2*b1;
	return;
}

GLuint Window::make_mesh(const IndexedFaceSet& mesh)//const vector<Vector3>& vertices, const vector<Triangle>& triangles){
{
	GLuint list;
	//GLfloat a, b;
	GLfloat da = 18.0, db = 18.0;
	GLfloat radius = 1.0;
	GLuint color;
	float n1, n2, n3;

	list = glGenLists(1);

	glNewList(list, GL_COMPILE);

	glColor3f(1.0, 0.0, 0.0);
	color = 0;
	for (unsigned i = 0; i < mesh.GetTriangles().size(); i++) {

		glBegin(GL_POLYGON);
		getNormal3V(
			mesh.GetVertices()[mesh.GetTriangles()[i].vertices[0]].f[0],
			mesh.GetVertices()[mesh.GetTriangles()[i].vertices[0]].f[1],
			mesh.GetVertices()[mesh.GetTriangles()[i].vertices[0]].f[2],
			mesh.GetVertices()[mesh.GetTriangles()[i].vertices[1]].f[0],
			mesh.GetVertices()[mesh.GetTriangles()[i].vertices[1]].f[1],
			mesh.GetVertices()[mesh.GetTriangles()[i].vertices[1]].f[2],
			mesh.GetVertices()[mesh.GetTriangles()[i].vertices[2]].f[0],
			mesh.GetVertices()[mesh.GetTriangles()[i].vertices[2]].f[1],
			mesh.GetVertices()[mesh.GetTriangles()[i].vertices[2]].f[2],
			n1, n2, n3);
		glNormal3f(n1, n2, n3);
		for (unsigned j = 0; j < mesh.GetTriangles()[i].vertices.size(); j++)
		{
			int vindex = mesh.GetTriangles()[i].vertices[j];
			glVertex3f(mesh.GetVertices()[vindex].f[0], mesh.GetVertices()[vindex].f[1], mesh.GetVertices()[vindex].f[2]);
		}
		glEnd();
		color = 1 - color;

	}

	glEndList();

	return list;
}

Window* Window::windowInstance = nullptr;

void Window::Init(int argc, char *argv[])
{
	if (windowInstance == nullptr)
		windowInstance = this;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 640);

	glutCreateWindow("Pavel Milchev - Decimation of triangular mesh");

	myinit();

	figure = 0;
}

void Window::Uninit()
{
	windowInstance = nullptr;
}

void Window::MainLoop()
{
	glutDisplayFunc(&Window::drawCallback);
	glutKeyboardFunc(&Window::keyCallback);
	glutReshapeFunc(reshape);

	glutMainLoop();
}

void Window::myinit(void)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	//Disable color materials, so that glMaterial calls work
	glDisable(GL_COLOR_MATERIAL);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
}

void Window::draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//printf("Mesh[%d] v# = %d, t# = %d\n", figure, indexedFaceSets[figure].GetVertices().size(), indexedFaceSets[figure].GetTriangles().size());

	light();

	material();
	glCallList(Mesh[figure]);

	glFlush();
	glutSwapBuffers();
}

void Window::drawCallback()
{
	if (windowInstance != nullptr)
		windowInstance->draw();
}

void Window::material(void)
{
	//The color of the sphere
	GLfloat materialColor[] = { 0.7f, 0.8f, 0.7f, 1.0f };
	//The specular (shiny) component of the material
	GLfloat materialSpecular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	//The color emitted by the material
	GLfloat materialEmission[] = { 0.05f, 0.05f, 0.05f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
	glMaterialf(GL_FRONT, GL_SHININESS, 12); //The shininess parameter
}

void Window::light(void)
{
	GLfloat lightPosition[] = { 4.0f, -3.5f, 0.0f, 1.0f };

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.0f);
	glRotatef(angle, 0, 1, 0);

	GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat lightColor[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat lightPos[] = { 1.5f * RADIUS, 2.f * RADIUS, 1.5f * RADIUS, 1.0f };
	//Diffuse (non-shiny) light component
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	//Specular (shiny) light component
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glPopMatrix();
}

void Window::reshape(int width, int height)
{
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-3.0, 3.0, -3.0, 3.0, 64, 256);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -100);
}

void Window::keyCallback(unsigned char k, int x, int y)
{
	if (windowInstance != nullptr)
		windowInstance->key(k, x, y);
}

void Window::key(unsigned char k, int x, int y)
{
	switch (k) {
	case 27:  /* Escape */
		exit(0);
	case 48:  // 0 
		figure = 0;
		glutPostRedisplay();
		break;
	case 49:  // 1
		figure = 1;
		glutPostRedisplay();
		break;
	case 50:  // 2
		figure = 2;
		glutPostRedisplay();
		break;
	case 51:  // 3
		figure = 3;
		glutPostRedisplay();
		break;
	case 115: // S
		glRotatef(5.0f, 1.0f, 0.0f, 0.0f);
		glutPostRedisplay();
		break;
	case 119: // W
		glRotatef(-5.0f, 1.0f, 0.0f, 0.0f);
		glutPostRedisplay();
		break;
	case 97: // A
		glRotatef(5.0f, 0.0f, 1.0f, 0.0f);
		glutPostRedisplay();
		break;
	case 100: // D
		glRotatef(-5.0f, 0.0f, 1.0f, 0.0f);
		glutPostRedisplay();
		break;
	case 113: // Q
		glScalef(0.8f, 0.8f, 0.8f);
		glutPostRedisplay();
		break;
	case 101: // E
		glScalef(1.25f, 1.25f, 1.25f);
		glutPostRedisplay();
		break;
	case 114: // R
	{
		figure = 3;

		int cnt = std::max(1, (int)(0.1f * indexedFaceSets[1].GetVertices().size()));
		int total = cnt;
		while (cnt--)
		{
			printf("%d out of %d\n", total - cnt, total);
			decimation2(indexedFaceSets[3].GetVertices(), indexedFaceSets[3].GetTriangles(),
				indexedFaceSets[4].GetVertices(), indexedFaceSets[4].GetTriangles());
			indexedFaceSets[3].GetVertices() = indexedFaceSets[4].GetVertices();
			indexedFaceSets[3].GetTriangles() = indexedFaceSets[4].GetTriangles();
		}
		Mesh[3] = make_mesh(indexedFaceSets[3]);

		glutPostRedisplay();
		break;
	}
	default:
		printf("%d\n", k);
	}
}