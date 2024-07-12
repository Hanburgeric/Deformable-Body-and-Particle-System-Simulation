#include <ctime>
#include "GL/freeglut.h"
#include <stdio.h>
#include <Windows.h>

#include "Simulator.h"

//Global constants
const float WINDOW_WIDTH = 1080.0f;
const float WINDOW_HEIGHT = 720.0f;

//OpenGL callback functions
void motion(int x, int y);
void mouse(int mouseEvent, int mouseState, int x, int y);
void keyboard(unsigned char key, int x, int y);
void specialFunc(int key, int x, int y);
void update(int value);
void render();
void reshape(int width, int height);

//Simulator class
Simulator* glSimulator;

int main(int argc, char* argv[])
{
	HWND hWnd = GetConsoleWindow();
	MoveWindow(hWnd, 0, 0, 1080, 720, true);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - (int)WINDOW_WIDTH) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - (int)WINDOW_HEIGHT) / 2);
	glutInitWindowSize((int)WINDOW_WIDTH, (int)WINDOW_HEIGHT);
	glutCreateWindow("Particle Simulation");
	glEnable(GL_DEPTH_TEST);

	printf("==================================================CONTROLS==================================================\n");
	printf("Start/Pause Simulation: Spacebar\n");
	printf("Start/Pause Deformable Body Simulation: [\n");
	printf("Start/Pause Particle Simulation: ]\n");
	printf("Restart Simulation: R\n");
	printf("\n");
	printf("Move Sphere: WASD\n");
	printf("\n");
	printf("View 1 (Nodes): 1\n");
	printf("View 2 (Springs): 2\n");
	printf("View 3 (Texture): 3\n");
	printf("\n");
	printf("Add 100 Particles: +\n");
	printf("Subtract 100 Particles: -\n");
	printf("\n");
	printf("Collision Mode 1: F1 (Checks every particle with every particle)\n");
	printf("Collision Mode 2: F2 (Checks every particle except those already checked)\n");
	printf("Collision Mode 3: F3 (Hash grid implementation of mode 1)\n");
	printf("Collision Mode 4 (Toggle): F (Attempts to resolve issue of the solid appearing through the deformable body)\n");
	printf("\n");
	printf("Move Camera: LMB\n");
	printf("Rotate Camera: RMB\n");
	printf("Camera Zoom: Mouse Wheel\n");
	printf("============================================================================================================\n");

	//Random seed
	srand((unsigned int)time(NULL));

	glSimulator = new Simulator();
	glSimulator->init();

	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialFunc);
	glutTimerFunc(10, update, 20);
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutMainLoop();

	delete glSimulator;
	glSimulator = nullptr;

	return 0;
}


void motion(int x, int y)
{
	glSimulator->motion(x, y);
}

void mouse(int mouseEvent, int mouseState, int x, int y)
{
	glSimulator->mouse(mouseEvent, mouseState, x, y);
}

void keyboard(unsigned char key, int x, int y)
{
	glSimulator->keyboard(key, x, y);
}

void specialFunc(int key, int x, int y)
{
	glSimulator->specialFunc(key, x, y);
}

void update(int value)
{
	glSimulator->update(value);
	glutPostRedisplay();
	glutTimerFunc(10, update, 20);
}

void render()
{
	glSimulator->render();
}

void reshape(int width, int height)
{
	glSimulator->reshape(width, height);
}
