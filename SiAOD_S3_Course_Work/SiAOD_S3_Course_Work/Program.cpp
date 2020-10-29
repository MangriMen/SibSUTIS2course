#include <iostream>
#include <SFML/Graphics.hpp>
#include "Program.h"
#include "MainMenu.h"
#include "Reader.h"

Program::Program()
{
}

void Program::Run() {
	MainMenu mainMenu;
	VisualTree visualTree;
	Reader *reader = new Reader();

	mainMenu.reader = *&reader;
	visualTree.reader = *&reader;

	mainMenu.RunThread();
	//visualTree.RunThread();
}