#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

namespace form
{
	enum Align {
		Left,
		Center,
		Right
	};
	
	namespace fColor {
		static const Color Background = Color(38, 50, 56);
		static const Color Button = Color(69, 90, 100);
		static const Color Outline = Color(55, 71, 79);
	}

	class Label {
	protected:
		Text text;
		Font font;
		int align;
	public:
		Label();

		Label(String text_, Vector2f pos_, size_t size, int align_ = Align::Left, String font_ = "fonts/Roboto-Medium.ttf");

		void Initialize(String text_, Vector2f pos_, size_t size, int align_ = Align::Left, String font_ = "fonts/Roboto-Medium.ttf");

		Text getText();

		void setText(String text_);

		void setPosition(Vector2f pos_);
	};

	class Button : public Label{
	protected:
		RectangleShape rec;
	public:
		Button();

		Button(String text_, Vector2f pos_, Vector2f rec_, size_t size);

		RectangleShape getButton();
		
		void setPosition(Vector2f pos_);

		void setOutlineThickness(int width_);

		void setOutlineColor(Color color_);

		void setFillColor(Color color_);

		void setSize(Vector2f rec_);

		Vector2f getPosition();

		Vector2f getSize();

		int getOutlineThickness();
	};

	class Table : public Button {
	private:
		Button** cells = nullptr;
		Vector2i proportions;
		Vector2f size;
	public:
		Table(size_t row_, size_t col_);

		Button** getCells();

		size_t cols();

		size_t rows();

		void update();

		int width();
		
		int height();

		void setPosition(Vector2f pos_);
	};
};

