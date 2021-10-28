#pragma once
#ifndef __CSCENE_H__
#define __CSCENE_H__

#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "CSceneEnums.h"
#include "CTile.h"

/// <summary>
/// Generic class for generating a scene/map
/// Currently no need for specialized creation
/// </summary>
class CScene :public sf::Drawable, public sf::Transformable
{
public:
	CScene();
	virtual ~CScene();

	virtual bool LoadMapConfig(const std::string& _filePath);
	virtual bool InitializeMap();
	virtual CSceneEnums::TILETYPE GetTileType(sf::Vector2i );
	virtual int GetUnits_Player();
	virtual int GetUnits_Opponent();
	static int* GetBaseColourArrayPointer() { return m_iBaseColour; }

private:
	static std::string ParseLineGetLabel (const std::string& _inputLine, std::string& _lineSettingValues);
	static bool ParseLineForMapSize(std::string& _inputLine, unsigned int& _inputMapRows, unsigned int& _inputMapColumns);
	static bool ParseLineForUnits(std::string& _inputLine, int* _inputArrayPointer);
	static bool ConvertToTileType(std::string& _inputString, CSceneEnums::TILETYPE& _outType);
	
	const int m_iTileWidth = 32;
	static int m_iBaseColour[3];
	static int m_iUnitsRed[3];
	static int m_iUnitsBlue[3];

	std::string m_strTileMapFilePath;
	//18 rows x 26 columns
	//CTile m_MapTile[18][26];
	std::vector<std::vector<CTile>>* m_MapTiles;
	unsigned int m_iMapColumns;
	unsigned int m_iMapRows;


	std::map<unsigned int, CSceneEnums::TILETYPE> m_mapTileValuesToCheck;
	sf::VertexArray m_SceneTileVertices;
	sf::Texture* m_MapTileMap;


	//This overriding function was taken from SFML's tutorial
	//for using the vertex array
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // !__CSCENE_H__