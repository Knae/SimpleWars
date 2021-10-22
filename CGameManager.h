#pragma once
#ifndef __CGAMEMANAGER_H__
#define __CGAMEMANAGER_H__

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "CUIManager.h"
class CGameManager
{
public:
	CGameManager();
	virtual ~CGameManager();

	virtual bool IntializeGame();
	virtual void SetPointersToOtherSystems(CUIManager* _inputUI);
	//void MoveToScene();
	virtual void DrawObject(sf::Drawable* _object);
	virtual void DisplayGameWorld();
	virtual void DestroyGameWorld();
	virtual void DisplayDebug();
private:
	const sf::Vector2u m_kv2uGameWindowSize_Default = sf::Vector2u(1024, 576);

	CUIManager* m_ptrUIManager;

	sf::RenderWindow* m_ptrGameWindow;
	sf::Font* m_ptrFont;
	sf::Sprite* m_ptrSpriteBackground;

	sf::Vector2u m_v2uGameWindowSize_Current;
};

#endif // !__CGAMEMANAGER_H__