#include "Form.h"

form::Label::Label()
{
	font.loadFromFile("fonts/Roboto-Medium.ttf");
	align = Align::Left;
	text.setFont(font);
	text.setString("Undefined");
	text.setFillColor(Color::Black);
}

form::Label::Label(String text_, Vector2f pos_, size_t size, int align_, String font_)
{
	font.loadFromFile(font_);
	align = align_;
	text.setFont(font);
	text.setString(text_);
	text.setFillColor(Color::White);
	text.setCharacterSize(size);
	switch (align_)
	{
	case form::Left:
		break;
	case form::Center:
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height);
		break;
	case form::Right:
		break;
	default:
		break;
	}
	text.setPosition(pos_);
}

void form::Label::Initialize(String text_, Vector2f pos_, size_t size, int align_, String font_)
{
	font.loadFromFile(font_);
	align = align_;
	text.setFont(font);
	text.setString(text_);
	text.setFillColor(Color::White);
	text.setCharacterSize(size);
	switch (align_)
	{
	case form::Left:
		break;
	case form::Center:
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height);
		break;
	case form::Right:
		break;
	default:
		break;
	}
	text.setPosition(pos_);
}

Text form::Label::getText() {
	return text;
}

void form::Label::setText(String text_)
{
	text.setString(text_);
	switch (align)
	{
	case form::Left:
		break;
	case form::Center:
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height);
		break;
	case form::Right:
		break;
	default:
		break;
	}
}

void form::Label::setPosition(Vector2f pos_)
{
	text.setPosition(pos_);
}

form::Button::Button() {
	text.setString("undef");
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height);

	rec.setSize(Vector2f(100, 60));
	//rec.setOrigin(rec.getLocalBounds().width / 2, rec.getLocalBounds().height / 2);

	rec.setOutlineThickness(8);
	rec.setFillColor(fColor::Button);
	rec.setOutlineColor(fColor::Outline);
}

form::Button::Button(String text_, Vector2f pos_, Vector2f rec_, size_t size)
{
	rec.setSize(rec_);
	//rec.setOrigin(rec.getLocalBounds().width / 2, rec.getLocalBounds().height / 2);
	rec.setPosition(pos_);

	rec.setOutlineThickness(-8);
	rec.setFillColor(fColor::Button);
	rec.setOutlineColor(fColor::Outline);

	text.setString(text_);
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height);
	text.setPosition(pos_ + Vector2f(rec.getLocalBounds().width / 2, rec.getLocalBounds().height / 2 - text.getLocalBounds().height / 10));
}

RectangleShape form::Button::getButton()
{
	return rec;
}

void form::Button::setPosition(Vector2f pos_)
{
	rec.setPosition(pos_);
	text.setPosition((Vector2f(rec.getSize().x / 2.0f, rec.getSize().y / 1.6f)) + pos_);
}

void form::Button::setOutlineThickness(int width_) {
	rec.setOutlineThickness(width_);
}

void form::Button::setOutlineColor(Color color_) {
	rec.setOutlineColor(color_);
}

void form::Button::setFillColor(Color color_) {
	rec.setFillColor(color_);
}

void form::Button::setSize(Vector2f rec_) {
	rec.setSize(rec_);
	//rec.setOrigin(rec.getLocalBounds().width / 2, rec.getLocalBounds().height / 2);
	rec.setPosition(rec.getPosition());
}

void form::Button::setOriginCenter()
{
	rec.setOrigin(rec.getLocalBounds().width / 2, rec.getLocalBounds().height / 2);
	//text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	//text.setPosition(rec.getPosition());
}

Vector2f form::Button::getPosition()
{
	return rec.getPosition();
}

Vector2f form::Button::getSize()
{
	return rec.getSize();
}

int form::Button::getOutlineThickness()
{
	return rec.getOutlineThickness();
}

form::Table::Table(size_t cols_, size_t rows_) {
	proportions = Vector2i(cols_, rows_);
	size = Vector2f(0, 0);
	cells = new Button * [cols_];
	for (int col = 0; col < cols_; col++) {
		cells[col] = new Button[rows_];;
	}

	for (int col = 0; col < cols_; col++) {
		for (int row = 0; row < rows_; row++) {
			cells[col][row].setOutlineThickness(-8);
			cells[col][row].setOutlineColor(fColor::Outline);
			cells[col][row].setFillColor(fColor::Button);
			cells[col][row].setSize(Vector2f(100, 60));

			cells[col][row].Initialize(to_string(col) + ' ' + to_string(row), Vector2f(0, 0), 16, Align::Center);
		}
	}
}

form::Button** form::Table::getCells()
{
	return cells;
}

size_t form::Table::cols()
{
	return proportions.x;
}

size_t form::Table::rows()
{
	return proportions.y;
}

void form::Table::update()
{
	size = Vector2f(0, 0);

	for (int i = 0; i < cols(); i++) {
		size.x += getCells()[i][0].getButton().getSize().x;
	}
	for (int i = 0; i < rows(); i++) {
		size.y += getCells()[0][i].getButton().getSize().y;
	}

	size.x -= (cols() - 1) * abs(getCells()[0][0].getButton().getOutlineThickness());
	size.y -= (rows() - 1) * abs(getCells()[0][0].getButton().getOutlineThickness());
}

int form::Table::width()
{
	return size.x;
}

int form::Table::height()
{
	return size.y;
}

Vector2f form::Table::getPosition() {
	return cells[0][0].getPosition();
}

void form::Table::setPosition(Vector2f pos_)
{
	update();
	for (int col = 0; col < cols(); col++) {
		for (int row = 0; row < rows(); row++) {
			int bWidth = abs(cells[col][row].getOutlineThickness());

			float offsetX = col == 0 ? pos_.x : -bWidth;
			float offsetY = row == 0 ? pos_.y : -bWidth;

			float prevX = col == 0 ? 0 : cells[col - 1][row].getSize().x + cells[col - 1][row].getPosition().x;
			float prevY = row == 0 ? 0 : cells[col][row - 1].getSize().y + cells[col][row - 1].getPosition().y;

			cells[col][row].setPosition(Vector2f(prevX + offsetX, prevY + offsetY));
		}
	}
}