#include "CScene.h"

int CScene::m_iBaseColour[] = { 0, 0 ,0 };
int CScene::m_iUnitsRed[] = { 0, 0 ,0 };
int CScene::m_iUnitsBlue[] = { 0, 0 ,0 };

CScene::CScene()
{
	m_MapTileMap = new sf::Texture();
	m_MapTiles = nullptr;
	m_iMapColumns = m_iMapRows = 0;
}

CScene::~CScene()
{
}

/// <summary>
/// Load map configuration from a external file
/// </summary>
/// <param name="_filePath"></param>
/// <returns></returns>
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

				std::getline(mapSettings, currentLine);
				while (currentLine.compare("</MapSize>") != 0)
				{
					ParseLineForMapSize(currentLine, m_iMapRows, m_iMapColumns);
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
							//For each tile, set the tile type to the corresponding type in our
							//vector tiletypes that we read earlier.
							for (unsigned int column = 0; column < m_iMapColumns; column++)
							{
								markerFirstPosition = markerSecondPosition;
								if (markerFirstPosition != 0) { markerFirstPosition++; }
								markerSecondPosition = currentLine.find(',', markerFirstPosition);
								parsedTileValue = std::stoi(currentLine.substr(markerFirstPosition, markerSecondPosition - markerFirstPosition));
								(*m_MapTiles)[row][column].SetTileValue(parsedTileValue);
								tileTypeIt = m_mapTileValuesToCheck.find(parsedTileValue);
								if (tileTypeIt != m_mapTileValuesToCheck.end())
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
					std::cout	<< "\nUError when creating map tiles, unable to set tiles. Config file may not be arranged correctly\n"
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
					ConvertToTileType(lineLabel,lineType);

					//This line is to make sure we start reading after the '=' sign
					markerSecondPosition = currentLine.find('=');
					while (markerSecondPosition != std::string::npos)
					{
						markerFirstPosition = markerSecondPosition+1;
						markerSecondPosition = currentLine.find(',', markerFirstPosition);

						parsedValue = std::stoi(currentLine.substr(markerFirstPosition, markerSecondPosition - markerFirstPosition));
						m_mapTileValuesToCheck.emplace(parsedValue,lineType);
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
				int parsedUnitIndex = 0;

				std::getline(mapSettings, currentLine);
				while (currentLine.compare("</Units_Blue>") != 0)
				{
					ParseLineForUnits(currentLine, m_iUnitsBlue);

					std::getline(mapSettings, currentLine);
				}
			}
			else if (currentLine.compare("<Units_Red>")==0)
			{
				//Getting Red units to place on map
				std::cout << "\nReading Red units on map\n";
				int parsedUnitIndex = 0;

				std::getline(mapSettings, currentLine);
				while (currentLine.compare("</Units_Red>") != 0)
				{
					ParseLineForUnits(currentLine, m_iUnitsRed);

					std::getline(mapSettings, currentLine);
				}
			}
			else if (currentLine.compare("<Units_Red>")==0)
			{
				//Getting Red units to place on map
				std::cout << "\nReading Red units on map\n";
				int parsedUnitIndex = 0;

				std::getline(mapSettings, currentLine);
				while (currentLine.compare("</Units_Red>") != 0)
				{
					ParseLineForUnits(currentLine, m_iUnitsRed);

					std::getline(mapSettings, currentLine);
				}
			}
			else if (currentLine.compare("<Others>") == 0)
			{
				//Getting misc settings
				std::cout << "\nReading other settings\n";

				std::getline(mapSettings, currentLine);
				while (currentLine.compare("</Others>") != 0)
				{
					std::string lineValues;
					std::string lineLabel =	ParseLineGetLabel(currentLine, lineValues);
					if (lineLabel.compare("BaseColour") == 0)
					{
						std::size_t markerFirstPosition = 0;
						std::size_t markerSecondPosition = -1;
						for (int i = 0; i < 3; i++)
						{

								markerFirstPosition = markerSecondPosition + 1;
								markerSecondPosition = lineValues.find(',', markerFirstPosition);

								m_iBaseColour[i] = std::stoi(lineValues.substr(markerFirstPosition, markerSecondPosition - markerFirstPosition));
						}
					}

					std::getline(mapSettings, currentLine);
				}
			}
		}
	}
	else
	{
		//ERROR: Unable to open file.
		std::cout << "\nUnable to open map config.\n";
		return false;
	}
	mapSettings.close();
	return true;
}

