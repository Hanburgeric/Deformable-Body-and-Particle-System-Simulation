#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"

class Particle
{
public:
	Particle(const float& mass, const Vec3& velocity, const Vec3& position, const float& radius, const float& restitution);

	void applyForce(const Vec3& force);

	void update(const float& deltaTime);
	void render();

	Vec3 m_force;
	float m_mass;
	Vec3 m_acceleration;
	Vec3 m_velocity;
	Vec3 m_position;
	float m_radius;
	float m_restitution;
};

#endif
