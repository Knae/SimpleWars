#pragma once
#ifndef __CGAMEMANAGER_H__
#define __CGAMEMANAGER_H__

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "CUIManager.h"
#include "CSceneManager.h"
#include "CDebug.h"
#include "CUnitManager.h"
#include "COverlayManager.h"
#include "CVFXManager.h"
#include "CAI_Controller.h"
#include "CParseConfigCommon.h"

/// <summary>
/// This will be the main class accessed by main
/// to manipulate game elements.
/// </summary>
class CGameManager
{
public:
	static CGameManager& GetRef() { static CGameManager m_gmThis; return m_gmThis; }
	virtual sf::RenderWindow* GetGameWindow() { return m_pGameWindow; }
	virtual ~CGameManager();
	//====================================
	//General functions
	//====================================
	virtual bool IntializeGame();
	virtual bool Update(double& _inElapsedTime);
	virtual bool ChangeCurrentState(CUIEnums::GAMESTATE _inState);
	virtual void SwitchTurns();
	virtual void EndGame(CUIEnums::TURN _inWinSide);
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
	//Unit functions
	//====================================
	virtual void ClearUnitsToPlace();
	virtual CUnit* GetCurrentSelectedUnit() { return m_pSelectedUnit; }
	//====================================
	//Scene management related
	//====================================
	virtual bool LoadScene(CSceneEnums::SCENETYPE _inScene);
	virtual void DisplayScene();
	//====================================
	//UI management related
	//====================================
	virtual bool InitializeUI();
	virtual void ProcessMouseClick();
	virtual void SetUIToModeSelection();
	virtual void SetUIToFactionSelection();
	virtual void SetUIToMapSelection();
	virtual void SetUIToUnitPlacement();
	virtual void SetUIToGameLoop();
	virtual void DisplayUI();
	virtual void UpdateSidePanelInfo(CUnit* _inViewedUnit = nullptr);
	//====================================
	//Debug related
	//====================================
	virtual void UpdateDebugWorld();

private:
	CGameManager();
	CGameManager(CGameManager const&) = delete;
	void operator=(CGameManager const&) = delete;

	void OnMouseClick_SidePanel(sf::Vector2f& _inMousePosition);
	void OnMouseClick_Map(sf::Vector2f& _inMousePosition);

	bool CheckIfMouseOverTile(sf::Vector2f _inPosition);

	void ProcessUnitAttack(CUnit* _inAttacker, CUnit* _Defender);
	void ProcessUnitAsDead(CUnit* _inUnit);
	void ParseGameSettings();
	void ParseGameStats();
	void RecordGameStats();


	const sf::Vector2u m_GameWindowSize_Default = sf::Vector2u(1024, 576);
	const std::string m_strGameConfig = "configs/game.ini";
	std::string m_strFontFile = "font/OpenSans-Regular.ttf";
	std::string m_strGameStatsConfig = "configs/stats.ini";
	std::string m_strVFXSettings = "configs/vfx.ini";
	std::string m_strMainMenuConfig = "configs/maps/MainMenu.ini";
	std::string m_strMountainVillageConfig = "configs/maps/MountainVillage.ini";
	std::string m_strMountainPassConfig = "configs/maps/MountainPass.ini";
	std::string m_strBridgeConfig = "configs/maps/Bridge.ini";
	std::string m_strUnitConfig = "configs/units/BaseSettings.ini";
	std::string m_strFactionConfig = "configs/factions.ini";

	CUIManager* m_pUIMgr;
	CSceneManager* m_pSceneMgr;
	CUnitManager* m_pUnitMgr;
	COverlayManager* m_pOverlayMgr;
	CVFXManager* m_pVFXMgr;
	CDebug& m_refDebug = CDebug::GetRef();

	sf::RenderWindow* m_pGameWindow;
	sf::RenderTexture* m_pGameBackground;
	sf::Sprite* m_pSpriteBackground;
	sf::Font* m_pFont;
	sf::Vector2u m_GameWindowSize_Current;
	sf::Vector2u m_SpawnAreaTopLeft;
	unsigned int m_uiSpawnAreaWidth;
	unsigned int m_uiSpawnAreaHeight;
	unsigned int m_uiWindowFrameLimit = 60;
	unsigned int m_uiWins_Red = 0;
	unsigned int m_uiWins_Blue = 0;
	bool m_bExecutingActions;
	bool m_bAttackOverlayShown;
	bool m_bWaitingForClick;
	bool m_bAIEnabled;
	bool m_bUnitActionInProgress = false;

	CSceneEnums::SCENETYPE m_eCurrentSelectedMap = CSceneEnums::SCENETYPE::MOUNTAINVILLAGE;
	CUIEnums::TURN m_eCurrentTurn;
	CUIEnums::GAMESTATE m_eCurrentState;
	//CUIEnums::MOUSESTATE m_eCurrentUIMouseState;
	CUnitEnums::TYPE m_eCurrentTypeChosen;
	CUnit* m_pSelectedUnit;

	CUnitEnums::FACTION m_eChosenFaction_Blue;
	CUnitEnums::FACTION m_eChosenFaction_Red;

	//track number of units to place
	std::map<CUnitEnums::TYPE, int> m_mapUnitsToPlaced_B;
	std::map<CUnitEnums::TYPE, int> m_mapUnitsToPlaced_R;
	std::map<CUnitEnums::TYPE, int>* m_pUnitsToPlace;
};

#endif // !__CGAMEMANAGER_H__