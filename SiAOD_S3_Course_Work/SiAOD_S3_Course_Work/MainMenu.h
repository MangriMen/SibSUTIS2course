#pragma once
#include <SFML/Graphics.hpp>
#include "Reader.h"

using namespace std;
using namespace sf;

class MainMenu
{
private:
	RenderWindow window;

	Thread m_thread;

	void Run();
public:
	Reader* reader;

	MainMenu() : m_thread(&MainMenu::Run, this), reader(nullptr) {};

	void RunThread();
};

