#ifndef GROUND_H
#define GROUND_H

#include "Vector.h"

class Ground
{
public:
	Ground(const Vec3& position);

	void render();

	Vec3 m_position;
	Vec3 m_normal;
};

#endif
