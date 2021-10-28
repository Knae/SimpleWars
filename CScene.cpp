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
			if (currentLine.compare("<MapSize>")==0)
			{
				//Getting the map size from map config
				std::cout << "\nReading map size\n";

				std::string lineLabel;
				std::string lineValues;
				std::getline(mapSettings, currentLine);
				while (currentLine.compare("</MapSize>") != 0)
				{
					lineLabel = ParseLineGetLabel(currentLine, lineValues);
					if (lineLabel.compare("Rows") == 0)
					{
						m_iMapRows = std::stoi(lineValues);
					}
					else if (lineLabel.compare("Columns") == 0)
					{
						m_iMapColumns = std::stoi(lineValues);
					}
					std::getline(mapSettings, currentLine);
				}
				m_MapTiles = new std::vector<std::vector<CTile>>(m_iMapRows,std::vector<CTile>(m_iMapColumns) );
			}
			else if (currentLine.compare("<Tiles>")==0)
			{
				if (m_MapTiles != nullptr)
				{
					//Getting the tile setup from map config
					std::cout << "\nReading tile information\n";
					std::size_t markerFirstPosition = 0;
					std::size_t markerSecondPosition = 0;
					int parsedTileValue = 0;
					std::map<unsigned int, CSceneEnums::TILETYPE>::iterator tileTypeIt;
					while (currentLine.compare("</Tiles>") != 0)
					{
						for (unsigned int row = 0; row < m_iMapRows; row++)
						{
							std::getline(mapSettings, currentLine);
							for (unsigned int column = 0; column < m_iMapColumns; column++)
							{
								markerFirstPosition = markerSecondPosition;
								if (markerFirstPosition != 0) { markerFirstPosition++; }
								markerSecondPosition = currentLine.find(',', markerFirstPosition);
								parsedTileValue = std::stoi(currentLine.substr(markerFirstPosition, markerSecondPosition - markerFirstPosition));
								(*m_MapTiles)[row][column].SetTileValue(parsedTileValue);
								tileTypeIt = m_MapTileValuesToCheck.find(parsedTileValue);
								if (tileTypeIt != m_MapTileValuesToCheck.end())
								{
									(*m_MapTiles)[row][column].SetTileType(tileTypeIt->second);
								}
								else
								{
									(*m_MapTiles)[row][column].SetTileType(CSceneEnums::TILETYPE::NONE);
								}
							}
						}
						std::getline(mapSettings, currentLine);
					}
				}
				else
				{
					//ERROR: MapTiles not created yet
					std::cout	<< "\nUnable to create map tiles. Config file may not be arranged correctly\n"
								<< "or the map size was stored incorrectly\n";
					std::getline(mapSettings, currentLine);
				}			
			}
			else if (currentLine.compare("<TileTypes>")==0)
			{
				std::string lineLabel;
				std::string lineValues;
				int parsedValue = 0;
				CSceneEnums::TILETYPE lineType = CSceneEnums::TILETYPE::NONE;
				std::size_t markerFirstPosition = 0;
				std::size_t markerSecondPosition = 0;

				//Getting tile types on map
				std::cout << "\nReading tile codes and relevant types\n";

				std::getline(mapSettings, currentLine);
				while (currentLine.compare("</TileTypes>") != 0)
				{
					lineLabel = ParseLineGetLabel(currentLine, lineValues);
					if (lineLabel.compare("GRASS") == 0)
					{
						lineType = CSceneEnums::TILETYPE::GRASS;
					}
					else if (lineLabel.compare("TALLGRASS") == 0)
					{
						lineType = CSceneEnums::TILETYPE::TALLGRASS;
					}
					else if (lineLabel.compare("ROAD") == 0)
					{
						lineType = CSceneEnums::TILETYPE::ROAD;
					}
					else if (lineLabel.compare("WATER") == 0)
					{
						lineType = CSceneEnums::TILETYPE::WATER;
					}
					else if (lineLabel.compare("HILL") == 0)
					{
						lineType = CSceneEnums::TILETYPE::HILL;
					}
					else if (lineLabel.compare("MOUNTAIN") == 0)
					{
						lineType = CSceneEnums::TILETYPE::MOUNTAIN;
					}
					else if (lineLabel.compare("URBAN") == 0)
					{
						lineType = CSceneEnums::TILETYPE::URBAN;
					}
					else if (lineLabel.compare("FORT") == 0)
					{
						lineType = CSceneEnums::TILETYPE::FORT;
					}

					//This line is to make sure we start reading after the '=' sign
					markerSecondPosition = currentLine.find('=');
					while (markerSecondPosition != std::string::npos)
					{
						markerFirstPosition = markerSecondPosition+1;
						markerSecondPosition = currentLine.find(',', markerFirstPosition);

						parsedValue = std::stoi(currentLine.substr(markerFirstPosition, markerSecondPosition - markerFirstPosition));
						m_MapTileValuesToCheck.emplace(parsedValue,lineType);
					}

					std::getline(mapSettings, currentLine);
				}
			}
			else if (currentLine.compare("<TileMapPath>")==0)
			{
				//Getting path to tilemap file
				std::cout << "\nReading tilemap path\n";
				std::getline(mapSettings, currentLine);
				ParseLineGetLabel(currentLine,m_strTileMapFilePath);
			}
			else if (currentLine.compare("<Units_Blue>")==0)
			{
				//Getting blue units to place on map
				std::cout << "\nReading Blue units on map\n";
			}
			else if (currentLine.compare("<Units_Red>")==0)
			{
				//Getting Red units to place on map
				std::cout << "\nReading Red units on map\n";
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
/// <summary>
/// Processes the line provided. Returns the label at the beginning
/// of the line and writes the values into arg2
/// </summary>
/// <param name="_inputLine"></param>
/// <param name="_lineSettingValues"></param>
/// <returns></returns>
std::string CScene::ParseLineGetLabel (const std::string& _inputLine, std::string& _lineSettingValues)
{ 
	std::size_t positionOfSymbol = _inputLine.find('=');
	if (positionOfSymbol != std::string::npos)
	{
		std::string parsedLabel = _inputLine.substr(0, positionOfSymbol);
		std::string parsedValue = _inputLine.substr(positionOfSymbol + 1, std::string::npos);
		_lineSettingValues = parsedValue;
		return parsedLabel;
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
