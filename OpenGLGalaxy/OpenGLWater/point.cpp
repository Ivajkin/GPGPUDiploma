#include "_stdafx.h"
#include "point.h"

point::point() 
	: mass(random()*1.1+0.1),
		position(emitter.getX() + random()*random()*0.6 - random()*0.6,
				emitter.getY() + random()*0.6 - random()*0.6,
				emitter.getZ() + random()*0.6 - random()*0.6),
		velocity(velocity_emitter.getX() + random()*2 - random()*2,
				velocity_emitter.getX() + random()*2 - random()*2,
				velocity_emitter.getX() + random()*2 - random()*2),
		acceleration(0,0,0)
{
	if(random() < 0.1605) {
		emitter = Vector3(
			random()*9-random()*9,
			random()*9-random()*9,
			random()*9-random()*9);
		velocity_emitter = Vector3(
			random()*9-random()*9,
			random()*9-random()*9,
			random()*9-random()*9);
	}
}

Vector3 point::emitter(0,0,0);
Vector3 point::velocity_emitter(0,0,0);