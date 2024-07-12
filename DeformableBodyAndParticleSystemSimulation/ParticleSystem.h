#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>

#include "Particle.h"

class Cloth;
class Sphere;

class ParticleSystem
{
public:
	ParticleSystem(const int& numParticles, const float& particleRadius, const float& particleRestitution,
		const float* emitterVertices, const float* boundaryVertices,
		const int& hashRadius, const float& bucketLength);
	~ParticleSystem();

	float generateRandomFloat(const float& min, const float& max);
	void generateParticles(const int& num);

	void applyForce(const int& value);
	void barycentricRTI(const Vec3& pos, const Vec3& dir, const Node* n1, const Node* n2, const Node* n3, float& u, float& v, float& t);
	void collisionResponse(const float& restitution, const float& deltaTime, Sphere* pSphere, Cloth* pCloth);

	void update(const float& deltaTime);
	void render();

	int m_collisionMode;

	double m_averageTime[3];
	double m_numIter[3];

	float m_particleRadius;
	float m_particleRestitution;

	float m_emitterVertices[6];
	float m_boundaryVertices[6];

	int m_hashRadius;
	float m_bucketLength;
	int m_numBucketsX;
	int m_numBucketsY;
	int m_numBucketsZ;
	std::vector<int>* m_pHashGrid;

	std::vector<Particle>m_particleList;
};

#endif
