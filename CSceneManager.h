#pragma once
#ifndef __CSCENEMANAGER_H__
#define __CSCENEMANAGER_H__

#include <SFML/Window.hpp>
#include "CScene.h"

/// <summary>
/// Manager class responsible for creating and destroying the
/// game map
/// </summary>

class CSceneManager
{
public:
	static CSceneManager& GetRef() { return m_SceneMgr; }
	virtual ~CSceneManager();

	static bool CreateScene(CSceneEnums::SCENETYPE& _inputType,const std::string& _inputConfigPath);
	static void DisplayScene(sf::RenderWindow& _targetWindow);
	static CScene* GetCurrentScene()						{ return m_pCurrentManagedScene; }
	static CTile* GetTileInScene(sf::Vector2f _inPosition)	{ return m_pCurrentManagedScene->GetTile(_inPosition); }
	static unsigned int GetTileSize()						{ return m_pCurrentManagedScene->GetTileWidth(); }

private:
	CSceneManager();
	CSceneManager(CSceneManager const&) = delete;
	void operator=(CSceneManager const&) = delete;

	static CSceneManager m_SceneMgr;
	static CScene* m_pCurrentManagedScene;
	static sf::RenderTexture* m_pSceneBackground;
	static sf::Sprite* m_pSpriteBackground;
};

#endif // !__CSCENEMANAGER_H__