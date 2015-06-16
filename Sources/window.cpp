#include "window.h"

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
