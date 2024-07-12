#ifndef SPHERE_H
#define SPHERE_H

#include "Vector.h"

class Sphere
{
public:
	Sphere(const Vec3& position, const float& radius);

	void applyForce(const Vec3& force);

	void update(const float& deltaTime);
	void render();

	Vec3 m_force;
	float m_mass;
	Vec3 m_acceleration;
	Vec3 m_velocity;
	Vec3 m_position;
	float m_radius;
};

#endif
