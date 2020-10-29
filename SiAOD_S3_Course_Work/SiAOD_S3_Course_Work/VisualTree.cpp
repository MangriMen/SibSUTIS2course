#include <thread>
#include "VisualTree.h"
#include "Form.h"

void VisualTree::Run()
{
	window.create(VideoMode(1024, 800), "VisualTree");
	window.setVerticalSyncEnabled(true);
	View view(FloatRect(0, 0, 1024, 800));

	Build(*&reader->btree.root, 1, "R");

	while (window.isOpen()) {
		Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::MouseWheelScrolled:
				if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
					if (event.mouseWheelScroll.delta > 0)
						zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, (1.f / 1.1f));
					else if (event.mouseWheelScroll.delta < 0)
						zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, 1.1f);
				}
				break;
			case Event::Resized:
				view.reset(FloatRect(0.f, 0.f, event.size.width, event.size.height));
				view.zoom(1.0f);
				window.setView(view);
				break;
			default:
				break;
			}
		}

		window.clear(form::fColor::Background);

		for (int i = 0; i < size-1; i++) {
			window.draw(nodes[i]->circle);
			//window.draw(nodes[i]->text);
		}

		window.display();
	}
}

void VisualTree::RunThread()
{
	m_thread.launch();
}
