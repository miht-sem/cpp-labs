#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>
#include <cstring>

#include <queue>

#include "cube.h"
#include "genetic_algorithm.h"

#define TIMER 1

unsigned int colors_of_cube[6] = {0xFFFFFF, 0xffd500, 0x0046ad, 0x009b48, 0xb71234, 0xff5800};
int x_rot = -60, y_rot = 180, z_rot = 0;
double translateZ = -30.0;
Cube cube(colors_of_cube);

GLfloat light_position[] = {500, 500, 500, 0};

bool auto_turning = false;
bool is_solving = false;
bool is_algorithm = false;
bool is_reversed = false;
int algorithm_count = 0;
int cant_find_solution = 0;
int current_direction = 1;

std::queue<std::string> queue_of_turns;

void gen_algorithm(Genetic_Algorithm &algo)
{
	auto solution = algo.algorithm();
	if (solution.second == "Сube is already solved")
	{
		algorithm_count = 0;
		is_algorithm = false;
		std::cout << solution.second << "\n";
		return;
	}

	if (solution.first)
	{
		std::cout << "Solved: \n"
				  << solution.second << "\n";
	}
	else
	{
		std::cout << "Сan't find solution"
				  << "\n";
		cant_find_solution += 1;
		algorithm_count += 1;
		is_algorithm = false;
		return;
	}

	is_solving = true;
	std::istringstream stream(solution.second);
	std::string step;

	while (stream >> step)
		queue_of_turns.push(step);

	is_algorithm = false;
	algorithm_count += 1;
}

void display()
{
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 0, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glTranslatef(0, 0, translateZ);
	glRotatef(x_rot, 1, 0, 0);
	glRotatef(y_rot, 0, 1, 0);
	glRotatef(z_rot, 0, 1, 1);
	glTranslatef(CUBE_SIZE / -2.0, CUBE_SIZE / -2.0, CUBE_SIZE / -2.0);
	cube.draw();
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat fAspect = (GLfloat)w / (GLfloat)h;
	gluPerspective(60, fAspect, 1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	srand(time(0));
	float mat_specular[] = {0.3, 0.3, 0.3, 0.0};
	float diffuse_light[] = {0.2, 0.2, 0.2, 1.0};
	float ambient_light[] = {0.9, 0.9, 0.9, 1.0};
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	cube.clear(colors_of_cube);
}

void save_cube(const std::string &file_name)
{
	std::ofstream outf(file_name);

	if (!outf)
		throw std::runtime_error("Could not open file for writing!\n");

	outf << cube;
	std::cout << "Saved current state of Cube"
			  << "\n";
	outf.close();
}

void load_cube(const std::string &file_name)
{
	std::ifstream inf(file_name);

	if (!inf)
		throw std::runtime_error("Could not open file for reading!\n");

	inf >> cube;
	std::cout << "Load last saved state of Cube!"
			  << "\n";
	inf.close();
}

void special_keys(int key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_DOWN:
		x_rot += 10;
		if (x_rot >= 360)
			x_rot -= 360;
		glutPostRedisplay();
		break;

	case GLUT_KEY_UP:
		x_rot -= 10;
		if (x_rot < 0)
			x_rot += 360;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		y_rot += 10;
		if (y_rot >= 360)
			y_rot -= 360;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		y_rot -= 10;
		if (y_rot < 0)
			y_rot += 360;
		glutPostRedisplay();
		break;

	case GLUT_KEY_HOME:
		translateZ += 5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_END:
		translateZ -= 5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_F1:
		cube.clear(colors_of_cube);
		glutPostRedisplay();
		break;

	case GLUT_KEY_F2:
		save_cube("cube_state.txt");
		break;

	case GLUT_KEY_F3:
		load_cube("cube_state.txt");
		break;

	default:
		return;
	}
}

void keys(unsigned char key, int, int)
{
	if (cube.current_turn == -1 && key >= '0' && key <= '7')
	{
		if (is_reversed) current_direction = -1;
		else current_direction = 1;
		
		cube.rotate(key - '0', 1, 9);
		display();
	}

	if (key == 'r')
		is_reversed = !is_reversed;

	if (key == 's')
	{
		if (is_algorithm)
		{
			std::cout << "Algorithm is already in process!"
					  << "\n";
			return;
		}
		if (is_solving)
		{
			std::cout << "Cube turning in process!"
					  << "\n";
			return;
		}
		if (auto_turning)
		{
			std::cout << "Auto turning is on! Stop it first."
					  << "\n";
			return;
		}
		std::cout << "Algorithm in process..."
				  << "\n";
		is_algorithm = true;
		Genetic_Algorithm algo(cube);
		gen_algorithm(algo);
	}

	if (key == 27)
		exit(0);
}

void mouse(int key, int state, int, int)
{
	if (key == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		auto_turning = !auto_turning;
	}
}

void timer(int)
{
	glutTimerFunc(TIMER, timer, 0);

	if (cube.current_turn != -1)
		cube.rotate(cube.current_turn, current_direction, 9);

	if (is_solving)
	{
		auto_turning = false;
		if (cube.current_turn == -1)
		{
			if (!queue_of_turns.empty())
			{
				std::string turn = queue_of_turns.front();
				queue_of_turns.pop();
				current_direction = cube.rotate(turn, 9);
			}
			else
			{
				is_solving = false;
				std::cout << '\n';
			}
		}
	}
	else if (auto_turning)
	{
		if (cube.current_turn == -1 && algorithm_count == 0)
			keys(rand() % 7 + '0', 0, 0);
	}
	else if (cube.calculate_incorrect_colors() == 0)
	{
		algorithm_count = 0;
	}
	else if (cant_find_solution == 3)
	{
		std::cout << "\nINVARIANT\n";
		algorithm_count = 0;
		cant_find_solution = 0;
	}
	else if (algorithm_count > 0 && cube.calculate_incorrect_colors() != 0 && !is_algorithm)
	{
		Genetic_Algorithm algo(cube);
		gen_algorithm(algo);

		if (algorithm_count > 3)
		{
			algorithm_count = 0;
		}
	}
	display();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(1, 1);
	glutCreateWindow("Rubik's Cube");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keys);
	glutMouseFunc(mouse);
	glutTimerFunc(TIMER, timer, 0);
	glutSpecialFunc(special_keys);
	glutMainLoop();
	return 0;
}