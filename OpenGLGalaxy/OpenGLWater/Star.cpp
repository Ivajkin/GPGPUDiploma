#include "_stdafx.h"
#include "Star.h"
#include <glut.h>

bool isDark = false;

Star::Star() {
	//colorDim[0][0] = sqrt(random())*0.6 + 0.4;
	//colorDim[0][1] = random();
	// r*r + g*g + b*b = 2+random();
	// b = sqrt(2+random() - g*g - r*r);
	//colorDim[0][2] = sqrt(2 + random() - colorDim[0][0]*colorDim[0][0] - colorDim[0][1]*colorDim[0][1]);
	if(random() < 0.15) {
		isDark = !isDark;
	}
	if(isDark) {
		colorDim[0][0] = random()*random()*random()*0.8 + 0.05;
		colorDim[0][1] = random()*0.6 + 0.25;
		colorDim[0][2] = random()*random()*0.8 + 0.05;
	} else {
		colorDim[0][0] = random()*random()*random()*0.7 + 0.3;
		colorDim[0][1] = random()*random()*0.55 + 0.45;
		colorDim[0][2] = random()*random()*random();
	}
	/*{
		bool isRed = ( random() < 0.2 );
		if(isRed) {
			colorDim[0][0] = 1;
			colorDim[0][1] = random()*random()*random()*random();
			colorDim[0][2] = random()*random()*random()*random();
		} else {
			bool isBlue = ( random() < 0.2 );
			if(isBlue) {
				colorDim[0][0] = random()*random()*random()*random();
				colorDim[0][1] = random()*random()*random()*random();
				colorDim[0][2] = 1;
			}
		}
	}*/
	colorDim[0][3] = random()*0.6 + 0.4;

	memcpy(colorDim[1], colorDim[0], sizeof(float)*3);
	colorDim[1][3] = colorDim[0][3]*0.5;
	memcpy(colorDim[2], colorDim[0], sizeof(float)*3);
	colorDim[2][3] = colorDim[0][3]*0.35;
	memcpy(colorDim[3], colorDim[0], sizeof(float)*3);
	colorDim[3][3] = colorDim[0][3]*0.075;
	memcpy(colorDim[4], colorDim[0], sizeof(float)*3);
	colorDim[4][3] = colorDim[0][3]*0.025;

	
	colorDim[0][0] = 1;
	colorDim[0][1] = 1;
	colorDim[0][2] = 1;
	colorDim[0][3] = 1;

	radius[0] = model.mass*0.02;
	radius[1] = model.mass*0.016;
	radius[2] = model.mass*0.041;
	radius[3] = model.mass*0.141;
	radius[4] = model.mass*0.241;
}
void Star::draw() {
	for(int i = 0; i < 5; ++i) {
	//for(int i = 4; i > -1; --i) {
		glColor4fv(colorDim[i]);
		glBegin(GL_TRIANGLE_FAN);
		for(float a = 0; a < M_PI*2; a += 0.5) {
			if(model.position.getZ() < -9) {
				continue;
			}
			const float rad = radius[i] / (10 + model.position.getZ()*0.5)*10;
			float x = rad*cos(a) + model.position.getX(), y = rad*sin(a) + model.position.getY();
			glVertex2f(x, y);
		}
		glEnd();
	}
}