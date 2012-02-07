#include "_stdafx.h"
#include "ActorManager.h"
#include "Actor.h"

void ActorManager::add(Actor* actor) {
	actors.push_back(actor);
}

void ActorManager::update() {
	for(vector<Actor*>::iterator i = actors.begin(); i != actors.end(); ++i) {
		(*i)->update();
	}
}

void ActorManager::draw() {
	for(vector<Actor*>::iterator i = actors.begin(); i != actors.end(); ++i) {
		(*i)->draw();
	}
}

ActorManager* ActorManager::instance() {
	if(!singletonPtr) {
		singletonPtr = new ActorManager();
	}
	return singletonPtr;
}

ActorManager* ActorManager::singletonPtr = 0;