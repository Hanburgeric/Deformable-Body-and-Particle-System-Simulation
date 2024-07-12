#include "GL/freeglut.h"

#include "Ground.h"

Ground::Ground(const Vec3& position)
{
	m_position = position;
	m_normal = Vec3(0.0f, 1.0f, 0.0f);
}

void Ground::render()
{
	float groundLength = 100.0f;
	float numHalfEdges = 64.0f;

	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (int x = 0; x < 2 * (int)numHalfEdges; x++)
	{
		for (int y = 0; y < 2 * (int)numHalfEdges; y++)
		{
			glNormal3f(0.0f, 1.0f, 0.0f);
			glVertex3f(-groundLength + groundLength / numHalfEdges * x, 0.0f, -groundLength + groundLength / numHalfEdges * y);
			glVertex3f(-groundLength + groundLength / numHalfEdges * (x + 1), 0.0f, -groundLength + groundLength / numHalfEdges * y);
			glVertex3f(-groundLength + groundLength / numHalfEdges * (x + 1), 0.0f, -groundLength + groundLength / numHalfEdges * (y + 1));
			glVertex3f(-groundLength + groundLength / numHalfEdges * x, 0.0f, -groundLength + groundLength / numHalfEdges * (y + 1));
		}
	}
	glEnd();
}
