#include "CScene.h"

CScene::CScene()
{
	m_MapTileMap = nullptr;
	m_MapTiles = nullptr;
}

CScene::~CScene()
{
}

bool CScene::LoadMapConfig(const std::string& _filePath)
{
	std::fstream mapSettings;
	std::string currentLine;

	mapSettings.open(_filePath);
	if (mapSettings.is_open())
	{
		while (std::getline(mapSettings, currentLine))
		{
			if (currentLine.compare("[MapSize]")==0)
			{
				//Getting the map size from map config
				std::cout << "\nReading map size\n";
				std::getline(mapSettings, currentLine);
				m_iMapRows = std::stoi(ParseLine(currentLine, "Rows"));
				std::getline(mapSettings, currentLine);
				m_iMapColumns = std::stoi(ParseLine(currentLine, "Columns"));
				m_MapTiles = new std::vector<std::vector<CTile>>(m_iMapRows,std::vector<CTile>(m_iMapColumns) );
			}
			else if (currentLine.compare("[Tiles]")==0)
			{
				//Getting the tile setup from map config
				std::cout << "\nReading tile information\n";
				for (unsigned int row = 0; row < m_iMapRows; row++)
				{
					for (unsigned int column = 0; column < m_iMapColumns; column++)
					{

					}
				}
			}
			else if (currentLine.compare("[TileTypes]")==0)
			{
				//Getting blue units to place on map
				std::cout << "\nReading tile codes and relevant types\n";
			}
			else if (currentLine.compare("[TileMapPath]")==0)
			{
				//Getting blue units to place on map
				std::cout << "\nReading tilemap path\n";
			}
			else if (currentLine.compare("[Units_Blue]")==0)
			{
				//Getting blue units to place on map
				std::cout << "\nReading Blue units on map\n";
			}
			else if (currentLine.compare("[Units_Red]")==0)
			{
				//Getting blue units to place on map
				std::cout << "\nReading Blue units on map\n";
			}
		}
	}
	else
	{
		//ERROR: Unable to open file.
		std::cout << "\nUnable to open map config.\n";
		return false;
	}
	return true;
}

bool CScene::InitializeMap()
{
	return false;
}

CSceneEnums::TILETYPE CScene::GetTileType(sf::Vector2i)
{
	return CSceneEnums::TILETYPE();
}

int CScene::GetUnits_Player()
{
	return 0;
}

int CScene::GetUnits_Opponent()
{
	return 0;
}

std::string CScene::ParseLine(const std::string& _inputLine, const std::string& _inputSearchCriteria)
{ 
	std::size_t positionOfSymbol = _inputLine.find('=');
	if (positionOfSymbol != std::string::npos && _inputLine.compare(0,positionOfSymbol,_inputSearchCriteria)==0)
	{
		std::string parsedValue = _inputLine.substr(positionOfSymbol + 1, std::string::npos);
		return parsedValue;
	}
	else
	{
		return std::string();
	}
}

void CScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = m_MapTileMap;

	// draw the vertex array
	target.draw(m_SceneTileVertices, states);
}
