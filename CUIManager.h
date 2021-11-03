#pragma once
#ifndef __CUIMANAGER_H__
#define __CUIMANAGER_H__

#include<SFML/Graphics.hpp>
/// <summary>
/// Manager class that is responsible for creating and managing
/// the UI
/// </summary>
class CUIManager
{
public:
	~CUIManager();
	static CUIManager& GetRef() { static CUIManager m_UIMgr; return m_UIMgr; }

	static void IntializeUI();
	static void DisplayUI(sf::Window& _inWindow);
	static bool ProcessClick(sf::Vector2f& _inCoords);
private:
	CUIManager();
	CUIManager(CUIManager const&) = delete;
	void operator=(CUIManager const&) = delete;

	static void ChangeMouseState();

	static sf::Font* m_pFont;
};

#endif // !__CUIMANAGER_H__