#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include "Particle.h"

using namespace sf;
using namespace std;

class Engine
{
private:
	// a regular RenderWindow
	RenderWindow m_Window;

	//vector for particles
	vector<Particle> m_particles;

	// private functions for internal use only
	void input();
	void update(float dtAsSeconds);
	void draw();


public:
	// the engine constructor
	Engine();

	// run will call all the private functions
	void run();

};