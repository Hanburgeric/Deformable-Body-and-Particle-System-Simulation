#define STB_IMAGE_IMPLEMENTATION

#include "GL/freeglut.h"
#include "GL/stb_image.h"

#include "Cloth.h"
#include "Ground.h"
#include "ParticleSystem.h"
#include "Simulator.h"
#include "Sphere.h"

Simulator::Simulator()
{
	//Initialization of camera settings (so that they do not reset when the simulation is restarted)
	m_translate[0] = 0.0f;
	m_translate[1] = -50.0f;

	m_rotate[0] = 25.0f;
	m_rotate[1] = -25.0f;

	m_zoom = 150.0f;

	//Loads the image for the cloth
	glGenTextures(1, &m_texture);

	const char* pFileName = "image.jpg";
	FILE* pFile = nullptr;
	fopen_s(&pFile, pFileName, "rb");

	if (pFile)
	{
		int x;
		int y;
		int channel;

		unsigned char* pImage = stbi_load_from_file(pFile, &x, &y, &channel, 4);
		fclose(pFile);

		//Binds the texture
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	}
}

Simulator::~Simulator()
{
	delete m_pParticleSystem;
	m_pParticleSystem = nullptr;
	delete m_pCloth;
	m_pCloth = nullptr;
	delete m_pSphere;
	m_pSphere = nullptr;
	delete m_pGround;
	m_pGround = nullptr;
	glDeleteTextures(1, &m_texture);
}

void Simulator::init()
{
	//Initialization of member variables
	m_start[0] = false;
	m_start[1] = false;

	m_mousePos[0] = 0;
	m_mousePos[1] = 0;

	m_mouseEvent[0] = false;
	m_mouseEvent[1] = false;

	m_lightPos[0] = 150.0f;
	m_lightPos[1] = 150.0f;
	m_lightPos[2] = 0.0f;
	m_lightPos[3] = 1.0f;

	float emitterVertices[6] = { -10.0f, 10.0f, 80.0f, 100.0f, -10.0f, 10.0f };
	float boundaryVertices[6] = { -10.0f, 10.0f, 0.0f, 50.0f, -10.0f, 10.0f };

	m_pGround = new Ground(Vec3(0.0f, 0.0f, 0.0f));
	m_pSphere = new Sphere(Vec3(0.0f, 10.0f, -10.0f), 10.0f);
	m_pCloth = new Cloth(50, 1, 50, 1, 1, 1, 1500.0f, 50.0f, 1000.0f, 0.5f);
	m_pParticleSystem = new ParticleSystem(1000, 0.75f, 0.5f, emitterVertices, boundaryVertices, 1, 2.5f);

	//Light initialization and settings
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	float noMat[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float matSpec[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float frontColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float matShininess = 20.0f;

	float lightDir[4] = { -1.0f, -1.0f, 0.0f, 0.0f };
	float lightAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float lightDiffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_AMBIENT, frontColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, frontColor);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 80.0f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 80.0f);
	glLightfv(GL_LIGHT0, GL_POSITION, m_lightPos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDir);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}

void Simulator::cleanup()
{
	delete m_pParticleSystem;
	m_pParticleSystem = nullptr;
	delete m_pCloth;
	m_pCloth = nullptr;
	delete m_pSphere;
	m_pSphere = nullptr;
	delete m_pGround;
	m_pGround = nullptr;
}

void Simulator::motion(const int& x, const int& y)
{
	//Calculates and stores the difference between the current and the previous mouse position
	int deltaX = x - m_mousePos[0];
	int deltaY = y - m_mousePos[1];

	//Updates the mouse position
	m_mousePos[0] = x;
	m_mousePos[1] = y;

	if (m_mouseEvent[0])
	{
		m_translate[0] += 0.1f * (float)deltaX;
		m_translate[1] -= 0.1f * (float)deltaY;
	}

	//Rotates the camera
	else if (m_mouseEvent[1])
	{
		m_rotate[0] += 0.1f * (float)deltaY;
		m_rotate[1] += 0.1f * (float)deltaX;
	}

	glutPostRedisplay();
}

