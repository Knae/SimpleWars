#pragma once
#ifndef __CSCENE_H__
#define __CSCENE_H__

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "CSceneEnums.h"

/// <summary>
/// Base class for all scenes in map. Set up as interface
/// </summary>
class CScene :public sf::Drawable, public sf::Transformable
{
public:
	CScene();
	virtual ~CScene();
	virtual bool InitializeMap() = 0;
	virtual CSceneEnums::SCENENAME GetSceneName() = 0;
	virtual CSceneEnums::TILETYPE GetTileType(sf::Vector2i ) = 0;
	virtual int GetNumberOfUnits_Player() = 0;
	virtual int GetNumberOfUnits_Opponent() = 0;

private:

};

#endif // !__CSCENE_H__