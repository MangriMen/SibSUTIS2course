#pragma once
#include <thread>
#include <SFML/Graphics.hpp>
#include "Reader.h"
#include "TreeVertex.h"
#include "InfoCard.h"

using namespace std;
using namespace sf;

class VisualTree
{
private:
	RenderWindow window;

	Thread m_thread;

	InfoCard* info;

	void Run();
public:
	Reader* reader;

	VisualTree() : m_thread(&VisualTree::Run, this), reader(nullptr), info(nullptr) { };

	void RunThread();
};
