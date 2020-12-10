#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include "Program.h"
#include "MainMenu.h"
#include "Reader.h"

Program::Program()
{
}

void Program::Run() {
	srand(static_cast<unsigned>(time(NULL)));
	MainMenu mainMenu;
	VisualTree visualTree;
	Reader *reader = new Reader();

	mainMenu.reader = reader;
	visualTree.reader = reader;

	mainMenu.RunThread();
	visualTree.RunThread();
}