#ifndef NODE_H
#define NODE_H

#include "Vector.h"

class Node
{
public:
	Node(const Vec3& position);

	void applyForce(const Vec3& force);

	void update(const float& deltaTime);
	void render();

	bool isFixed;
	Vec3 m_force;
	float m_mass;
	Vec3 m_acceleration;
	Vec3 m_velocity;
	Vec3 m_position;
	Vec3 m_normal;
};

#endif
