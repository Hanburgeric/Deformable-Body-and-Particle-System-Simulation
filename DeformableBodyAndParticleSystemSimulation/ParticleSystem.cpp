#include <chrono>
#include "GL/freeglut.h"
#include <stdio.h>

#include "Cloth.h"
#include "Node.h"
#include "ParticleSystem.h"
#include "Sphere.h"

ParticleSystem::ParticleSystem(const int& numParticles, const float& particleRadius, const float& particleRestitution,
	const float* emitterVertices, const float* boundaryVertices,
	const int& hashRadius, const float& bucketLength)
{
	m_collisionMode = 0;

	for (int i = 0; i < sizeof(m_averageTime) / sizeof(double); i++)
	{
		m_averageTime[i] = 0.0f;
		m_numIter[i] = 1.0f;
	}

	m_particleRadius = particleRadius;
	m_particleRestitution = particleRestitution;

	for (int i = 0; i < 6; i++)
	{
		m_emitterVertices[i] = emitterVertices[i];
		m_boundaryVertices[i] = boundaryVertices[i];
	}

	m_hashRadius = hashRadius;
	m_bucketLength = bucketLength;
	m_numBucketsX = ceilf((m_boundaryVertices[1] - m_boundaryVertices[0]) / m_bucketLength);
	m_numBucketsY = ceilf((m_boundaryVertices[3] - m_boundaryVertices[2]) / m_bucketLength);
	m_numBucketsZ = ceilf((m_boundaryVertices[5] - m_boundaryVertices[4]) / m_bucketLength);

	m_pHashGrid = new std::vector<int>[m_numBucketsX * m_numBucketsY * m_numBucketsZ];

	generateParticles(numParticles);
}

ParticleSystem::~ParticleSystem()
{
	delete[] m_pHashGrid;
	m_pHashGrid = nullptr;
}

float ParticleSystem::generateRandomFloat(const float& min, const float& max)
{
	return min + (float)rand() / ((float)RAND_MAX / (max - min));
}

void ParticleSystem::generateParticles(const int& num)
{
	//Subtracts particles if num is negative
	if (num < 0)
	{
		if (-num >= m_particleList.size())
		{
			m_particleList.clear();
		}

		else
		{
			m_particleList.erase(m_particleList.begin(), m_particleList.begin() - num);
		}
	}

	else
	{
		//Adds particles if num is positive
		for (int i = 0; i < num; i++)
		{
			Vec3 randPos(generateRandomFloat(m_emitterVertices[0], m_emitterVertices[1]),
				generateRandomFloat(m_emitterVertices[2], m_emitterVertices[3]),
				generateRandomFloat(m_emitterVertices[4], m_emitterVertices[5]));
			m_particleList.push_back(Particle(1.0f, Vec3(0.0f, 0.0f, 0.0f), randPos, m_particleRadius, m_particleRestitution));
		}
	}
}

void ParticleSystem::applyForce(const int& value)
{
	//Applies gravity
	for (int i = 0; i < m_particleList.size(); i++)
	{
		m_particleList[i].applyForce(Vec3(0.0f, -9.81f / (float)value, 0.0f) * m_particleList[i].m_mass);
	}
}

void ParticleSystem::barycentricRTI(const Vec3& pos, const Vec3& dir, const Node* n1, const Node* n2, const Node* n3, float& u, float& v, float& t)
{
	float term1 = dotProduct(pos - n1->m_position, crossProduct(dir, n3->m_position - n1->m_position));
	float term2 = dotProduct(dir, crossProduct(pos - n1->m_position, n2->m_position - n1->m_position));
	float term3 = dotProduct(n3->m_position - n1->m_position, crossProduct(pos - n1->m_position, n2->m_position - n1->m_position));
	float denominator = dotProduct(n2->m_position - n1->m_position, crossProduct(dir, n3->m_position - n1->m_position));

	u = term1 / denominator;
	v = term2 / denominator;
	t = term3 / denominator;
}

