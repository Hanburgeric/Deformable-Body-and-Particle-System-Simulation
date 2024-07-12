#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Vector.h"

class Cloth;
class Ground;
class ParticleSystem;
class Sphere;

class Simulator
{
public:
	Simulator();
	~Simulator();

	void init();
	void cleanup();
	void motion(const int& x, const int& y);
	void mouse(const int& mouseEvent, const int& mouseState, const int& x, const int& y);
	void keyboard(const unsigned char& key, const int& x, const int& y);
	void specialFunc(const int& key, const int& x, const int& y);
	void update(const int& value);
	void render();
	void reshape(const int& width, const int& height);

	bool m_start[2];
	int m_mousePos[2];
	bool m_mouseEvent[2];
	float m_translate[2];
	float m_rotate[2];
	float m_zoom;

	float m_lightPos[4];

	GLuint m_texture;
	Ground* m_pGround;
	Sphere* m_pSphere;
	Cloth* m_pCloth;
	ParticleSystem* m_pParticleSystem;
};

#endif
