#include "GL/freeglut.h"

#include "Node.h"

Node::Node(const Vec3& position)
{
	isFixed = false;
	m_force = Vec3();
	m_mass = 1.0f;
	m_acceleration = Vec3();
	m_velocity = Vec3();
	m_position = position;
	m_normal = Vec3();
}

void Node::applyForce(const Vec3& force)
{
	m_force += force;
}

void Node::update(const float& deltaTime)
{
	if (!isFixed)
	{
		m_acceleration = m_force / m_mass;
		m_velocity += m_acceleration * deltaTime;
		m_position += m_velocity * deltaTime;
	}

	m_force.setZero();
}

void Node::render()
{
	glDisable(GL_LIGHTING);
	glColor3f(0.9f, 0.9f, 0.15f);
	glPointSize(2.0f);

	glBegin(GL_POINTS);
	glVertex3f(m_position.x, m_position.y, m_position.z);
	glEnd();
	glEnable(GL_LIGHTING);
}