void ParticleSystem::collisionResponse(const float& restitution, const float& deltaTime, Sphere* pSphere, Cloth* pCloth)
{
	if (!m_particleList.empty())
	{
		for (int i = 0; i < m_particleList.size(); i++)
		{
			//Boundary collision
			if (m_particleList[i].m_position.x <= m_boundaryVertices[0] + m_particleList[i].m_radius && m_particleList[i].m_velocity.x < 0)
			{
				m_particleList[i].m_position.x = m_boundaryVertices[0] + m_particleList[i].m_radius;
				m_particleList[i].m_velocity.x *= -m_particleList[i].m_restitution;
			}

			if (m_particleList[i].m_position.x >= m_boundaryVertices[1] - m_particleList[i].m_radius && m_particleList[i].m_velocity.x > 0)
			{
				m_particleList[i].m_position.x = m_boundaryVertices[1] - m_particleList[i].m_radius;
				m_particleList[i].m_velocity.x *= -m_particleList[i].m_restitution;
			}

			if (m_particleList[i].m_position.y <= m_boundaryVertices[2] + m_particleList[i].m_radius && m_particleList[i].m_velocity.y < 0)
			{
				m_particleList[i].m_position.y = m_boundaryVertices[2] + m_particleList[i].m_radius;
				m_particleList[i].m_velocity.y *= -m_particleList[i].m_restitution;
			}

			if (m_particleList[i].m_position.y >= m_boundaryVertices[3] - m_particleList[i].m_radius && m_particleList[i].m_velocity.y > 0)
			{
				m_particleList[i].m_position.y = m_boundaryVertices[3] - m_particleList[i].m_radius;
				m_particleList[i].m_velocity.y *= -m_particleList[i].m_restitution;
			}

			if (m_particleList[i].m_position.z <= m_boundaryVertices[4] + m_particleList[i].m_radius && m_particleList[i].m_velocity.z < 0)
			{
				m_particleList[i].m_position.z = m_boundaryVertices[4] + m_particleList[i].m_radius;
				m_particleList[i].m_velocity.z *= -m_particleList[i].m_restitution;
			}

			if (m_particleList[i].m_position.z >= m_boundaryVertices[5] - m_particleList[i].m_radius && m_particleList[i].m_velocity.z > 0)
			{
				m_particleList[i].m_position.z = m_boundaryVertices[5] - m_particleList[i].m_radius;
				m_particleList[i].m_velocity.z *= -m_particleList[i].m_restitution;
			}

			//Rigid body (sphere) collision
			Vec3 sphereNorm = m_particleList[i].m_position - pSphere->m_position;

			if (sphereNorm.length() <= m_particleList[i].m_radius + pSphere->m_radius)
			{
				float velDot = dotProduct(sphereNorm, m_particleList[i].m_velocity);
				m_particleList[i].m_position = pSphere->m_position + sphereNorm.normalize() * (m_particleList[i].m_radius + pSphere->m_radius);
				m_particleList[i].m_velocity = m_particleList[i].m_velocity - 2.0f * velDot * sphereNorm * deltaTime;
			}

			////Deformable body (cloth) collision
			//for (int j = 0; j < pCloth->m_pFaceList.size(); j += 3)
			//{
			//	float u;
			//	float v;
			//	float t;

			//	barycentricRTI(m_particleList[i].m_position,
			//		m_particleList[i].m_velocity,
			//		pCloth->m_pFaceList[j],
			//		pCloth->m_pFaceList[j + 1],
			//		pCloth->m_pFaceList[j + 2],
			//		u, v, t);

			//	if (u > 0.0f && v > 0.0f && u + v < 1.0f && t > 0.0f && t < deltaTime)
			//	{
			//		Vec3 weightedPosition = (1.0f - u - v) * pCloth->m_pFaceList[j]->m_position +
			//			v * pCloth->m_pFaceList[j + 1]->m_position +
			//			u * pCloth->m_pFaceList[j + 2]->m_position;

			//		Vec3 weightedNormal = (1.0f - u - v) * pCloth->m_pFaceList[j]->m_normal +
			//			v * pCloth->m_pFaceList[j + 1]->m_normal +
			//			u * pCloth->m_pFaceList[j + 2]->m_normal;

			//		m_particleList[i].m_position = weightedPosition + (weightedNormal * m_particleList[i].m_radius);

			//		m_particleList[i].m_velocity = m_particleList[i].m_restitution *
			//			(m_particleList[i].m_velocity - 2.0f * dotProduct(m_particleList[i].m_velocity, weightedNormal) * weightedNormal);
			//	}
			//}
		}

		////Deformable body (cloth) collision with hash grid
		////Clears the hash grid
		//for (int x = 0; x < m_numBucketsX; x++)
		//{
		//	for (int y = 0; y < m_numBucketsY; y++)
		//	{
		//		for (int z = 0; z < m_numBucketsZ; z++)
		//		{
		//			m_pHashGrid[x + m_numBucketsX * (y + m_numBucketsY * z)].clear();
		//		}
		//	}
		//}

		////Creates the hash grid
		//for (int i = 0; i < pCloth->m_pFaceList.size(); i += 6)
		//{
		//	int x = (int)((pCloth->m_pFaceList[i]->m_position.x + m_bucketLength) / 2.0f);
		//	int y = (int)((pCloth->m_pFaceList[i]->m_position.y + m_bucketLength) / 2.0f);
		//	int z = (int)((pCloth->m_pFaceList[i]->m_position.z + m_bucketLength) / 2.0f);

		//	if (x < 0)
		//	{
		//		x = 0;
		//	}
		//	else if (x > m_numBucketsX - 1)
		//	{
		//		x = m_numBucketsX - 1;
		//	}
		//	if (y < 0)
		//	{
		//		y = 0;
		//	}
		//	else if (y > m_numBucketsY - 1)
		//	{
		//		y = m_numBucketsY - 1;
		//	}
		//	if (z < 0)
		//	{
		//		z = 0;
		//	}
		//	else if (z > m_numBucketsZ - 1)
		//	{
		//		z = m_numBucketsZ - 1;
		//	}

		//	m_pHashGrid[x + m_numBucketsX * (y + m_numBucketsY * z)].push_back(i);
		//}

		////Pushes back neighboring nodes
		//for (int i = 0; i < m_particleList.size(); i++)
		//{
		//	int x = (int)((m_particleList[i].m_position.x + m_bucketLength) / 2.0f);
		//	int y = (int)((m_particleList[i].m_position.y + m_bucketLength) / 2.0f);
		//	int z = (int)((m_particleList[i].m_position.z + m_bucketLength) / 2.0f);

		//	std::vector<int>neighborList;

		//	for (int x0 = x - m_hashRadius; x0 <= x + m_hashRadius; x0++)
		//	{
		//		for (int y0 = y - m_hashRadius; y0 <= y + m_hashRadius; y0++)
		//		{
		//			for (int z0 = z - m_hashRadius; z0 <= z + m_hashRadius; z0++)
		//			{
		//				if (x0 >= 0 && x0 <= m_numBucketsX - 1 && y0 >= 0 && y0 <= m_numBucketsY - 1 && z0 >= 0 && z0 <= m_numBucketsZ - 1)
		//				{
		//					for (int j = 0; j < m_pHashGrid[x0 + m_numBucketsX * (y0 + m_numBucketsY * z0)].size(); j++)
		//					{
		//						neighborList.push_back(m_pHashGrid[x0 + m_numBucketsX * (y0 + m_numBucketsY * z0)][j]);
		//					}
		//				}
		//			}
		//		}
		//	}

		//	//Checks for collision with neighboring nodes
		//	for (int k = 0; k < neighborList.size(); k++)
		//	{
		//		int j = neighborList[k];

		//		float u;
		//		float v;
		//		float t;

		//		barycentricRTI(m_particleList[i].m_position,
		//			m_particleList[i].m_velocity,
		//			pCloth->m_pFaceList[j],
		//			pCloth->m_pFaceList[j + 1],
		//			pCloth->m_pFaceList[j + 2],
		//			u, v, t);

		//		if (u > 0.0f && v > 0.0f && u + v < 1.0f && t > 0.0f && t < deltaTime)
		//		{
		//			Vec3 weightedPosition = (1.0f - u - v) * pCloth->m_pFaceList[j]->m_position +
		//				v * pCloth->m_pFaceList[j + 1]->m_position +
		//				u * pCloth->m_pFaceList[j + 2]->m_position;

		//			Vec3 weightedNormal = (1.0f - u - v) * pCloth->m_pFaceList[j]->m_normal +
		//				v * pCloth->m_pFaceList[j + 1]->m_normal +
		//				u * pCloth->m_pFaceList[j + 2]->m_normal;

		//			m_particleList[i].m_position = weightedPosition + (weightedNormal * m_particleList[i].m_radius);

		//			m_particleList[i].m_velocity = m_particleList[i].m_restitution *
		//				(m_particleList[i].m_velocity - 2.0f * dotProduct(m_particleList[i].m_velocity, weightedNormal) * weightedNormal);
		//		}
		//	}
		//}

		//Measure the amount of time taken to calculate particle-particle collisions
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

		//Particle collision
		switch (m_collisionMode)
		{
		case 0:	//Mode 1: Checks every particle with every particle
		{
			for (int i = 0; i < m_particleList.size(); i++)
			{
				for (int j = 0; j < m_particleList.size(); j++)
				{
					if (i != j)
					{
						Vec3 ij = m_particleList[j].m_position - m_particleList[i].m_position;
						Vec3 ji = m_particleList[i].m_position - m_particleList[j].m_position;
						float distance = ij.length();

						if (distance <= m_particleList[i].m_radius + m_particleList[j].m_radius)
						{
							Vec3 norm1 = ij / distance;
							Vec3 norm2 = ji / distance;

							//Collision normal
							Vec3 vn1 = norm1 * (dotProduct(norm1, m_particleList[i].m_velocity));
							Vec3 vn2 = norm2 * (dotProduct(norm2, m_particleList[j].m_velocity));

							//Collision tangent
							Vec3 vt1 = m_particleList[i].m_velocity - vn1;
							Vec3 vt2 = m_particleList[j].m_velocity - vn2;

							//Relative velocity
							Vec3 rVel = m_particleList[j].m_velocity - m_particleList[i].m_velocity;

							//Prevents overlapping collisions
							if (dotProduct(rVel, ij) < 0.0f)
							{
								m_particleList[i].m_velocity = vt1 + (vn2 * 2.0f * m_particleList[j].m_mass + vn1 *
									(m_particleList[i].m_mass - m_particleList[j].m_mass)) /
									(m_particleList[i].m_mass + m_particleList[j].m_mass);
								m_particleList[j].m_velocity = vt2 + (vn1 * 2.0f * m_particleList[i].m_mass + vn2 *
									(m_particleList[j].m_mass - m_particleList[i].m_mass)) /
									(m_particleList[i].m_mass + m_particleList[j].m_mass);
							}
						}
					}
				}
			}
			break;
		}
		case 1:	//Mode 1: Checks every particle with every particle
		{
			for (int i = 0; i < m_particleList.size() - 1; i++)
			{
				for (int j = i + 1; j < m_particleList.size(); j++)
				{
					Vec3 ij = m_particleList[j].m_position - m_particleList[i].m_position;
					Vec3 ji = m_particleList[i].m_position - m_particleList[j].m_position;
					float distance = ij.length();

					if (distance <= m_particleList[i].m_radius + m_particleList[j].m_radius)
					{
						Vec3 norm1 = ij / distance;
						Vec3 norm2 = ji / distance;

						//Collision normal
						Vec3 vn1 = norm1 * (dotProduct(norm1, m_particleList[i].m_velocity));
						Vec3 vn2 = norm2 * (dotProduct(norm2, m_particleList[j].m_velocity));

						//Collision tangent
						Vec3 vt1 = m_particleList[i].m_velocity - vn1;
						Vec3 vt2 = m_particleList[j].m_velocity - vn2;

						//Relative velocity
						Vec3 rVel = m_particleList[j].m_velocity - m_particleList[i].m_velocity;

						//Prevents overlapping collisions
						if (dotProduct(rVel, ij) < 0.0f)
						{
							m_particleList[i].m_velocity = vt1 + (vn2 * 2.0f * m_particleList[j].m_mass + vn1 *
								(m_particleList[i].m_mass - m_particleList[j].m_mass)) /
								(m_particleList[i].m_mass + m_particleList[j].m_mass);
							m_particleList[j].m_velocity = vt2 + (vn1 * 2.0f * m_particleList[i].m_mass + vn2 *
								(m_particleList[j].m_mass - m_particleList[i].m_mass)) /
								(m_particleList[i].m_mass + m_particleList[j].m_mass);
						}
					}
				}
			}
			break;
		}
		case 2:	//Mode 3: Hash grid implementation of mode 1
		{
			//Clears the hash grid
			for (int x = 0; x < m_numBucketsX; x++)
			{
				for (int y = 0; y < m_numBucketsY; y++)
				{
					for (int z = 0; z < m_numBucketsZ; z++)
					{
						m_pHashGrid[x + m_numBucketsX * (y + m_numBucketsY * z)].clear();
					}
				}
			}

			//Creates the hash grid
			for (int i = 0; i < m_particleList.size(); i++)
			{
				int x = (int)((m_particleList[i].m_position.x - m_boundaryVertices[0]) / m_bucketLength);
				int y = (int)((m_particleList[i].m_position.y - m_boundaryVertices[2]) / m_bucketLength);
				int z = (int)((m_particleList[i].m_position.z - m_boundaryVertices[4]) / m_bucketLength);

				if (x < 0)
				{
					x = 0;
				}
				else if (x > m_numBucketsX - 1)
				{
					x = m_numBucketsX - 1;
				}
				if (y < 0)
				{
					y = 0;
				}
				else if (y > m_numBucketsY - 1)
				{
					y = m_numBucketsY - 1;
				}
				if (z < 0)
				{
					z = 0;
				}
				else if (z > m_numBucketsZ - 1)
				{
					z = m_numBucketsZ - 1;
				}

				m_pHashGrid[x + m_numBucketsX * (y + m_numBucketsY * z)].push_back(i);
			}

			//Pushes back neighboring particles
			for (int i = 0; i < m_particleList.size(); i++)
			{
				int x = (int)((m_particleList[i].m_position.x - m_boundaryVertices[0]) / m_bucketLength);
				int y = (int)((m_particleList[i].m_position.y - m_boundaryVertices[2]) / m_bucketLength);
				int z = (int)((m_particleList[i].m_position.z - m_boundaryVertices[4]) / m_bucketLength);

				std::vector<int>neighborList;

				for (int x0 = x - m_hashRadius; x0 <= x + m_hashRadius; x0++)
				{
					for (int y0 = y - m_hashRadius; y0 <= y + m_hashRadius; y0++)
					{
						for (int z0 = z - m_hashRadius; z0 <= z + m_hashRadius; z0++)
						{
							if (x0 >= 0 && x0 <= m_numBucketsX - 1 && y0 >= 0 && y0 <= m_numBucketsY - 1 && z0 >= 0 && z0 <= m_numBucketsZ - 1)
							{
								for (int j = 0; j < m_pHashGrid[x0 + m_numBucketsX * (y0 + m_numBucketsY * z0)].size(); j++)
								{
									neighborList.push_back(m_pHashGrid[x0 + m_numBucketsX * (y0 + m_numBucketsY * z0)][j]);
								}
							}
						}
					}
				}

				//Checks for collision with neighboring particles
				for (int k = 0; k < neighborList.size(); k++)
				{
					int j = neighborList[k];
					if (i != j)
					{
						Vec3 ij = m_particleList[j].m_position - m_particleList[i].m_position;
						Vec3 ji = m_particleList[i].m_position - m_particleList[j].m_position;
						float distance = ij.length();

						if (distance <= m_particleList[i].m_radius + m_particleList[j].m_radius)
						{
							Vec3 norm1 = ij / distance;
							Vec3 norm2 = ji / distance;

							//Collision normal
							Vec3 vn1 = norm1 * (dotProduct(norm1, m_particleList[i].m_velocity));
							Vec3 vn2 = norm2 * (dotProduct(norm2, m_particleList[j].m_velocity));

							//Collision tangent
							Vec3 vt1 = m_particleList[i].m_velocity - vn1;
							Vec3 vt2 = m_particleList[j].m_velocity - vn2;

							//Relative velocity
							Vec3 rVel = m_particleList[j].m_velocity - m_particleList[i].m_velocity;

							//Prevents overlapping collisions
							if (dotProduct(rVel, ij) < 0.0f)
							{
								m_particleList[i].m_velocity = vt1 + (vn2 * 2.0f * m_particleList[j].m_mass + vn1 *
									(m_particleList[i].m_mass - m_particleList[j].m_mass)) /
									(m_particleList[i].m_mass + m_particleList[j].m_mass);
								m_particleList[j].m_velocity = vt2 + (vn1 * 2.0f * m_particleList[i].m_mass + vn2 *
									(m_particleList[j].m_mass - m_particleList[i].m_mass)) /
									(m_particleList[i].m_mass + m_particleList[j].m_mass);
							}
						}
					}
				}
			}
			break;
		}
		default:
		{
			break;
		}
		}

		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		double dt = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		m_averageTime[m_collisionMode] = m_averageTime[m_collisionMode] *
			(m_numIter[m_collisionMode] - 1.0f) / m_numIter[m_collisionMode] +
			dt / m_numIter[m_collisionMode];
		m_numIter[m_collisionMode] += 1.0f;
		printf("Average Time (Collision Mode %d): %0.2f ms\n", m_collisionMode + 1, m_averageTime[m_collisionMode]);
	}
}

