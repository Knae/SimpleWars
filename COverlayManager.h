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

	static void InitializeOverlays(const std::string& _inConfigPath, sf::Font* _inFont, int _inTileSize = 32);
	static void Update(sf::Vector2f& _inMouse, double _elapsedTime=0.0f);
	static void DisplayOverlays(sf::RenderWindow& _inWindow );
	static void ClearTileOverlays();

	static void CreateUnitSelectorOverlays();
	static void CreateUnitPlacementOverlay(	sf::Vector2u& _tileTopLeft,
											unsigned int& _inAreaWidth,
											unsigned int& _inAreaHeight);
	static void CreateUnitOverlay(sf::Vector2u& _tilePosition, int _inRange);
	static void ClearRangePlacementOverlay();
	static void ShowUnitSelector(sf::Vector2u& _inTileLocation);
	static void ShowMoveSelector(sf::Vector2f& _mouseLocation);
	static void ShowAttackSelector(sf::Vector2f& _mouseLocation);
	static void UpdateMoveMod(sf::Vector2f& _inMousePosition, float _inMoveMod = 0.0f, bool _inNewValue = false);
	static void HideUnitSelector();
	static void HideMoveSelector();
	static void HideAttackSelector();
	static bool IsInSpawnArea(	sf::Vector2u& _tilePosition, 
								sf::Vector2u& _tileTopLeft,
								unsigned int& _inAreaWidth,
								unsigned int& _inAreaHeight);
	static bool IsInSpawnArea(	sf::Vector2f& _tilePosition,
								sf::Vector2u& _tileTopLeft,
								unsigned int& _inAreaWidth,
								unsigned int& _inAreaHeight);
	static bool IsInRange(sf::Vector2u& _tilePosition, sf::Vector2u& _tileTarget, int _inRange);
	static bool IsInRange(sf::Vector2u& _tilePosition, sf::Vector2f& _tileTarget, int _inRange);

private:
	COverlayManager();

	static std::vector<sf::Sprite*> m_vecOverlayTileSelector;
	static std::map<std::string, sf::IntRect> m_mapSpriteRect;

	static std::string m_strTileSelector;
	static sf::Texture* m_pOverlayTexture;
	static sf::Text* m_pMoveCostMod;
	static int m_iTileSize;
	static bool m_bShowAttackSelector;
	static bool m_bShowMoveSelector;
};

#endif // !__COVERLAYMANAGER_H__