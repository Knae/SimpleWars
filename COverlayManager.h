#pragma once
#ifndef __COVERLAYMANAGER_H__
#define __COVERLAYMANAGER_H__

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "CParseConfigCommon.h"

class COverlayManager
{
public:
	~COverlayManager();
	static COverlayManager& GetRef() { static COverlayManager m_OverlayMgr; return m_OverlayMgr; }

	static void InitializeOverlays(const std::string& _inConfigPath, int _inTileSize);
	static void DisplayOverlays(sf::RenderWindow& _inWindow);
	static void ClearTileOverlays();
	static void CreateRangeOverlay(sf::Vector2i& _tilePosition, int _inRange);

private:
	COverlayManager();

	static std::vector<sf::Sprite*> m_vecOverlayTileSelector;

	static std::string m_strTileSelector;
	static std::map<std::string*, sf::IntRect*> m_mapSpriteRect;
	static sf::Texture* m_pOverlayTexture;
};

#endif // !__COVERLAYMANAGER_H__