void Simulator::mouse(const int& mouseEvent, const int& mouseState, const int& x, const int& y)
{
	//Updates the mouse position
	m_mousePos[0] = x;
	m_mousePos[1] = y;

	switch (mouseEvent)
	{
	case GLUT_LEFT_BUTTON:
	{
		if (GLUT_DOWN == mouseState)
		{
			m_mouseEvent[0] = true;
		}
		else
		{
			m_mouseEvent[0] = false;
		}
		break;
	}
	case GLUT_RIGHT_BUTTON:
	{
		if (GLUT_DOWN == mouseState)
		{
			m_mouseEvent[1] = true;
		}
		else
		{
			m_mouseEvent[1] = false;
		}
		break;
	}
	case 3:
	{
		m_zoom -= 2.5f;
		break;
	}
	case 4:
	{
		m_zoom += 2.5f;
		break;
	}
	default:
	{
		break;
	}
	}

	glutPostRedisplay();
}

void Simulator::keyboard(const unsigned char& key, const int& x, const int& y)
{
	switch (key)
	{
	case ' ':
	{
		m_start[0] = !m_start[0];
		m_start[1] = !m_start[1];
		break;
	}
	case '[':
	{
		m_start[0] = !m_start[0];
		break;
	}
	case ']':
	{
		m_start[1] = !m_start[1];
		break;
	}
	case 'r':
	{
		cleanup();
		init();
		break;
	}
	case 'w':
	{
		m_pSphere->applyForce(Vec3(0.0f, 0.0f, -5.0f));
		break;
	}
	case 'a':
	{
		m_pSphere->applyForce(Vec3(-5.0f, 0.0f, 0.0f));
		break;
	}
	case 's':
	{
		m_pSphere->applyForce(Vec3(0.0f, 0.0f, 5.0f));
		break;
	}
	case 'd':
	{
		m_pSphere->applyForce(Vec3(5.0f, 0.0f, 0.0f));
		break;
	}
	case '1':
	{
		m_pCloth->m_renderMode = 0;
		break;
	}
	case '2':
	{
		m_pCloth->m_renderMode = 1;
		break;
	}
	case '3':
	{
		m_pCloth->m_renderMode = 2;
		break;
	}
	case 'f':
	{
		m_pCloth->m_resolveFaces = !m_pCloth->m_resolveFaces;
		break;
	}
	case '=':
	{
		m_pParticleSystem->generateParticles(100);
		break;
	}
	case '-':
	{
		m_pParticleSystem->generateParticles(-100);
		break;
	}
	default:
	{
		break;
	}
	}

	glutPostRedisplay();
}

void Simulator::specialFunc(const int& key, const int& x, const int& y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
	{
		m_pParticleSystem->m_collisionMode = 0;
		break;
	}
	case GLUT_KEY_F2:
	{
		m_pParticleSystem->m_collisionMode = 1;
		break;
	}
	case GLUT_KEY_F3:
	{
		m_pParticleSystem->m_collisionMode = 2;
		break;
	}
	default:
	{
		break;
	}
	}

	glutPostRedisplay();
}

void Simulator::update(const int& value)
{
	if (m_start[0])
	{
		for (int i = 0; i < value; i++)
		{
			m_pSphere->update(0.01f);

			m_pCloth->applyForce(value);
			m_pCloth->update(0.01f);
			m_pCloth->collisionResponse(0.01f, m_pGround, m_pSphere);
		}

		m_pCloth->computeNormals();
	}

	if (m_start[1])
	{
		for (int i = 0; i < value; i++)
		{
			m_pParticleSystem->applyForce(value);
			m_pParticleSystem->update(0.01f);
			m_pParticleSystem->collisionResponse(0.5f, 0.01f, m_pSphere, m_pCloth);
		}
	}
}

void Simulator::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(m_translate[0], m_translate[1], -m_zoom);
	glPushMatrix();

	glRotatef(m_rotate[0], 1.0f, 0.0f, 0.0f);
	glRotatef(m_rotate[1], 0.0f, 1.0f, 0.0f);

	//Renders the ground
	m_pGround->render();

	//Renders the rigid body (sphere)
	m_pSphere->render();

	//Renders the deformable body (cloth)
	m_pCloth->render();

	//Renders the particle system
	m_pParticleSystem->render();

	glPopMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, m_lightPos);
	glutSwapBuffers();
	glutPostRedisplay();
}

void Simulator::reshape(const int& width, const int& height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)width / (float)height, 0.1f, 500.0f);
	glViewport(0.0f, 0.0f, width, height);
}
