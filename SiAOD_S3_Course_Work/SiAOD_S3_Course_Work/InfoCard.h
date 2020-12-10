#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Employee.h"
#include "Form.h"
#include "TreeColor.h"
#include "Utils.h"

using namespace sf;

class InfoCard
{
public:
	RectangleShape background;
	static String placeholder[4];
	form::Label tFIO;
	form::Label tDepartmentNumber;
	form::Label tPlace;
	form::Label tBirthDate;
	form::Button  close;

	InfoCard()
	{
		background.setFillColor(TreeColor::Background);
		background.setPosition(0, 0);
		background.setSize(Vector2f(500, 300));
		background.setOutlineThickness(-6);
		background.setOrigin(0, background.getLocalBounds().height);

		Vector2f top = background.getPosition() + Vector2f(0, -background.getLocalBounds().height);

		tFIO.Initialize(L"ФИО: ", top + Vector2f(10, 10), 24, 0);
		tDepartmentNumber.Initialize(L"Номер отдела: ", top + Vector2f(10, 50), 24, 0);
		tPlace.Initialize(L"Должность: ", top + Vector2f(10, 90), 24, 0);
		tBirthDate.Initialize(L"Дата рождения: ", top + Vector2f(10, 130), 24, 0);

		close = form::Button("X", Vector2f(0, 0), Vector2f(25, 25), 24);
		close.setText("X");
		close.setOutlineThickness(-2);
		close.setOutlineColor(Color::White);
		close.setOriginCenter();
		close.setPosition(background.getPosition() + Vector2f(background.getLocalBounds().width, -background.getLocalBounds().height) + Vector2f(-3.5f, 3.5f));
	}

	void SetEmployee(const char* FIO_, const short int departmentNumber_, const char* place_, const char* birthDate) {
		String out;

		cp866_to_utf8(out, FIO_, sizeof(Employee::FIO));
		tFIO.setText(placeholder[0] + out);

		tDepartmentNumber.setText(placeholder[1] + std::to_string(departmentNumber_));

		cp866_to_utf8(out, place_, sizeof(Employee::place));
		tPlace.setText(placeholder[2] + out);

		tBirthDate.setText(placeholder[3] + birthDate);
	}

	void setPosition(Vector2f pos_) {
		background.setPosition(pos_);

		Vector2f top = background.getPosition() + Vector2f(0, -background.getLocalBounds().height);

		tFIO.setPosition(top + Vector2f(10, 10));
		tDepartmentNumber.setPosition(top + Vector2f(10, 50));
		tPlace.setPosition(top + Vector2f(10, 90));
		tBirthDate.setPosition(top + Vector2f(10, 130));

		close.setPosition(background.getPosition() + Vector2f(background.getLocalBounds().width, -background.getLocalBounds().height) + Vector2f(-3.5f, 3.5f));
	}

	void Draw(RenderWindow& window) {
		window.draw(background);
		window.draw(tFIO.getText());
		window.draw(tDepartmentNumber.getText());
		window.draw(tPlace.getText());
		window.draw(tBirthDate.getText());
		window.draw(close.getButton());
		//window.draw(close.getText());
	}
};
