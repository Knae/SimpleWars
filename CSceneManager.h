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
	static CScene* GetCurrentScene() { return m_pCurrentManagedScene; }


private:
	CSceneManager();
	CSceneManager(CSceneManager const&) = delete;
	void operator=(CSceneManager const&) = delete;

	static CSceneManager m_SceneMgr;
	static CScene* m_pCurrentManagedScene;
};

#endif // !__CSCENEMANAGER_H__