#include "VisualTree.h"
#include "Form.h"

void VisualTree::Run()
{
	window.create(VideoMode(1024, 800), "VisualTree");
	window.setVerticalSyncEnabled(true);

	View view(FloatRect(0, 0, 1024, 800));

	while (window.isOpen()) {
		Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}

		window.clear(form::fColor::Background);

		window.display();
	}
}

void VisualTree::RunThread()
{
	m_thread.launch();
}
