#pragma once
#include <vector>

using std::vector;

class Actor;
class ActorManager {
private:
	ActorManager() {}
	~ActorManager() {}
public:
	void update();
	void draw();
	void add(Actor*);
	static ActorManager* instance();
private:
	vector<Actor*> actors;
	static ActorManager* singletonPtr;
};