bool CScene::InitializeMap()
{
	if (!m_MapTileMap->loadFromFile(m_strTileMapFilePath))
	{
		//ERROR: Unable to load tilemap
		std::cout << "\nFailed loading tilemap!\n";
		return false;
	}

	m_SceneTileVertices.setPrimitiveType(sf::Quads);
	//Based on SFML tutorial, I am assuming it's number of tile
	//and 4 points each tile
	m_SceneTileVertices.resize(m_iTileWidth * m_iTileWidth * 4);

	sf::Vector2f currentTileSize((float)(m_iTileWidth), (float)(m_iTileWidth) );

	for (unsigned short currentRow = 0; currentRow < m_iMapRows; currentRow++)
	{
		for (unsigned short currentColumn = 0; currentColumn < m_iMapColumns; currentColumn++)
		{
			int currentTileMark = (*m_MapTiles)[currentRow][currentColumn].GetTileValue();

			//Which tile in the tileset that
			//we're putting on the background
			int tileMapRow = 0;
			int tileMapColumn = 0;

			//Tilemap uses 32*32 size squares. Using this information to calculate the 
			//total columns in the tilemap, and thus what tile the TileMark
			//corresponds to.   
			tileMapColumn = currentTileMark % (m_MapTileMap->getSize().x / m_iTileWidth);
			tileMapRow = currentTileMark / (m_MapTileMap->getSize().x / m_iTileWidth);

			//If we've done one row, then the indexes for the next row will be will all be
			//plus the number of tiles in a row
			//ie:		index 0 -till-> 9
			// then:	index 10-till->19 (each index is 10 above the previous row)
			//
			sf::Vertex* currentTileQuad = &m_SceneTileVertices[(currentColumn + (currentRow * m_iTileWidth)) * 4];

			//Set the location of the 4 points of the Quads
			currentTileQuad[0].position = sf::Vector2f(currentColumn * currentTileSize.x, currentRow * currentTileSize.y);
			currentTileQuad[1].position = sf::Vector2f((currentColumn + 1) * currentTileSize.x, currentRow * currentTileSize.y);
			currentTileQuad[2].position = sf::Vector2f((currentColumn + 1) * currentTileSize.x, (currentRow + 1) * currentTileSize.y);
			currentTileQuad[3].position = sf::Vector2f(currentColumn * currentTileSize.x, (currentRow + 1) * currentTileSize.y);

			//Set the texture location on the tileset
			currentTileQuad[0].texCoords = sf::Vector2f(tileMapColumn * currentTileSize.x, tileMapRow * currentTileSize.y);
			currentTileQuad[1].texCoords = sf::Vector2f((tileMapColumn + 1) * currentTileSize.x, tileMapRow * currentTileSize.y);
			currentTileQuad[2].texCoords = sf::Vector2f((tileMapColumn + 1) * currentTileSize.x, (tileMapRow + 1) * currentTileSize.y);
			currentTileQuad[3].texCoords = sf::Vector2f(tileMapColumn * currentTileSize.x, (tileMapRow + 1) * currentTileSize.y);
		}
	}
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

/// <summary>
/// Read the line to the number of rows or columns
/// </summary>
/// <param name="_inputLine"></param>
/// <param name="_inputMapRows"></param>
/// <param name="_inputMapColumns"></param>
/// <returns></returns>
bool CScene::ParseLineForMapSize(std::string& _inputLine, unsigned int& _inputMapRows, unsigned int& _inputMapColumns)
{
	std::string lineLabel;
	std::string lineValues;

	lineLabel = ParseLineGetLabel(_inputLine, lineValues);
	if (lineLabel.compare("Rows") == 0)
	{
		_inputMapRows = std::stoi(lineValues);
	}
	else if (lineLabel.compare("Columns") == 0)
	{
		_inputMapColumns = std::stoi(lineValues);
	}
	else
	{
		return false;
	}

	return true;

}
/// <summary>
/// Parse line to get the number of units of that type
/// </summary>
/// <param name="_inputLine"></param>
/// <param name="_inputArrayPointer"></param>
/// <returns></returns>
bool CScene::ParseLineForUnits(std::string& _inputLine, int* _inputArrayPointer)
{
	std::string lineLabel;
	std::string lineValue;
	int parsedUnitIndex = 0;

	lineLabel = ParseLineGetLabel(_inputLine, lineValue);

	if (lineLabel.compare("INFANTRY") == 0)
	{
		parsedUnitIndex = 0;
	}
	else if (lineLabel.compare("TANK") == 0)
	{
		parsedUnitIndex = 1;
	}
	else if (lineLabel.compare("ARTILLERY") == 0)
	{
		parsedUnitIndex = 2;
	}
	else
	{
		//Unrecognised line
		return false;
	}
	*(_inputArrayPointer+ parsedUnitIndex)= std::stoi(lineValue);
	return true;
}

/// <summary>
/// Parse the given string to get the corresponding tiletype
/// </summary>
/// <param name="_inputString"></param>
/// <param name="_outType"></param>
/// <returns></returns>
bool CScene::ConvertToTileType(std::string& _inputString, CSceneEnums::TILETYPE& _outType)
{
	if (_inputString.compare("GRASS") == 0)
	{
		_outType = CSceneEnums::TILETYPE::GRASS;
	}
	else if (_inputString.compare("TALLGRASS") == 0)
	{
		_outType = CSceneEnums::TILETYPE::TALLGRASS;
	}
	else if (_inputString.compare("ROAD") == 0)
	{
		_outType = CSceneEnums::TILETYPE::ROAD;
	}
	else if (_inputString.compare("WATER") == 0)
	{
		_outType = CSceneEnums::TILETYPE::COAST;
	}
	else if (_inputString.compare("WATER") == 0)
	{
		_outType = CSceneEnums::TILETYPE::WATER;
	}
	else if (_inputString.compare("HILL") == 0)
	{
		_outType = CSceneEnums::TILETYPE::HILL;
	}
	else if (_inputString.compare("MOUNTAIN") == 0)
	{
		_outType = CSceneEnums::TILETYPE::MOUNTAIN;
	}
	else if (_inputString.compare("URBAN") == 0)
	{
		_outType = CSceneEnums::TILETYPE::URBAN;
	}
	else if (_inputString.compare("FORT") == 0)
	{
		_outType = CSceneEnums::TILETYPE::FORT;
	}
	else if (_inputString.compare("BASETILE") == 0)
	{
		_outType = CSceneEnums::TILETYPE::BASETILE;
	}
	else
	{
		//Unrecognised tile type name
		return false;
	}
	return true;
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
