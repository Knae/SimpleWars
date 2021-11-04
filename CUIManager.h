#pragma once
#ifndef __CUIMANAGER_H__
#define __CUIMANAGER_H__

#include<SFML/Graphics.hpp>
#include <string>
#include "UIEnums.h"
#include "CUnitEnums.h"
/// <summary>
/// Manager class that is responsible for creating and managing
/// the UI
/// </summary>
class CUIManager
{
public:
	~CUIManager();
	static CUIManager& GetRef() { static CUIManager m_UIMgr; return m_UIMgr; }

	static bool IntializeUI(sf::Vector2u _inWindowSize, const unsigned int _inSceneWidth = 832);
	static void DisplayUI(sf::RenderWindow& _inWindow);
	static bool ProcessClick(sf::Vector2f& _inCoords);
	static int ProcessClickInCtrlPanel(sf::Vector2f& _inCoords);
	static void SetUpUnitPlacementPanel();
	static void SetCurrentGameState(UIEnums::GAMESTATE _inState) { m_eCurrentUIState = _inState; }
	static void SetCurrentTurn(UIEnums::TURN _inTurn) { m_eCurrentTurn = _inTurn; }
	static void SetChosenUnitToNone() { m_eCurrentUnitChosen = CUnitEnums::TYPE::NONE; }
	static CUnitEnums::TYPE GetChosenUnitToNone() { return m_eCurrentUnitChosen; }
	static UIEnums::MOUSESTATE GetCurrentState() {return m_eCurrentMouseState;}

private:
	CUIManager();
	CUIManager(CUIManager const&) = delete;
	void operator=(CUIManager const&) = delete;

	static void ChangeMouseState();

	static const std::string m_strUnitButtonSpriteMap;// = "assets/spritemaps/UnitButtons.png";
	static const std::string m_strEmblemSpriteMap;// = "assets/spritemaps/FactionEmblems.png";
	static const std::string m_strGameButtonsSpriteMap;// = "assets/spritemaps/GameButtons.png";
	static const std::string m_strTileSelectorSpriteMap;// = "assets/spritemaps/tileSelection.png";
	static const sf::IntRect m_ButtonUnitRect_Blue;// = { 0, 0, 32, 32 };
	static const sf::IntRect m_ButtonUnitRect_Red;// = { 0, 32, 32, 32 };
	//const sf::IntRect m_TileSpriteStart = { 96, 0, 32, 32 };
	//const sf::IntRect m_TileSpriteFinish = { 128, 0, 32, 32 };
	//const sf::IntRect m_TileSpriteStartAlg = { 0, 0, 160, 32 };
	//const sf::IntRect m_TileSpriteReset = { 160, 0, 160, 32 };

	static std::vector<sf::Sprite*> m_vecButtons_UnitPlacementPanel;
	static std::vector<sf::Text*> m_vecText_UnitPlacementPanel;
	static sf::Texture* m_ButtonUnitTexture;
	static sf::Font* m_pFont;
	static sf::RenderTexture* m_pSceneBackground;
	static sf::Sprite* m_pSpriteBackground;
	static unsigned int m_uSceneWidth;

	static const CUnitEnums::TYPE m_UnitOnButton[3];

	static UIEnums::TURN m_eCurrentTurn;
	static UIEnums::GAMESTATE m_eCurrentUIState;
	static UIEnums::MOUSESTATE m_eCurrentMouseState;
	static CUnitEnums::TYPE m_eCurrentUnitChosen;
};

#endif // !__CUIMANAGER_H__