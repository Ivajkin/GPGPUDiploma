// OpenGLWater.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <exception>
#include "Actor.h"
#include "ActorManager.h"
#include "Vector2.h"
#include "UnitTest.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <glut.h>
#include "AutoProcessProfiler.h"
#include "Star.h"
/*
class R2Iterator {
public:
	R2Iterator(float* position, const int x, const int y, const int width, const int height)
		: x(x), y(y), width(width), height(height)				
	{
		_ptr = position;
	}
	R2Iterator& operator*() const {
		return *this;
	}
	R2Iterator& operator++() {
		++_ptr;
		++x;
		if(x == width) {
			x = 0;
			++y;
		}
		assert(height > y);
		return *this;
	}
	bool operator!=(R2Iterator& sec) const {
		return sec._ptr != this->_ptr;
	}
	const int getX() const {
		return x;
	}
	const int getY() const {
		return y;
	}
private:
	int x, y;
	const int width, height;
	float* _ptr;
};

class R2 {
public:
	R2() {
		space = new float[width*height];
	}
	~R2() {
		delete [] space;
	}
	float& get(int x, int y) {
		assert((width > x && height > y && 0 <= x && 0 <= y) && "index out of bound");
		int n = x + y*width;
		return space[n];
	}
	typedef R2Iterator iterator;
	const iterator begin() const {
		return iterator(space, 0, 0, width, height);
	}
	const iterator end() const {
		return iterator(space + width*height, width, height, width, height);
	}
private:
	float* space;
	static const int width = 512, height = 256;
};
class World {
public:
	World() {
	}
	void draw() {
		std::for_each(field.begin(), field.end(), drawElement);
	}
private:
	static void drawElement(R2::iterator value) {
		value.getX();
	}
	R2 field;
};

World world;*/
/*
class Star : public Actor {
public:
	Star() {
		universe.push_back(this);
	}
	void update() {
		for(std::vector<Star*>::iterator i = universe.begin(); i != universe.end(); ++i) {
			Star& sec = (*i);
		}
	}
private:
	std::vector<Star*> universe;
	double mass;
	point now, future;
};
*/

class Logic {
public:
	Logic() {
		for(int i = 0; i < 1024; ++i) {
			createStar();
		}
	}
	~Logic() {
		while(field.size()) {
			delete field.back();
			field.pop_back();
		}
	}
	void createStar() {
		field.push_back(new Star());
	}
	void update( /* seconds */ double elapsedTime) {
		
		static AutoProcessProfiler gravityComputationProcess("Gravity Computation Process");

		// cudaDeviceSynchronize()
		gravityComputationProcess.start_measurement();
		for(std::vector<Star*>::iterator i = field.begin(); i != field.end(); ++i) {
			for(std::vector<Star*>::iterator j = field.begin(); j != field.end(); ++j) {
				point& a = (*i)->model;
				a.acceleration = Vector2::Zero;
				if(i != j) {
					point& b = (*j)->model;
					Vector2 range = b.position - a.position;
					double squaredRangeLength = range.squaredLength();
					// ����������� ����.
					a.velocity = a.velocity * pow(0.995, elapsedTime);
					// ������������ �� �������.
					if(squaredRangeLength > 0.005) {
						// m*M/r^2
						a.acceleration += range * a.mass * b.mass / (squaredRangeLength*sqrt(squaredRangeLength)) * 20;
						// Ҹ���� ������� ��� ������������ ��������.
						//a.acceleration -= range * a.mass * b.mass * squaredRangeLength * 0.01;
					} else {
						// ��������� ���� (��������������).
						a.velocity = a.velocity * pow(0.6, elapsedTime);
					}
				}
			}
		}
		for(std::vector<Star*>::iterator i = field.begin(); i != field.end(); ++i) {
			point& a = (*i)->model;
			a.velocity += a.acceleration * elapsedTime;
			a.position += a.velocity * elapsedTime;
		}
		// cudaDeviceSynchronize()
		gravityComputationProcess.end_measurement();
	}
private:
	std::vector<Star*> field;
} appLogic;

const int SCREEN_WIDTH = 1366, SCREEN_HEIGHT = 768;

void switchWindowedMode();

namespace gl_callback {
	
	int current_window;
	void init(const int current_window) {
		gl_callback::current_window = current_window;

		glClearColor(0.0,0.0,0.0,1.0);
		glClearDepth(0.1);
		glEnable(GL_DEPTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	void draw() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		appLogic.update(1.0/60.0);
		ActorManager::instance()->update();
		
		static AutoProcessProfiler drawStarProcess("Draw Process");
		glFlush();
		drawStarProcess.start_measurement();
			glPushMatrix();
			glScalef(300.0/SCREEN_WIDTH, 300.0/SCREEN_HEIGHT, 1);
			ActorManager::instance()->draw();
			glPopMatrix();
		glFlush();
		drawStarProcess.end_measurement();
		//drawStarProcess.display_info();

		glutSwapBuffers();
	}
	void idle() {
	}
	void timer(int frame) {
		draw();
		glutTimerFunc(1000/60, timer, frame+1);
	}
	void keyboard(unsigned char key, int x, int y) {
		switch(key) {
			case 13:
				switchWindowedMode();
				break;
			case 27:
				glutDestroyWindow(current_window);
				exit(0);
				break;
			case 115:
				for(int i = 0; i < 100; ++i) {
					appLogic.createStar();
				}
				break;
		}
	}
}

void switchWindowedMode() {	
	static bool isFullscreenModeEnabled = false;
	if(isFullscreenModeEnabled) {
		//glutLeaveGameMode();
		glutReshapeWindow(SCREEN_WIDTH-100, SCREEN_HEIGHT-100);
		glutPositionWindow(50,50);
	} else {
		glutFullScreen();
		//glutEnterGameMode();
		//glutDisplayFunc(gl_callback::draw);
	}
	isFullscreenModeEnabled = !isFullscreenModeEnabled;
}

int _tmain(int argc, char* argv[])
{
	{
		// random() test
		int itertion_count = 0,
			up_itertion_count = 0,
			down_itertion_count = 0;
		double value = 0,
			up_value = 0,
			down_value = 0;
		for(int i = 0; i < 1000000; ++i) {
			++itertion_count;
			double val = random();
			value += val;
			if(val > 0.5) {
				up_value += val;
				++up_itertion_count;
			} else {
				down_value += val;
				++down_itertion_count;
			}
		}
		// Must be about 0.5
		printf("Average: %f\n", (float)(value/itertion_count));
		printf("Average upper value: %f\n", (float)(up_value/up_itertion_count));
		printf("Average downward: %f\n", (float)(down_value/down_itertion_count));
	}
	{
		//int* i = new int(8);
		//srand((int)i);
		//delete i;
		srand(clock());
	}
	// UNIT TESTS!
	UnitTest::Run();
	printf("Ready to go!\n");
	// MAIN
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	const int current_window = glutCreateWindow("N body galaxy");
	glutDisplayFunc(gl_callback::draw);
	glutIdleFunc(gl_callback::idle);
	glutKeyboardFunc(gl_callback::keyboard);
	gl_callback::timer(0);

	gl_callback::init(current_window);

	glutMainLoop();
	return 0;
}

