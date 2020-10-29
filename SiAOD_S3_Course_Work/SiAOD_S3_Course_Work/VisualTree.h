#pragma once
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class VisualTree {
private:
	RenderWindow window;

	Thread m_thread;

	void Run();
public:
	VisualTree() : m_thread(&VisualTree::Run, this) {};

	void RunThread();

};