void ParticleSystem::update(const float& deltaTime)
{
	for (int i = 0; i < m_particleList.size(); i++)
	{
		m_particleList[i].update(deltaTime);
	}
}

void ParticleSystem::render()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(2.5f);

	//Renders the emitter box
	glBegin(GL_LINE_STRIP);
	glVertex3f(m_emitterVertices[0], m_emitterVertices[2], m_emitterVertices[4]);	//1
	glVertex3f(m_emitterVertices[0], m_emitterVertices[3], m_emitterVertices[4]);	//2
	glVertex3f(m_emitterVertices[1], m_emitterVertices[3], m_emitterVertices[4]);	//3
	glVertex3f(m_emitterVertices[1], m_emitterVertices[2], m_emitterVertices[4]);	//4
	glVertex3f(m_emitterVertices[0], m_emitterVertices[2], m_emitterVertices[4]);	//5
	glVertex3f(m_emitterVertices[0], m_emitterVertices[2], m_emitterVertices[5]);	//6
	glVertex3f(m_emitterVertices[0], m_emitterVertices[3], m_emitterVertices[5]);	//7
	glVertex3f(m_emitterVertices[1], m_emitterVertices[3], m_emitterVertices[5]);	//8
	glVertex3f(m_emitterVertices[1], m_emitterVertices[2], m_emitterVertices[5]);	//9
	glVertex3f(m_emitterVertices[0], m_emitterVertices[2], m_emitterVertices[5]);	//10
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(m_emitterVertices[0], m_emitterVertices[3], m_emitterVertices[4]);	//2
	glVertex3f(m_emitterVertices[0], m_emitterVertices[3], m_emitterVertices[5]);	//7
	glVertex3f(m_emitterVertices[1], m_emitterVertices[3], m_emitterVertices[4]);	//3
	glVertex3f(m_emitterVertices[1], m_emitterVertices[3], m_emitterVertices[5]);	//8
	glVertex3f(m_emitterVertices[1], m_emitterVertices[2], m_emitterVertices[4]);	//4
	glVertex3f(m_emitterVertices[1], m_emitterVertices[2], m_emitterVertices[5]);	//9
	glEnd();

	//Renders the boundary box
	glBegin(GL_LINE_STRIP);
	glVertex3f(m_boundaryVertices[0], m_boundaryVertices[2], m_boundaryVertices[4]);	//1
	glVertex3f(m_boundaryVertices[0], m_boundaryVertices[3], m_boundaryVertices[4]);	//2
	glVertex3f(m_boundaryVertices[1], m_boundaryVertices[3], m_boundaryVertices[4]);	//3
	glVertex3f(m_boundaryVertices[1], m_boundaryVertices[2], m_boundaryVertices[4]);	//4
	glVertex3f(m_boundaryVertices[0], m_boundaryVertices[2], m_boundaryVertices[4]);	//5
	glVertex3f(m_boundaryVertices[0], m_boundaryVertices[2], m_boundaryVertices[5]);	//6
	glVertex3f(m_boundaryVertices[0], m_boundaryVertices[3], m_boundaryVertices[5]);	//7
	glVertex3f(m_boundaryVertices[1], m_boundaryVertices[3], m_boundaryVertices[5]);	//8
	glVertex3f(m_boundaryVertices[1], m_boundaryVertices[2], m_boundaryVertices[5]);	//9
	glVertex3f(m_boundaryVertices[0], m_boundaryVertices[2], m_boundaryVertices[5]);	//10
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(m_boundaryVertices[0], m_boundaryVertices[3], m_boundaryVertices[4]);	//2
	glVertex3f(m_boundaryVertices[0], m_boundaryVertices[3], m_boundaryVertices[5]);	//7
	glVertex3f(m_boundaryVertices[1], m_boundaryVertices[3], m_boundaryVertices[4]);	//3
	glVertex3f(m_boundaryVertices[1], m_boundaryVertices[3], m_boundaryVertices[5]);	//8
	glVertex3f(m_boundaryVertices[1], m_boundaryVertices[2], m_boundaryVertices[4]);	//4
	glVertex3f(m_boundaryVertices[1], m_boundaryVertices[2], m_boundaryVertices[5]);	//9
	glEnd();

	for (int i = 0; i < m_particleList.size(); i++)
	{
		m_particleList[i].render();
	}
}
