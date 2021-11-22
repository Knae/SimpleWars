#pragma once
#ifndef __CGAMEMANAGER_H__
#define __CGAMEMANAGER_H__

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <iomanip>
#include "CUIManager.h"
#include "CSceneManager.h"
#include "CDebug.h"
#include "CUnitManager.h"
#include "COverlayManager.h"
#include "CParseConfigCommon.h"

/// <summary>
/// This will be the main class accessed by main
/// to manipulate game elements.
/// </summary>
class CGameManager
{
public:
	static CGameManager& GetRef()				{ static CGameManager m_gmThis; return m_gmThis; }
	virtual sf::RenderWindow* GetGameWindow()	{ return m_pGameWindow; }
	virtual ~CGameManager();
	//====================================
	//General functions
	//====================================
	virtual bool IntializeGame();
	virtual bool UpdateManagers(double& _inElapsedTime);
	virtual bool ChangeCurrentState(CUIEnums::GAMESTATE _inState);
	virtual void SwitchTurns();
	virtual void SetPointersToOtherSystems(
							CUIManager* _inputUI,
							CSceneManager* _inputScene,
							CUnitManager* _inputUnit,
							COverlayManager* _inputOverlay
						);
	virtual void DrawObject(sf::Drawable* _object);
	virtual void DisplayGameWorld();
	virtual void DestroyGameWorld();
	virtual void DisplayDebug();
	virtual void ClearUnitsToPlace();
	virtual CUnit* GetCurrentSelectedUnit() { return m_pSelectedUnit; }
	//====================================
	//Scene management related
	//====================================
	virtual bool LoadScene();
	virtual void DisplayScene();
	//====================================
	//UI management related
	//====================================
	virtual bool InitializeUI();
	virtual void ProcessMouseClick();
	virtual void SetUIToUnitPlacement();
	virtual void SetUIToGameLoop();
	virtual void DisplayUI();
	virtual void UpdateSidePanelInfo(	CUnit* _inViewedUnit = nullptr);
	//====================================
	//UI management related
	//====================================
	virtual void UpdateDebugWorld();

private:
	CGameManager();
	CGameManager(CGameManager const&) = delete;
	void operator=(CGameManager const&) = delete;

	bool CheckIfMouseOverTile(sf::Vector2f _inPosition);
	void ProcessUnitAsDead(CUnit* _inUnit);

	const sf::Vector2u m_GameWindowSize_Default = sf::Vector2u(1024, 576);
	std::string m_strMountainVillageConfig= "configs/maps/MountainVillage.ini";
	std::string m_strUnitConfig = "configs/units/BaseSettings.ini";
	std::string m_strFactionConfig = "configs/factions.ini";
	
	CUIManager* m_pUIMgr;
	CSceneManager* m_pSceneMgr;
	CUnitManager* m_pUnitMgr;
	COverlayManager* m_pOverlayMgr;
	CDebug&  m_refDebug = CDebug::GetRef();


	sf::RenderWindow* m_pGameWindow;
	sf::RenderTexture* m_pGameBackground;
	sf::Sprite* m_pSpriteBackground;
	sf::Font* m_pFont;
	sf::Vector2u m_GameWindowSize_Current;
	bool m_bExecutingActions;
	bool m_bAttackOverlayShown;
	bool m_bWaitingForClick;

	CUIEnums::TURN m_eCurrentTurn;
	CUIEnums::GAMESTATE m_eCurrentState;
	CUIEnums::MOUSESTATE m_eCurrentUIMouseState;
	CUnitEnums::TYPE m_eCurrentTypeChosen;
	CUnit* m_pSelectedUnit;

	//track number of units to place
	std::map<CUnitEnums::TYPE, int> m_mapUnitsToPlaced_B;
	std::map<CUnitEnums::TYPE, int> m_mapUnitsToPlaced_R;
	std::map<CUnitEnums::TYPE, int>* m_pUnitsToPlace;
};

#endif // !__CGAMEMANAGER_H__