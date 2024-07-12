#ifndef CLOTH_H
#define CLOTH_H

#include <vector>

#include "Vector.h"

class Ground;
class Node;
class Sphere;
class Spring;

class Cloth
{
public:
	Cloth(const int& numX, const int& numY, const int& numZ,
		const float& deltaX, const float& deltaY, const float& deltaZ,
		const float& structural, const float& shear, const float& bending,
		const float& damping);
	~Cloth();

	void computeNormals();

	void addForce(const Vec3& force);
	void applyForce(const int& value);

	void collisionResponse(const float& deltaTime, const Ground* pGround, const Sphere* pSphere);

	void update(const float& deltaTime);
	void render();

	int m_renderMode;
	bool m_resolveFaces;

	int m_numNodesX;
	int m_numNodesY;
	int m_numNodesZ;

	std::vector<Node*>m_pNodeList;
	std::vector<Spring*>m_pSpringList;
	std::vector<Node*>m_pFaceList;
	std::vector<float>m_textureX;
	std::vector<float>m_textureY;
};

#endif
