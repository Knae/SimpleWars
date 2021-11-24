#pragma once
#ifndef __CUIMANAGER_H__
#define __CUIMANAGER_H__

#include<SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "CButton.h"
#include "CInfoTextDisplay.h"
#include "CUIEnums.h"
#include "CUnitEnums.h"
#include "CSceneEnums.h"
#include "CParseConfigCommon.h"
/// <summary>
/// Manager class that is responsible for creating and managing
/// the UI
/// </summary>

class CTerrainEffects;
class CUnit;

class CUIManager
{
public:
	~CUIManager();
	static CUIManager& GetRef() { static CUIManager m_UIMgr; return m_UIMgr; }

	static bool IntializeUI(sf::Vector2u _inWindowSize, sf::Font* _inFont, const unsigned int _inSceneWidth = 832);
	static void UpdateUI();
	static void DisplayUI(sf::RenderWindow& _inWindow);
	static void ClearUIElements();
	static bool ProcessClick(sf::Vector2f& _inCoords, int& _outIndex);
	static int	ProcessClickInCtrlPanel(sf::Vector2f& _inCoords);
	static bool GetCButtonClicked(sf::Vector2f _inPosition, int& _outButtonIndex);
	static void SetUpModeSelectionPanel();
	static void SetUpUnitPlacementPanel(int* _inAmountA, int* _inAmountB, int* _inAmountC);
	static void SetUpGameLoopPanel();
	static bool UpdateInfoDisplay(
			CUnit* _inSelectedUnit = nullptr,
			CUnit* _inViewedUnit = nullptr,
			CTerrainEffects* _inSelectedUnitTerrain = nullptr,
			CTerrainEffects* _inViewedUnitTerrain = nullptr
		);
		//CUnitEnums::SIDE _inSide = CUnitEnums::SIDE::NONE,
		//CUnitEnums::FACTION _inFaction = CUnitEnums::FACTION::NONE);
	static bool GetIfTurnEndClicked()								{ return m_bEndTurn; }
	static bool GetIfForfeitChosen()								{ return m_bForfeitChosen; }
	static void VictoryAchieved(CUIEnums::TURN _inSide);
	/// <summary>
	/// Set the current state the UI manager is at
	/// DOES NOT CHANGE THE UI. Changing the UI setup needs
	/// to be done manually because of the different informatioin
	/// required depending on the state.
	/// </summary>
	/// <param name="_inState"></param>
	static void ResetChecks()										
	{
		m_eCurrentTypeChosen = CUnitEnums::TYPE::NONE;
		m_bForfeitChosen = false; 
		m_bDisplayVictory = false;
	}
	static void SetCurrentGameState(CUIEnums::GAMESTATE _inState)	{ m_eCurrentUIState = _inState; }
	static void SetCurrentMouseState(CUIEnums::MOUSESTATE _inState) { m_eCurrentMouseState = _inState; }
	static void SetCurrentTurn(CUIEnums::TURN _inTurn)				{ m_eCurrentTurn = _inTurn; m_bEndTurn = false; }
	static void SetChosenUnitToNone()								{ m_eCurrentTypeChosen = CUnitEnums::TYPE::NONE; }
	static CUnitEnums::TYPE GetChosenUnit()							{ return m_eCurrentTypeChosen; }
	static CUIEnums::MOUSESTATE GetMouseCurrentState()				{ return m_eCurrentMouseState; }
	static void SetCurrentUnitHasNoMovePoints(bool _inHasNoPoints)	{ m_bUnitHasNoMovePoints = _inHasNoPoints; }
	static void SetCurrentUnitHasAttacked(bool _inHasAttacked)		{ m_bUnitHasAttacked = _inHasAttacked; }
	static void SwitchTurnForUnitPlacment(int* _inAmountA, int* _inAmountB, int* _inAmountC);

private:
	CUIManager();
	CUIManager(CUIManager const&) = delete;
	void operator=(CUIManager const&) = delete;

	static void ChangeState(CUIEnums::GAMESTATE _input)				{ m_eCurrentUIState = _input; }

	static const std::string m_strUnitButtonSpriteMap;
	static const std::string m_strEmptyUnitSprite;
	static const std::string m_strEmblemSpriteMap;
	static const std::string m_strGameButtonsSpriteMap;
	static const std::string m_strTileSelectorSpriteMap;
	static const std::string m_strFinishButtonSprite;
	static const std::string m_strVictorySprite;
	static const sf::IntRect m_ButtonUnitRect_Blue;
	static const sf::IntRect m_ButtonUnitRect_Red;
	static const sf::IntRect m_ButtonGameLoop;
	static const sf::IntRect m_ButtonFactionEmblem;

	static std::vector<CButton*> m_vecCustomButtons;
	static std::vector<sf::Sprite*> m_vecButtons_ControlPanel;
	static std::vector<sf::Sprite*> m_vecOverlays;
	static std::vector<sf::Text*> m_vecText_ControlPanel;
	static std::vector<int*> m_vecText_DisplayVariables;
	static sf::RenderTexture* m_pPanelBackground;
	static sf::Texture* m_pEmblemTexture;
	static sf::Texture* m_pButtonUnitTexture;
	static sf::Texture* m_pEmptyUnitPortrait;
	static sf::Texture* m_pButtonsGameLoop;
	static sf::Texture* m_pButtonFinish;
	static sf::Sprite* m_pSpriteBackground;
	static sf::Font* m_pFont;
	static sf::Texture m_VictoryTexture;
	static sf::Sprite m_VictorySprite;
	static sf::IntRect m_VictoryRect_Blue;
	static sf::IntRect m_VictoryRect_Red;
	static unsigned int m_uSceneWidth;
	static bool m_bUnitControllable;
	static bool m_bUnitHasNoMovePoints;
	static bool m_bUnitHasAttacked;
	static bool m_bEndTurn;
	static bool m_bForfeitChosen;
	static bool m_bDisplayVictory;
	static bool m_bDisplayInfoText;

	static const CUnitEnums::TYPE m_eUnitOnButton[4];

	static CUIEnums::TURN m_eCurrentTurn;
	static CUIEnums::GAMESTATE m_eCurrentUIState;
	static CUIEnums::MOUSESTATE m_eCurrentMouseState;
	static CUnitEnums::TYPE m_eCurrentTypeChosen;
	static CUnitEnums::SIDE m_eCurrentUnitSide;
	static CInfoTextDisplay m_Info_FactionBonus;
	static CInfoTextDisplay m_Info_UnitStats;
	static CInfoTextDisplay m_Info_OccupiedTerrain;
	static CInfoTextDisplay m_Info_ViewedTerrain;
};

#endif // !__CUIMANAGER_H__