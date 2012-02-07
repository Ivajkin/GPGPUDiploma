#include "_stdafx.h"
#include "Actor.h"

Actor::Actor() {
	ActorManager::instance()->add(this);
}