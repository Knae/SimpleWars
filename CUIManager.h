#pragma once
#ifndef __CUIMANAGER_H__
#define __CUIMANAGER_H__

#include<SFML/Graphics.hpp>
#include <string>
#include "UIEnums.h"
#include "CUnitEnums.h"
#include "CSceneEnums.h"
#include "CParseConfigCommon.h"
/// <summary>
/// Manager class that is responsible for creating and managing
/// the UI
/// </summary>
class CUIManager
{
public:
	~CUIManager();
	static CUIManager& GetRef() { static CUIManager m_UIMgr; return m_UIMgr; }

	static bool IntializeUI(sf::Vector2u _inWindowSize, sf::Font* _inFont, const unsigned int _inSceneWidth = 832);
	static void UpdateUI();
	static void DisplayUI(sf::RenderWindow& _inWindow);
	static void ClearUIElements();
	static bool ProcessClick(sf::Vector2f& _inCoords);
	static int	ProcessClickInCtrlPanel(sf::Vector2f& _inCoords);
	static void SetUpUnitPlacementPanel(int* _inAmountA, int* _inAmountB, int* _inAmountC);
	static void SetUpGameLoopPanel();
	static bool UpdateInfoDisplay(
		CSceneEnums::TILETYPE _inTerrain,
		CUnitEnums::SIDE _inSide = CUnitEnums::SIDE::NONE,
		CUnitEnums::TYPE _inType = CUnitEnums::TYPE::NONE,
		CUnitEnums::FACTION _inFaction = CUnitEnums::FACTION::NONE);
	static bool GetIfTurnEndClicked()								{ return m_bEndTurn; }
	/// <summary>
	/// Set the current state the UI manager is at
	/// DOES NOT CHANGE THE UI. Changing the UI setup needs
	/// to be done manually because of the different informatioin
	/// required depending on the state.
	/// </summary>
	/// <param name="_inState"></param>
	static void SetCurrentGameState(UIEnums::GAMESTATE _inState)	{ m_eCurrentUIState = _inState; }
	static void SetCurrentTurn(UIEnums::TURN _inTurn)				{ m_eCurrentTurn = _inTurn; m_bEndTurn = false; }
	static void SetChosenUnitToNone()								{ m_eCurrentTypeChosen = CUnitEnums::TYPE::NONE; }
	static CUnitEnums::TYPE GetChosenUnit()							{ return m_eCurrentTypeChosen; }
	static UIEnums::MOUSESTATE GetCurrentState()					{ return m_eCurrentMouseState; }
	static void SwitchTurnForUnitPlacment(int* _inAmountA, int* _inAmountB, int* _inAmountC);

private:
	CUIManager();
	CUIManager(CUIManager const&) = delete;
	void operator=(CUIManager const&) = delete;

	static void ChangeState(UIEnums::GAMESTATE _input) { m_eCurrentUIState = _input; }

	static const std::string m_strUnitButtonSpriteMap;
	static const std::string m_strEmptyUnitSprite;
	static const std::string m_strEmblemSpriteMap;
	static const std::string m_strGameButtonsSpriteMap;
	static const std::string m_strTileSelectorSpriteMap;
	static const std::string m_strFinishButtonSprite;
	static const sf::IntRect m_ButtonUnitRect_Blue;
	static const sf::IntRect m_ButtonUnitRect_Red;
	static const sf::IntRect m_ButtonGameLoop;
	//const sf::IntRect m_TileSpriteStart = { 96, 0, 32, 32 };
	//const sf::IntRect m_TileSpriteFinish = { 128, 0, 32, 32 };
	//const sf::IntRect m_TileSpriteStartAlg = { 0, 0, 160, 32 };
	//const sf::IntRect m_TileSpriteReset = { 160, 0, 160, 32 };

	static std::vector<sf::Sprite*> m_vecButtons_ControlPanel;
	static std::vector<sf::Sprite*> m_vecOverlays;
	static std::vector<sf::Text*> m_vecText_UnitPlacementPanel;
	static std::vector<int*> m_vecText_DisplayVariables;
	static sf::Texture* m_ButtonUnitTexture;
	static sf::Texture* m_EmptyUnitPortrait;
	static sf::Texture* m_ButtonsGameLoop;
	static sf::Texture* m_ButtonFinish;
	static sf::Font* m_pFont;
	static sf::RenderTexture* m_pPanelBackground;
	static sf::Sprite* m_pSpriteBackground;
	static unsigned int m_uSceneWidth;
	static bool m_bUnitControllable;
	static bool m_bEndTurn;
	//static bool m_bToggleRange;

	static const CUnitEnums::TYPE m_UnitOnButton[4];

	static UIEnums::TURN m_eCurrentTurn;
	static UIEnums::GAMESTATE m_eCurrentUIState;
	static UIEnums::MOUSESTATE m_eCurrentMouseState;
	static CUnitEnums::TYPE m_eCurrentTypeChosen;
	static CUnitEnums::SIDE m_eCurrentUnitSide;
};

#endif // !__CUIMANAGER_H__