#pragma once
#ifndef __CGAMEMANAGER_H__
#define __CGAMEMANAGER_H__

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "CUIManager.h"
#include "CSceneManager.h"

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
	virtual void SetPointersToOtherSystems(CUIManager* _inputUI, CSceneManager* _inputScene);
	virtual void DrawObject(sf::Drawable* _object);
	virtual void DisplayGameWorld();
	virtual void DestroyGameWorld();
	virtual void DisplayDebug();
	//====================================
	//Scene management related
	//====================================
	virtual bool LoadScene();
	virtual void DisplayScene();

	virtual sf::RenderWindow* GetGameWindow() { return m_pGameWindow; }

private:
	CGameManager();
	CGameManager(CGameManager const&) = delete;
	void operator=(CGameManager const&) = delete;

	const sf::Vector2u m_kv2uGameWindowSize_Default = sf::Vector2u(1024, 576);
	//const static std::string m_strMountainVillageConfig;
	const std::string m_strMountainVillageConfig= "configs/maps/MountainVillage.ini";
	
	CUIManager* m_pUIMgr;
	CSceneManager* m_pSceneMgr;

	sf::RenderWindow* m_pGameWindow;
	sf::RenderTexture* m_pGameBackground;
	sf::Sprite* m_pSpriteBackground;
	sf::Font* m_pFont;
	sf::Vector2u m_v2uGameWindowSize_Current;
};

#endif // !__CGAMEMANAGER_H__