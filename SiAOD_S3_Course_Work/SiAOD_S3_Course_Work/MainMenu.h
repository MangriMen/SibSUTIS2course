#pragma once
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class MainMenu
{
private:
	RenderWindow window;

	Thread m_thread;

	void Run();
public:
	MainMenu() : m_thread(&MainMenu::Run, this) {};

	void RunThread();

};

