#pragma once
#ifndef __CINFOTEXTDISPLAY_H__
#define __CINFOTEXTDISPLAY_H__

#include <SFML/Graphics.hpp>
#include <vector>

class CDebug
{
public:
	static CDebug& GetRef() { static CDebug m_refThis; return m_refThis; }
	~CDebug();

	void Initiallize(sf::Font* _inFont);


private:
	CDebug();

	std::string m_strUnitButtonSpritemap = "assets/spritemaps/UnitButtons.png";
	std::string m_strEmptyUnitSpritemap = "assets/spritemaps/EmptyUnit.png";
	sf::IntRect m_ButtonUnitRect_Blue = sf::IntRect( 0, 0, 32, 32 );

	sf::Texture* m_EmptyPortrait;
	sf::Sprite* m_UnitPortrait;
	sf::Sprite* m_InfoBackground;
	sf::Texture* m_UnitTexture;
};

#endif // ! __CINFOTEXTDISPLAY_H__