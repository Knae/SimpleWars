#pragma once
#ifndef __CGAMEMANAGER_H__
#define __CGAMEMANAGER_H__

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "CUIManager.h"
#include "CSceneManager.h"
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
	static CGameManager& GetRef() { static CGameManager m_gmThis; return m_gmThis; }
	virtual ~CGameManager();
	//====================================
	//General functions
	//====================================
	virtual bool IntializeGame();
	virtual bool UpdateManagers(double& _inElapsedTime);
	virtual bool ChangeCurrentState(UIEnums::GAMESTATE _inState);
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

	virtual sf::RenderWindow* GetGameWindow() { return m_pGameWindow; }

private:
	CGameManager();
	CGameManager(CGameManager const&) = delete;
	void operator=(CGameManager const&) = delete;

	const sf::Vector2u m_kv2uGameWindowSize_Default = sf::Vector2u(1024, 576);
	//const static std::string m_strMountainVillageConfig;
	const std::string m_strMountainVillageConfig= "configs/maps/MountainVillage.ini";
	const std::string m_strUnitConfig = "configs/units.ini";
	const std::string m_strFactionConfig = "configs/factions.ini";
	
	CUIManager* m_pUIMgr;
	CSceneManager* m_pSceneMgr;
	CUnitManager* m_pUnitMgr;
	COverlayManager* m_pOverlayMgr;

	sf::RenderWindow* m_pGameWindow;
	sf::RenderTexture* m_pGameBackground;
	sf::Sprite* m_pSpriteBackground;
	sf::Font* m_pFont;
	sf::Vector2u m_v2uGameWindowSize_Current;
	bool m_bAttackOverlayShown;

	UIEnums::TURN m_eCurrentTurn;
	UIEnums::GAMESTATE m_eCurrentState;
	UIEnums::MOUSESTATE m_eCurrentUIMouseState;
	CUnitEnums::TYPE m_eCurrentTypeChosen;
	CUnit* m_SelectedUnit;

	//track number of units to place
	std::map<CUnitEnums::TYPE, int> m_iUnitsToPlaced_B;
	std::map<CUnitEnums::TYPE, int> m_iUnitsToPlaced_R;
	std::map<CUnitEnums::TYPE, int>* m_pUnitsToPlace;
};

#endif // !__CGAMEMANAGER_H__