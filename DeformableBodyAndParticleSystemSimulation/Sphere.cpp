#include "GL/freeglut.h"

#include "Sphere.h"

Sphere::Sphere(const Vec3& position, const float& radius)
{
	m_force = Vec3();
	m_mass = 1.0f;
	m_acceleration = Vec3();
	m_velocity = Vec3();
	m_position = position;
	m_radius = radius;
}

void Sphere::applyForce(const Vec3& force)
{
	m_force += force;
}

void Sphere::update(const float& deltaTime)
{
	m_acceleration = m_force / m_mass;
	m_velocity += m_acceleration * deltaTime;
	m_position += m_velocity * deltaTime;

	m_force.setZero();
}

void Sphere::render()
{
	glPushMatrix();
	glColor3f(0.8f, 0.0f, 0.0f);
	glTranslatef(m_position.x, m_position.y, m_position.z);
	glutSolidSphere(m_radius, 100.0f, 100.0f);
	glPopMatrix();
}
