#include "GL/freeglut.h"

#include "Node.h"
#include "Spring.h"

Spring::Spring(Node* pNode1, Node* pNode2, const float& spring, const float& damping)
{
	m_pNode1 = pNode1;
	m_pNode2 = pNode2;
	m_spring = spring;
	m_damping = damping;
	m_initLength = (m_pNode1->m_position - m_pNode2->m_position).length();
}

void Spring::applyForce()
{
	//Apply spring and damping force
	Vec3 deltaVel = m_pNode2->m_velocity - m_pNode1->m_velocity;
	Vec3 deltaPos = m_pNode2->m_position - m_pNode1->m_position;
	float deltaPosLength = deltaPos.length();
	Vec3 unitDeltaPos = deltaPos / deltaPosLength;
	Vec3 force = unitDeltaPos * (m_spring * (deltaPosLength - m_initLength) + (m_damping * dotProduct(deltaVel, unitDeltaPos)));

	m_pNode1->applyForce(force);
	m_pNode2->applyForce(-1.0f * force);
}

void Spring::render()
{
	glDisable(GL_LIGHTING);
	glColor3f(0.9f, 0.9f, 0.15f);
	glLineWidth(2.0f);

	glBegin(GL_LINES);
	glVertex3f(m_pNode1->m_position.x, m_pNode1->m_position.y, m_pNode1->m_position.z);
	glVertex3f(m_pNode2->m_position.x, m_pNode2->m_position.y, m_pNode2->m_position.z);
	glEnd();
	glEnable(GL_LIGHTING);
}
