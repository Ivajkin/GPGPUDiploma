#include "_stdafx.h"
#include "point.h"

point::point() 
	: mass(random()*1.1+0.1),
		position(emitter.getX() + random()*random()*0.5 - random()*0.5,
				emitter.getY() + random()*0.5 - random()*0.5,
				emitter.getZ() + random()*0.5 - random()*0.5),
		velocity(random()*2 - random()*2,
				random()*2 - random()*2,
				random()*2 - random()*2),
		acceleration(0,0,0)
{
	if(random() < 0.1305) {
		emitter = Vector2(
			random()*9-random()*9,
			random()*9-random()*9,
			random()*9-random()*9);
	}
}

Vector2 point::emitter(0,0,0);