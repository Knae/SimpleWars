#pragma once
#ifndef __CSCENE_H__
#define __CSCENE_H__

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include "CSceneEnums.h"
#include "CUnitEnums.h"
#include "CTile.h"
#include "CParseConfigCommon.h"

/// <summary>
/// Generic class for generating a scene/map
/// Currently no need for specialized creation
/// </summary>
class CScene :public sf::Drawable, public sf::Transformable, public CParseConfigCommon
{
public:
	CScene();
	virtual ~CScene();

	virtual bool ParseConfig(const std::string& _filePath);
	virtual bool InitializeMap();
	virtual void ResetTiles();
	virtual CSceneEnums::TILETYPE GetTileType(sf::Vector2f _input);
	virtual CSceneEnums::TILETYPE GetTileType(unsigned int _inX, unsigned int _inY);
	virtual int GetUnitAmount(CUnitEnums::SIDE _inSide, CUnitEnums::TYPE _inType);
	virtual void GetUnitsToPlace(std::map<CUnitEnums::TYPE, int>* _inUnitsB, std::map<CUnitEnums::TYPE, int>* _inUnitsR);
	virtual void PlaceAIUnits();
	int* GetBaseColourArrayPointer() { return m_iBaseColour; }
	virtual unsigned int GetSceneWidth_Pixels() { return m_uiMapColumns*m_uiTileWidth; }
	virtual unsigned int GetSceneHeight_Pixels() { return m_uiMapRows*m_uiTileWidth; }
	virtual unsigned int GetTileWidth() { return m_uiTileWidth; }
	virtual CTile* GetTile(sf::Vector2f _inPosition);
	virtual CTile* GetTile(unsigned int _inX, unsigned int _inY);
private:
	typedef struct recSpawnArea
	{
		sf::Vector2u m_UpperLeft;
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
	}Spawn;

	static bool ParseLineForMapSize(std::string& _inputLine, unsigned int& _inputMapRows, unsigned int& _inputMapColumns);
	static bool ParseLineForUnits(std::string& _inputLine, int& _outUnitAmount, CUnitEnums::TYPE& _outType);
	
	const unsigned int m_uiTileWidth = 32;
	int m_iBaseColour[3] = { 0 };
	std::map<CUnitEnums::TYPE, int> m_mapUnitsRed;
	std::map<CUnitEnums::TYPE, int> m_mapUnitsBlue;

	std::string m_strTileMapFilePath;
	std::vector<std::vector<CTile>>* m_pMapTiles;
	std::map< CUnitEnums::TYPE, std::vector<sf::Vector2u>*> m_AILocations;
	Spawn m_SpawnBlue;
	Spawn m_SpawnRed;
	unsigned int m_uiMapColumns;
	unsigned int m_uiMapRows;

	std::map<unsigned int, CSceneEnums::TILETYPE> m_mapTileValuesToCheck;
	sf::VertexArray m_SceneTileVertices;
	sf::Texture* m_pMapTileMap;

	//This overriding function was taken from SFML's tutorial
	//for using the vertex array
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // !__CSCENE_H__