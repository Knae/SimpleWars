#pragma once
#ifndef __CBUTTON_H__
#define __CBUTTON_H__

#include <SFML/Graphics.hpp>

class CButton
{
public:
	CButton();
	~CButton();

	void Display();
	void CheckIfClicked(sf::Vector2f _inCoordinates);
private:
	sf::Text m_ButtonText;
	sf::RectangleShape m_ButtonBorder;
};

#endif // !__CBUTTON_H__
