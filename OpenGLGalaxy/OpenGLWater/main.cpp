// OpenGLWater.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <exception>
#include "Actor.h"
#include "ActorManager.h"
#include "Vector2.h"
#include "UnitTest.h"
#include <cmath>
#include <glut.h>
#include "AutoProcessProfiler.h"
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

double random() {
	return rand()%100000 / 100000.0;
}
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
	

struct point {
	point()
		: mass(random()*1.1+0.1),
			position(random()*0.5 - random()*0.5, random()*0.5 - random()*0.5, random()*0.5 - random()*0.5),
			velocity(random()*2 - random()*2, random()*2 - random()*2, random()*2 - random()*2),
			acceleration(0,0,0)
	{
	}
	const double mass;
	Vector2 position,
			velocity,
			acceleration;
};

class Star : public Actor {
public:
	Star() {
		color[0] = random()*0.5 + 0.5;
		color[1] = random()*0.5 + 0.5;
		color[2] = random()*0.5 + 0.5;
		color[3] = random()*0.5 + 0.5;

		memcpy(colorDim1, color, sizeof(float)*3);
		colorDim1[3] = color[3]*0.5;
		memcpy(colorDim2, color, sizeof(float)*3);
		colorDim2[3] = color[3]*0.35;
		memcpy(colorDim3, color, sizeof(float)*3);
		colorDim3[3] = color[3]*0.075;
	}
	void draw() {
		{
			glColor4fv(color);
			const double radius = model.mass*0.02;
			Vector2 lt(model.position.getX() - radius, model.position.getY() - radius, 0),
					rb(model.position.getX() + radius, model.position.getY() + radius, 0);
			glRectf(lt.getX(), lt.getY(), rb.getX(), rb.getY());
		} {
			glColor4fv(colorDim1);
			const double radius = model.mass*0.032;
			Vector2 lt(model.position.getX() - radius, model.position.getY() - radius, 0),
					rb(model.position.getX() + radius, model.position.getY() + radius, 0);
			glRectf(lt.getX(), lt.getY(), rb.getX(), rb.getY());
		} {
			glColor4fv(colorDim2);
			const double radius = model.mass*0.082;
			Vector2 lt(model.position.getX() - radius, model.position.getY() - radius, 0),
					rb(model.position.getX() + radius, model.position.getY() + radius, 0);
			glRectf(lt.getX(), lt.getY(), rb.getX(), rb.getY());
		} {
			glColor4fv(colorDim3);
			const double radius = model.mass*0.282;
			Vector2 lt(model.position.getX() - radius, model.position.getY() - radius, 0),
					rb(model.position.getX() + radius, model.position.getY() + radius, 0);
			glRectf(lt.getX(), lt.getY(), rb.getX(), rb.getY());
		}
	}
	void update() {}
	point model;
private:
	float color[4];
	float colorDim1[4];
	float colorDim2[4];
	float colorDim3[4];
};

class Logic {
public:
	Logic() {
		for(int i = 0; i < 1024; ++i) {
			field.push_back(new Star());
		}
	}
	~Logic() {
		while(field.size()) {
			delete field.back();
			field.pop_back();
		}
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
					if(squaredRangeLength > 0.001) {
						// m*M/r^2
						a.acceleration += range * a.mass * b.mass / (squaredRangeLength*sqrt(squaredRangeLength));
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

const int SCREEN_WIDTH = 1300, SCREEN_HEIGHT = 700;

namespace gl_callback {
	void init() {
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
}

int _tmain(int argc, char* argv[])
{
	{
		int* i = new int(8);
		srand((int)i);
		delete i;
	}
	// UNIT TESTS!
	UnitTest::Run();
	printf("Ready to go!");
	// MAIN
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("N body galaxy");
	glutDisplayFunc(gl_callback::draw);
	glutIdleFunc(gl_callback::idle);
	gl_callback::timer(0);

	gl_callback::init();

	glutMainLoop();
	return 0;
}
