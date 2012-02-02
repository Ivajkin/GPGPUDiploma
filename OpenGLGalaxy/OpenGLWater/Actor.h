#pragma once
#include "ActorManager.h"

class Actor {
public:
	virtual void draw() = 0;
	virtual void update() = 0;
protected:
	Actor();
};