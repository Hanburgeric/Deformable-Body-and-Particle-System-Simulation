#include "GL/freeglut.h"

#include "Particle.h"

Particle::Particle(const float& mass, const Vec3& velocity, const Vec3& position, const float& radius, const float& restitution)
{
	m_force = Vec3();
	m_mass = mass;
	m_acceleration = Vec3();
	m_velocity = velocity;
	m_position = position;
	m_radius = radius;
	m_restitution = restitution;
}

void Particle::applyForce(const Vec3& force)
{
	m_force += force;
}

void Particle::update(const float& deltaTime)
{
	m_acceleration = m_force / m_mass;
	m_velocity += m_acceleration * deltaTime;
	m_position += m_velocity * deltaTime;

	m_force.setZero();
}

void Particle::render()
{
	glEnable(GL_POINT_SMOOTH);
	glColor3f(0.863f, 0.847f, 0.753f);
	glPointSize(m_radius * 10.0f);
	glBegin(GL_POINTS);
	glVertex3f(m_position.x, m_position.y, m_position.z);
	glEnd();
	glDisable(GL_POINT_SMOOTH);
}
