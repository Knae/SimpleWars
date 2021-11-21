#pragma once
#ifndef  __CINFOTEXTDISPLAY_H__
#define __CINFOTEXTDISPLAY_H__


#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
class CInfoTextDisplay
{
public:
	CInfoTextDisplay();
	~CInfoTextDisplay();

	void Initialize(float _inTopLeftPosX, float _inTopLeftPosY, sf::Font* _inFont,unsigned int _inNumberOfText);
	bool SetTextAtIndex(int _inIndex, std::string _inabel, std::string _inValue);
	bool SetValueDisplayAtIndex(int _inIndex, std::string _inValue);
	void DisplayInfo(sf::RenderWindow& _inWindow);
	void Clear();
private:
	sf::Vector2f m_TopLeftPosition;

	std::vector<sf::Text*> m_vecLabels;
	std::vector<sf::Text*> m_vecValues;

	const unsigned int m_fFontSize = 12;
	const sf::Color m_fFontColor = sf::Color::White;
};

#endif // ! __CINFOTEXTDISPLAY_H__