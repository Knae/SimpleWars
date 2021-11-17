#include "CScene.h"

CScene::CScene()
{
	m_pMapTileMap = new sf::Texture();
	m_pMapTiles = nullptr;
	m_uiMapColumns = m_uiMapRows = 0;
}

CScene::~CScene()
{
	if (m_pMapTiles != nullptr)
	{
		delete m_pMapTiles;
		m_pMapTiles = nullptr;
	}
}

/// <summary>
/// Load map configuration from a external file
/// TODO: separate each parsing process to its own function
/// </summary>
/// <param name="_filePath"></param>
/// <returns></returns>
bool CScene::ParseConfig(const std::string& _filePath)
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
					ParseLineForMapSize(currentLine, m_uiMapRows, m_uiMapColumns);
					std::getline(mapSettings, currentLine);
				}
				m_pMapTiles = new std::vector<std::vector<CTile>>(m_uiMapRows,std::vector<CTile>(m_uiMapColumns) );
			}
			else if (currentLine.compare("<Tiles>")==0)
			{
				if (m_pMapTiles != nullptr)
				{
					//Getting the tile setup from map config
					std::cout << "\nReading tile information\n";
					std::size_t markerFirstPosition = 0;
					std::size_t markerSecondPosition = 0;
					int parsedTileValue = 0;
					std::map<unsigned int, CSceneEnums::TILETYPE>::iterator tileTypeIt;
					while (currentLine.compare("</Tiles>") != 0)
					{
						for (unsigned int row = 0; row < m_uiMapRows; row++)
						{
							std::getline(mapSettings, currentLine);
							//For each tile, set the tile type to the corresponding type in our
							//vector tiletypes that we read earlier.
							for (unsigned int column = 0; column < m_uiMapColumns; column++)
							{
								markerFirstPosition = markerSecondPosition;
								if (markerFirstPosition != 0) { markerFirstPosition++; }
								markerSecondPosition = currentLine.find(',', markerFirstPosition);
								parsedTileValue = std::stoi(currentLine.substr(markerFirstPosition, markerSecondPosition - markerFirstPosition));
								(*m_pMapTiles)[row][column].SetTileValue(parsedTileValue);
								tileTypeIt = m_mapTileValuesToCheck.find(parsedTileValue);
								if (tileTypeIt != m_mapTileValuesToCheck.end())
								{
									(*m_pMapTiles)[row][column].SetTileType(tileTypeIt->second);
								}
								else
								{
									(*m_pMapTiles)[row][column].SetTileType(CSceneEnums::TILETYPE::NONE);
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
				int parsedUnitAmount = 0;
				CUnitEnums::TYPE parsedType = CUnitEnums::TYPE::NONE;

				std::getline(mapSettings, currentLine);
				while (currentLine.compare("</Units_Blue>") != 0)
				{
					ParseLineForUnits(currentLine, parsedUnitAmount, parsedType);
					m_mapUnitsBlue.emplace(parsedType, parsedUnitAmount);

					std::getline(mapSettings, currentLine);
				}
			}
			else if (currentLine.compare("<Units_Red>")==0)
			{
				//Getting Red units to place on map
				std::cout << "\nReading Red units on map\n";
				int parsedUnitAmount = 0;
				CUnitEnums::TYPE parsedType = CUnitEnums::TYPE::NONE;

				std::getline(mapSettings, currentLine);
				while (currentLine.compare("</Units_Red>") != 0)
				{
					ParseLineForUnits(currentLine, parsedUnitAmount, parsedType);
					m_mapUnitsRed.emplace(parsedType, parsedUnitAmount);
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
	if (!m_pMapTileMap->loadFromFile(m_strTileMapFilePath))
	{
		//ERROR: Unable to load tilemap
		std::cout << "\nFailed loading tilemap!\n";
		return false;
	}

	m_SceneTileVertices.setPrimitiveType(sf::Quads);
	//Based on SFML tutorial, I am assuming it's number of tiles
	//and 4 points each tile
	m_SceneTileVertices.resize(m_uiTileWidth * m_uiTileWidth * 4);

	sf::Vector2f currentTileSize((float)(m_uiTileWidth), (float)(m_uiTileWidth) );

	for (unsigned short currentRow = 0; currentRow < m_uiMapRows; currentRow++)
	{
		for (unsigned short currentColumn = 0; currentColumn < m_uiMapColumns; currentColumn++)
		{
			int currentTileMark = (*m_pMapTiles)[currentRow][currentColumn].GetTileValue();

			//Which tile in the tileset that
			//we're putting on the background
			int tileMapRow = 0;
			int tileMapColumn = 0;

			//Tilemap uses 32*32 size squares. Using this information to calculate the 
			//total columns in the tilemap, and thus what tile the TileMark
			//corresponds to.   
			tileMapColumn = currentTileMark % (m_pMapTileMap->getSize().x / m_uiTileWidth);
			tileMapRow = currentTileMark / (m_pMapTileMap->getSize().x / m_uiTileWidth);

			//If we've done one row, then the indexes for the next row will be will all be
			//plus the number of tiles in a row
			//ie:		index 0 -till-> 9
			// then:	index 10-till->19 (each index is 10 above the previous row)
			//
			sf::Vertex* currentTileQuad = &m_SceneTileVertices[(currentColumn + (currentRow * m_uiTileWidth)) * 4];

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

void CScene::ResetTiles()
{
	for (unsigned short currentRow = 0; currentRow < m_uiMapRows; currentRow++)
	{
		for (unsigned short currentColumn = 0; currentColumn < m_uiMapColumns; currentColumn++)
		{
			(*m_pMapTiles)[currentRow][currentColumn].UnitLeavesTile();
		}
	}
}

CSceneEnums::TILETYPE CScene::GetTileType(sf::Vector2f _input)
{
	return GetTile(_input)->GetTileType();
}

CSceneEnums::TILETYPE CScene::GetTileType(unsigned int _inX, unsigned int _inY)
{
	return GetTile(_inX, _inY)->GetTileType();
}

/// <summary>
/// Get the amount of units of a given type that the given side
/// should have on this map
/// </summary>
/// <param name="_inSide">The side in question. CUnitEnums::SIDE</param>
/// <param name="_inType">The unit type in question. CUnitEnums::TYPE</param>
/// <returns></returns>
int CScene::GetUnitAmount(CUnitEnums::SIDE _inSide, CUnitEnums::TYPE _inType)
{
	std::map<CUnitEnums::TYPE, int>::iterator element;

	if (_inSide == CUnitEnums::SIDE::BLUE)
	{
		element = m_mapUnitsBlue.find(_inType);
		if(element!=m_mapUnitsBlue.end())
		{
			return element->second;
		}
	}
	else
	{
		element = m_mapUnitsRed.find(_inType);
		if (element != m_mapUnitsBlue.end())
		{
			return element->second;
		}
	}

	return 0;
}

/// <summary>
///  Populate the given map containers with the number units to be placed
///  for the player side
/// </summary>
/// <param name="_inUnitsB">pointer to the container for Blue units. std::map<CUnitEnums::TYPE, int>*</param>
/// <param name="_inUnitsR">pointer to the container for Red units. std::map<CUnitEnums::TYPE, int>*</param>
void CScene::GetUnitsToPlace(std::map<CUnitEnums::TYPE, int>* _inUnitsB, std::map<CUnitEnums::TYPE, int>* _inUnitsR)
{
		//element = m_mapUnitsBlue.find(_inType);
	for(auto& element:m_mapUnitsBlue)
	{
		int amount = element.second;
		(*_inUnitsB).emplace(element.first, amount);
	}
	
	for (auto& element : m_mapUnitsRed)
	{
		int amount = element.second;
		(*_inUnitsR).emplace(element.first, amount);
	}
}

CTile* CScene::GetTile(sf::Vector2f _inPosition)
{
	unsigned int tilePositionX = (unsigned int)(_inPosition.x / m_uiTileWidth);
	unsigned int tilePositionY = (unsigned int)(_inPosition.y / m_uiTileWidth);

	if (tilePositionX < m_uiMapColumns &&
		tilePositionY < m_uiMapRows &&
		tilePositionX >= 0 && tilePositionY >= 0)
	{
		CTile* tile = GetTile(tilePositionX, tilePositionY);
		return tile;
	}
	else
	{
		return nullptr;
	}

}
CTile* CScene::GetTile(unsigned int _inX, unsigned int _inY)
{
	if (_inX < m_uiMapColumns && _inY < m_uiMapRows &&
		_inX >= 0 && _inY >= 0)
	{
		CTile* tile = &(*m_pMapTiles)[_inY][_inX];
		return tile;
	}
	else
	{
		return nullptr;
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
/// Parse the line for the number of units of what type on this map
/// </summary>
/// <param name="_inputLine">The single line from the config file. string&/param>
/// <param name="_outUnitAmount">Where the unit amount is returned. int&</param>
/// <param name="_outType">Where the unit type is stored. CUnitEnums::TYPE&</param>
/// <returns></returns>
bool CScene::ParseLineForUnits(std::string& _inputLine, int& _outUnitAmount, CUnitEnums::TYPE& _outType)
{
	std::string lineLabel;
	std::string lineValue;
	int parsedUnitIndex = 0;

	lineLabel = ParseLineGetLabel(_inputLine, lineValue);

	if (CParseConfigCommon::ConvertToUnitType(lineLabel, _outType))
	{
		_outUnitAmount = std::stoi(lineValue);
		return true;
	}
	else
	{
		_outUnitAmount = 0;
		return false;
	}
}


/// <summary>
/// Overriding the sfml default draw function to draw all the vertices
/// created to render the background map
/// </summary>
/// <param name="target"></param>
/// <param name="states"></param>
void CScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = m_pMapTileMap;

	// draw the vertex array
	target.draw(m_SceneTileVertices, states);
}
