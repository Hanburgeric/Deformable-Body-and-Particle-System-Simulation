#include "GL/freeglut.h"

#include "Cloth.h"
#include "Ground.h"
#include "Node.h"
#include "Sphere.h"
#include "Spring.h"

Cloth::Cloth(const int& numX, const int& numY, const int& numZ,
	const float& deltaX, const float& deltaY, const float& deltaZ,
	const float& structural, const float& shear, const float& bending,
	const float& damping)
{
	m_renderMode = 0;
	m_resolveFaces = true;

	m_numNodesX = numX;
	m_numNodesY = numY;
	m_numNodesZ = numZ;

	float minX = -(float)m_numNodesX / 2.0f;
	float maxX = (float)m_numNodesX / 2.0f;
	float minY = -(float)m_numNodesY / 2.0f;
	float maxY = (float)m_numNodesY / 2.0f;
	float minZ = -(float)m_numNodesZ / 2.0f;
	float maxZ = (float)m_numNodesZ / 2.0f;

	for (float x = minX; x < maxX; x += 1.0f)
	{
		for (float z = minZ; z < maxZ; z += 1.0f)
		{
			//Initialization of nodes
			Node* pNewNode = new Node(Vec3(x, 25.0f, z));

			//Sets all nodes in one edge to be fixed
			if (z == minZ)
			{
				pNewNode->isFixed = true;
			}

			m_pNodeList.push_back(pNewNode);

			//Initialization of springs
			int nodeListSize = m_pNodeList.size();
			if (x > minX)
			{
				Spring* pNewSpring = new Spring(m_pNodeList[nodeListSize - 1], m_pNodeList[nodeListSize - m_numNodesZ - 1], structural, damping);
				m_pSpringList.push_back(pNewSpring);
			}

			if (z > minZ)
			{
				Spring* pNewSpring = new Spring(m_pNodeList[nodeListSize - 1], m_pNodeList[nodeListSize - 2], structural, damping);
				m_pSpringList.push_back(pNewSpring);
			}

			if (x > minX && z > minZ)
			{
				Spring* pNewSpring = new Spring(m_pNodeList[nodeListSize - 1], m_pNodeList[nodeListSize - m_numNodesZ - 2], shear, damping);
				m_pSpringList.push_back(pNewSpring);

				//Initialization of faces
				m_pFaceList.push_back(m_pNodeList[nodeListSize - m_numNodesZ - 2]);
				m_pFaceList.push_back(m_pNodeList[nodeListSize - 1]);
				m_pFaceList.push_back(m_pNodeList[nodeListSize - m_numNodesZ - 1]);

				m_pFaceList.push_back(m_pNodeList[nodeListSize - m_numNodesZ - 2]);
				m_pFaceList.push_back(m_pNodeList[nodeListSize - 2]);
				m_pFaceList.push_back(m_pNodeList[nodeListSize - 1]);
			}

			if (x > minX && z < maxZ - 1.0f)
			{
				Spring* pNewSpring = new Spring(m_pNodeList[nodeListSize - 1], m_pNodeList[nodeListSize - m_numNodesZ], shear, damping);
				m_pSpringList.push_back(pNewSpring);
			}

			if (x > minX + 1.0f)
			{
				Spring* pNewSpring = new Spring(m_pNodeList[nodeListSize - 1], m_pNodeList[nodeListSize - 2 * m_numNodesZ - 1], bending, damping);
				m_pSpringList.push_back(pNewSpring);
			}

			if (z > minZ + 1.0f)
			{
				Spring* pNewSpring = new Spring(m_pNodeList[nodeListSize - 1], m_pNodeList[nodeListSize - 3], bending, damping);
				m_pSpringList.push_back(pNewSpring);
			}
		}
	}

	//Initialization of texture coordinates
	for (int i = 0; i < m_pFaceList.size(); i += 3)
	{
		float delta = 0.02f;
		float x = (float)((i % 294) / 6) * delta;
		float y = 1.0f - (float)(i / 294) * delta;

		if (i % 6)
		{
			m_textureX.push_back(x);
			m_textureY.push_back(y);
			m_textureX.push_back(x);
			m_textureY.push_back(y - delta);
			m_textureX.push_back(x + delta);
			m_textureY.push_back(y - delta);
		}

		else
		{
			m_textureX.push_back(x);
			m_textureY.push_back(y);
			m_textureX.push_back(x + delta);
			m_textureY.push_back(y - delta);
			m_textureX.push_back(x + delta);
			m_textureY.push_back(y);
		}
	}

	computeNormals();
}

Cloth::~Cloth()
{
	for (int i = 0; i < m_pSpringList.size(); i++)
	{
		delete m_pSpringList[i];
	}

	for (int i = 0; i < m_pNodeList.size(); i++)
	{
		delete m_pNodeList[i];
	}
}

