#pragma once
#ifndef __CDEBUG__H__
#define __CDEBUG__H__

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "CInfoTextDisplay.h"

class CUnit;
class CDebug
{
public:
	static CDebug& GetRef() { static CDebug m_refThis; return m_refThis; }
	~CDebug();

	bool Initialize(sf::Font* _inFont);
	void Update();
	void DisplayWindow();
	sf::RenderWindow* GetWindows() { return m_pDebugWindow; }

	CUnit* GetUnitToDestoy();
	void UpdateInfoDisplay(CUnit* _inUnit);

private:
	CDebug();
	CDebug(CDebug const&) = delete;
	void operator=(CDebug const&) = delete;

	int isButtonClicked(sf::Vector2f& _input);

	std::string m_strUnitButtonSpritemap = "assets/spritemaps/UnitButtons.png";
	std::string m_strEmptyUnitSpritemap = "assets/spritemaps/EmptyUnit.png";
	std::string m_strDebugButtonSpriteMap = "assets/spritemaps/DebugButtons.png";
	std::string m_strDebugKill = "assets/spritemaps/SkelIcon.png";

	const sf::IntRect m_PortraitIntRect_Blue = sf::IntRect( 0, 0, 32, 32 );
	const sf::IntRect m_PortraitIntRect_Red = sf::IntRect(0, 32, 32, 32);
	const sf::IntRect m_ButtonPlusIntRect = sf::IntRect(0, 0, 16, 16);
	const sf::IntRect m_ButtonMinusIntRect = sf::IntRect(0, 16, 16, 16);

	sf::RenderWindow* m_pDebugWindow;
	sf::Texture* m_pEmptyPortrait;
	sf::Texture* m_pUnitTexture;
	sf::Texture* m_pDebugButtonTex;
	sf::Texture* m_pDebugKillTex;
	sf::Sprite* m_pUnitPortrait;
	sf::Sprite* m_pInfoBackground;
	sf::Text m_HP;
	sf::Text m_Movements;
	sf::Text m_Damage;
	sf::Text m_Range;
	CInfoTextDisplay m_UnitInfo;
	CUnit* m_pCurrentUnit;
	CUnit* m_pUnitToDestroy;

	std::vector<sf::Sprite*> m_vecButtons;

	const unsigned int m_uiFontSize = 16;
	const sf::Color m_FontColor = sf::Color::White;
};

#endif // ! __CDEBUG__H__