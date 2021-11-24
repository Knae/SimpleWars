#pragma once
#ifndef __CBUTTON_H__
#define __CBUTTON_H__

#include <SFML/Graphics.hpp>

class CButton
{
public:
	CButton();
	~CButton();

	void Initialize(std::string _inName, sf::Font* _inFont, std::string _inText, sf::Vector2f _inPosition, unsigned int in_TileWidth);
	void Display(sf::RenderWindow& _inWindow);
	bool CheckIfClicked(sf::Vector2f _inCoordinates);
	std::string& GetButtonName() { return m_strButtonName; }
private:
	sf::Text m_ButtonText;
	sf::RectangleShape m_ButtonBorder;
	std::string m_strButtonName;

	const unsigned int m_uiFontSize = 24;
	const float m_fOutlineThickness = 8;
	const sf::Color m_FontColor = sf::Color::White;
	const sf::Vector2f m_ButtonDimensions = sf::Vector2f(128.0f,64.0f);
};

#endif // !__CBUTTON_H__