void Cloth::computeNormals()
{
	//Calculates and stores vertex normals
	for (int i = 0; i < m_pFaceList.size(); i += 3)
	{
		Vec3 normal = crossProduct(m_pFaceList[i + 2]->m_position - m_pFaceList[i]->m_position, m_pFaceList[i + 1]->m_position - m_pFaceList[i]->m_position);
		m_pFaceList[i]->m_normal += normal;
		m_pFaceList[i + 1]->m_normal += normal;
		m_pFaceList[i + 2]->m_normal += normal;
	}

	for (int i = 0; i < m_pFaceList.size(); i++)
	{
		m_pFaceList[i]->m_normal = m_pFaceList[i]->m_normal.normalize();
	}
}

void Cloth::addForce(const Vec3& force)
{
	for (int i = 0; i < m_pNodeList.size(); i++)
	{
		m_pNodeList[i]->m_force += force;
	}
}

void Cloth::applyForce(const int& value)
{
	//Applies gravity
	for (int i = 0; i < m_pNodeList.size(); i++)
	{
		m_pNodeList[i]->applyForce(Vec3(0.0f, -9.81f / (float)value, 0.0f) * m_pNodeList[i]->m_mass);
	}

	for (int i = 0; i < m_pSpringList.size(); i++)
	{
		m_pSpringList[i]->applyForce();
	}
}

void Cloth::collisionResponse(const float& deltaTime, const Ground* pGround, const Sphere* pSphere)
{
	//Collision with the ground
	for (int i = 0; i < m_pNodeList.size(); i++)
	{
		float posDot = dotProduct(m_pNodeList[i]->m_position - pGround->m_position, pGround->m_normal);
		float velDot = dotProduct(pGround->m_normal, m_pNodeList[i]->m_velocity);

		if (posDot < 0 && velDot < 0)
		{
			m_pNodeList[i]->m_position += Vec3(0.0f, 0.1f, 0.0f);
			m_pNodeList[i]->m_velocity.y = (m_pNodeList[i]->m_velocity - 2.0f * velDot * pGround->m_normal).y * deltaTime;
		}
	}

	//Collision with the static body (a sphere)
	for (int i = 0; i < m_pNodeList.size(); i++)
	{
		Vec3 sphereNorm = m_pNodeList[i]->m_position - pSphere->m_position;

		if (sphereNorm.length() <= pSphere->m_radius)
		{
			float velDot = dotProduct(sphereNorm, m_pNodeList[i]->m_velocity);
			m_pNodeList[i]->m_position = pSphere->m_position + sphereNorm.normalize() * pSphere->m_radius;
			m_pNodeList[i]->m_velocity = m_pNodeList[i]->m_velocity - 2.0f * velDot * sphereNorm * deltaTime;
		}
	}

	//Resolves (or attempts to resolve) the issue of the sphere sticking out through the faces
	if (m_resolveFaces)
	{
		for (int i = 0; i < m_pFaceList.size(); i += 3)
		{
			Vec3 midPoint = (m_pFaceList[i]->m_position + m_pFaceList[i + 1]->m_position + m_pFaceList[i + 2]->m_position) / 3.0f;
			Vec3 collisionVec = midPoint - pSphere->m_position;
			float height = pSphere->m_radius - collisionVec.length();

			for (int j = 0; j < 3; j++)
			{
				if (height > 0.0f)
				{
					float relDir = dotProduct(m_pFaceList[i + j]->m_normal, collisionVec);

					if (relDir > 0.0f)
					{
						m_pFaceList[i + j]->m_position = m_pFaceList[i + j]->m_position + m_pFaceList[i + j]->m_normal * height;
					}

					else if (relDir < 0.0f)
					{
						m_pFaceList[i + j]->m_position = m_pFaceList[i + j]->m_position - m_pFaceList[i + j]->m_normal * height;
					}
				}
			}
		}
	}
}

void Cloth::update(const float& deltaTime)
{
	for (int i = 0; i < m_pNodeList.size(); i++)
	{
		m_pNodeList[i]->update(deltaTime);
	}
}

void Cloth::render()
{
	switch (m_renderMode)
	{
	case 0:	//Mode 1: Draws nodes
	{
		for (int i = 0; i < m_pNodeList.size(); i++)
		{
			m_pNodeList[i]->render();
		}
		break;
	}
	case 1:	//Mode 2: Draws springs
	{
		for (int i = 0; i < m_pSpringList.size(); i++)
		{
			m_pSpringList[i]->render();
		}
		break;
	}
	case 2:	//Mode 3: Draws a texture
	{
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 1.0f, 1.0f);
		for (int i = 0; i < m_pFaceList.size(); i += 3)
		{
			glTexCoord2f(m_textureX[i], m_textureY[i]);
			glVertex3f(m_pFaceList[i]->m_position.x, m_pFaceList[i]->m_position.y, m_pFaceList[i]->m_position.z);
			glTexCoord2f(m_textureX[i + 1], m_textureY[i + 1]);
			glVertex3f(m_pFaceList[i + 1]->m_position.x, m_pFaceList[i + 1]->m_position.y, m_pFaceList[i + 1]->m_position.z);
			glTexCoord2f(m_textureX[i + 2], m_textureY[i + 2]);
			glVertex3f(m_pFaceList[i + 2]->m_position.x, m_pFaceList[i + 2]->m_position.y, m_pFaceList[i + 2]->m_position.z);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
		break;
	}
	default:
	{
		break;
	}
	}
}
