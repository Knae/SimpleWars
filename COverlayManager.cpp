#include "COverlayManager.h"

std::string		COverlayManager::m_strTileSelector;
sf::Texture*	COverlayManager::m_pOverlayTexture;
sf::Text*		COverlayManager::m_pMoveCostMod;
int				COverlayManager::m_iTileSize;
bool			COverlayManager::m_bShowAttackSelector;
bool			COverlayManager::m_bShowMoveSelector;
std::map<std::string, sf::IntRect>		COverlayManager::m_mapSpriteRect;
std::vector<sf::Sprite*>				COverlayManager::m_vecOverlayTileSelector;

COverlayManager::COverlayManager()
{
	m_pOverlayTexture = nullptr;
	m_iTileSize = 32;
}

COverlayManager::~COverlayManager()
{
	delete m_pOverlayTexture;
	delete m_pMoveCostMod;

	m_pOverlayTexture = nullptr;
	m_pMoveCostMod = nullptr;

	ClearTileOverlays();
}

/// <summary>
/// Empties the current available overlays and recreates the default ones
/// </summary>
/// <param name="_inConfigPath"></param>
/// <param name="_inFont"></param>
/// <param name="_inTileSize"></param>
void COverlayManager::InitializeOverlays(const std::string& _inConfigPath, sf::Font* _inFont, int _inTileSize)
{
	ClearTileOverlays();
	if (m_pOverlayTexture != nullptr)
	{
		delete m_pOverlayTexture;
		m_pOverlayTexture = nullptr;
	}

	m_pOverlayTexture = new sf::Texture;
	if (!m_pOverlayTexture->loadFromFile("assets/spritemaps/UnitSelection.png"))
	{
		//ERROR: Unable to load textures for overlay
	}

	m_iTileSize = _inTileSize;

	//Quick and dirty method to add IntRects for the 4 selector overlays
	sf::IntRect* currentRect = new sf::IntRect(0,0,32,32);
	m_mapSpriteRect.emplace("Move",*currentRect);

	currentRect = new sf::IntRect(32, 0, 32, 32);
	m_mapSpriteRect.emplace("Attack", *currentRect);

	currentRect = new sf::IntRect(64, 0, 32, 32);
	m_mapSpriteRect.emplace("Selected", *currentRect);

	currentRect = new sf::IntRect(96, 0, 32, 32);
	m_mapSpriteRect.emplace("Range", *currentRect);

	m_pMoveCostMod = new sf::Text();
	m_pMoveCostMod->setFillColor(sf::Color::White);
	m_pMoveCostMod->setOutlineColor(sf::Color::Black);
	m_pMoveCostMod->setOutlineThickness(2.0f);
	m_pMoveCostMod->setCharacterSize(16);
	m_pMoveCostMod->setFont(*_inFont);

	CreateUnitSelectorOverlays();

	currentRect = nullptr;
}

/// <summary>
/// Updates the location of the attack and move overlay as necessary
/// </summary>
/// <param name="_inMouse"></param>
/// <param name="_elapsedTime"></param>
void COverlayManager::Update(sf::Vector2f& _inMouse, double _elapsedTime)
{
	UpdateMoveMod(_inMouse);

	if (m_bShowAttackSelector)
	{
		ShowAttackSelector(_inMouse);
	}
	else if (m_bShowMoveSelector)
	{
		ShowMoveSelector(_inMouse);
	}
}

/// <summary>
/// Draws all the overlay objects
/// </summary>
/// <param name="_inWindow"></param>
void COverlayManager::DisplayOverlays(sf::RenderWindow& _inWindow)
{
	if (m_bShowMoveSelector)
	{
		_inWindow.draw(*m_pMoveCostMod);
	}

	for (auto& element : m_vecOverlayTileSelector)
	{
		_inWindow.draw(*element);
	}
}

/// <summary>
/// Clears all overlays except ther first 3
/// </summary>
void COverlayManager::ClearTileOverlays()
{
	for (auto& element : m_vecOverlayTileSelector)
	{
		delete element;
		element = nullptr;
	}
	m_vecOverlayTileSelector.clear();
	m_mapSpriteRect.clear();
}

/// <summary>
/// Create the unit selector overlays.
/// </summary>
void COverlayManager::CreateUnitSelectorOverlays()
{
	sf::Vector2f positionOffScreen(-32.0f,0.0f);
	sf::Sprite* newRangeOverlay = new sf::Sprite;
	newRangeOverlay->setTexture(*m_pOverlayTexture);
	newRangeOverlay->setTextureRect(m_mapSpriteRect.find("Selected")->second);
	//The the overlay off screen
	newRangeOverlay->setPosition(positionOffScreen);
	m_vecOverlayTileSelector.push_back(newRangeOverlay);
	newRangeOverlay = nullptr;

	newRangeOverlay = new sf::Sprite;
	newRangeOverlay->setTexture(*m_pOverlayTexture);
	newRangeOverlay->setTextureRect(m_mapSpriteRect.find("Move")->second);
	//The the overlay off screen
	newRangeOverlay->setPosition(positionOffScreen);
	m_vecOverlayTileSelector.push_back(newRangeOverlay);
	newRangeOverlay = nullptr;

	newRangeOverlay = new sf::Sprite;
	newRangeOverlay->setTexture(*m_pOverlayTexture);
	newRangeOverlay->setTextureRect(m_mapSpriteRect.find("Attack")->second);
	//The the overlay off screen
	newRangeOverlay->setPosition(positionOffScreen);
	m_vecOverlayTileSelector.push_back(newRangeOverlay);
	newRangeOverlay = nullptr;
}

/// <summary>
/// Creates the spawn area overlay
/// </summary>
/// <param name="_tileTopLeft"></param>
/// <param name="_inAreaWidth"></param>
/// <param name="_inAreaHeight"></param>
void COverlayManager::CreateUnitPlacementOverlay(sf::Vector2u& _tileTopLeft, unsigned int& _inAreaWidth, unsigned int& _inAreaHeight)
{
	for (unsigned int row = _tileTopLeft.y; row <= (_tileTopLeft.y + _inAreaHeight); row++)
	{
		for (unsigned int column = _tileTopLeft.x; column <= (_tileTopLeft.x + _inAreaWidth); column++)
		{
			sf::Vector2u currentTarget(column, row);
			sf::Sprite* newRangeOverlay = new sf::Sprite;
			newRangeOverlay->setTexture(*m_pOverlayTexture);
			newRangeOverlay->setTextureRect(m_mapSpriteRect.find("Selected")->second);
			//convert the position to pixels
			newRangeOverlay->setPosition(sf::Vector2f((float)(currentTarget.x * m_iTileSize), (float)(currentTarget.y * m_iTileSize)));
			m_vecOverlayTileSelector.push_back(newRangeOverlay);
			newRangeOverlay = nullptr;
		}
	}
}

/// <summary>
/// Create the overlay to show the range of the selected unit
/// </summary>
/// <param name="_tilePosition"></param>
/// <param name="_inRange"></param>
void COverlayManager::CreateRangeOverlay(sf::Vector2u& _tilePosition, int _inRange)
{
	int minX = _tilePosition.x - _inRange;
	int minY = _tilePosition.y - _inRange;

	if (minX < 0)
	{
		minX = 0;
	}

	if (minY < 0)
	{
		minY = 0;
	}

	for (int row = minY; row <= (minY + (2 * _inRange)); row++)
	{
		for (int column = minX; column <= (minX + (2 * _inRange)); column++)
		{
			sf::Vector2u currentTarget(column, row);
			if (IsInRange(_tilePosition, currentTarget, _inRange))
			{
				//std::string mapKey("Range");
				sf::Sprite* newRangeOverlay = new sf::Sprite;
				newRangeOverlay->setTexture(*m_pOverlayTexture);
				newRangeOverlay->setTextureRect(m_mapSpriteRect.find("Range")->second);
				//convert the position to pixels
				newRangeOverlay->setPosition(sf::Vector2f( (float)(currentTarget.x*m_iTileSize), (float)(currentTarget.y * m_iTileSize) ));
				m_vecOverlayTileSelector.push_back(newRangeOverlay);
				newRangeOverlay = nullptr;
			}
		}
	}
}

/// <summary>
/// Remove the range overlay
/// </summary>
void COverlayManager::ClearRangePlacementOverlay()
{
	if (m_vecOverlayTileSelector.size() > 0)
	{
		for (unsigned int i = 3; i < m_vecOverlayTileSelector.size(); i++)
		{
			delete m_vecOverlayTileSelector[i];
			m_vecOverlayTileSelector[i] = nullptr;
		}

		m_vecOverlayTileSelector.erase( m_vecOverlayTileSelector.begin()+3,m_vecOverlayTileSelector.end() );
	}

}

/// <summary>
/// Show the overlay to indicate the chosen unit
/// </summary>
/// <param name="_inTileLocation"></param>
void COverlayManager::ShowUnitSelector(sf::Vector2u& _inTileLocation)
{
	//convert the pixel location to tile index to figure out which tile
	//the overlay will be on. Then convert back to get the top-left pixel location of
	//that tile.
	//[NOTE]Repeated use. Probably should be set to a common function
	//sf::Vector2i tileAtLocation( (int)(_inTileLocation.x/m_iTileSize), (int)(_inTileLocation.y/m_iTileSize) );
	if (m_vecOverlayTileSelector.size() >= 3)
	{
		m_vecOverlayTileSelector[0]->setPosition(sf::Vector2f((float)(_inTileLocation.x * m_iTileSize), (float)(_inTileLocation.y * m_iTileSize)));
	}
}

/// <summary>
/// Shows and updates the location of the the overlay to indicate the tile to move to
/// </summary>
/// <param name="_mouseLocation"></param>
void COverlayManager::ShowMoveSelector(sf::Vector2f& _mouseLocation)
{
	//convert the pixel location to tile index to figure out which tile
	//the overlay will be on. Then convert back to get the top-left pixel location of
	//that tile.
	//[NOTE]Repeated use.Probably should be set to a common function
	if (m_vecOverlayTileSelector.size() >= 3)
	{
		sf::Vector2i tileAtLocation((int)(_mouseLocation.x / m_iTileSize), (int)(_mouseLocation.y / m_iTileSize));
		m_vecOverlayTileSelector[1]->setPosition(sf::Vector2f((float)(tileAtLocation.x * m_iTileSize), (float)(tileAtLocation.y * m_iTileSize)));
		m_bShowMoveSelector = true;
	}
	else
	{
		m_bShowMoveSelector = false;
	}
}


/// <summary>
/// shows and updates the location of the overlay that indicates the tile to attack
/// </summary>
/// <param name="_mouseLocation"></param>
void COverlayManager::ShowAttackSelector(sf::Vector2f& _mouseLocation)
{
	//convert the pixel location to tile index to figure out which tile
	//the overlay will be on. Then convert back to get the top-left pixel location of
	//that tile.
	//[NOTE]Repeated use. Probably should be set to a common function
	if (m_vecOverlayTileSelector.size() >= 3)
	{
		sf::Vector2i tileAtLocation((int)(_mouseLocation.x / m_iTileSize), (int)(_mouseLocation.y / m_iTileSize));
		m_vecOverlayTileSelector[2]->setPosition(sf::Vector2f( (float)(tileAtLocation.x * m_iTileSize), (float)(tileAtLocation.y * m_iTileSize) ));
		m_bShowAttackSelector = true;
	}
	else
	{
		m_bShowAttackSelector = false;
	}
}

/// <summary>
/// Update the overlay that shows the additionla MOV cost to
/// move a unit to the tile, as well as the position of overlay
/// </summary>
/// <param name="_inMousePosition"></param>
/// <param name="_inMoveMod"></param>
/// <param name="_inNewValue"></param>
void COverlayManager::UpdateMoveMod(sf::Vector2f& _inMousePosition, float _inMoveMod, bool _inNewValue)
{
	if (_inNewValue)
	{
		if (_inMoveMod > 0.0f)
		{
			m_pMoveCostMod->setFillColor(sf::Color::Green);
		}
		else if (_inMoveMod < 0.0f)
		{
			m_pMoveCostMod->setFillColor(sf::Color::Red);
		}
		else
		{
			m_pMoveCostMod->setFillColor(sf::Color::White);
		}

		std::stringstream shortenDecimal;
		shortenDecimal << std::setprecision(1) << std::fixed << _inMoveMod;

		std::string moveMod = "MOV cost:\n " + shortenDecimal.str();
		m_pMoveCostMod->setString(moveMod);
	}
	
	m_pMoveCostMod->setPosition(_inMousePosition.x + 16.0f, _inMousePosition.y - 48.0f);
}

void COverlayManager::HideUnitSelector()
{
	m_vecOverlayTileSelector[0]->setPosition(sf::Vector2f(-32.0f, 0.0f));
}

void COverlayManager::HideMoveSelector()
{
	m_vecOverlayTileSelector[1]->setPosition(sf::Vector2f(-32.0f, 0.0f));
	m_bShowMoveSelector = false;
}

void COverlayManager::HideAttackSelector()
{
	m_vecOverlayTileSelector[2]->setPosition(sf::Vector2f(-32.0f, 0.0f));
	m_bShowAttackSelector = false;
}

/// <summary>
/// checks if the position is within the spawn area
/// </summary>
/// <param name="_tilePosition"></param>
/// <param name="_tileTopLeft"></param>
/// <param name="_inAreaWidth"></param>
/// <param name="_inAreaHeight"></param>
/// <returns></returns>
bool COverlayManager::IsInSpawnArea(sf::Vector2u& _tilePosition, sf::Vector2u& _tileTopLeft, unsigned int& _inAreaWidth, unsigned int& _inAreaHeight)
{
	if ((_tilePosition.x >= _tileTopLeft.x && _tilePosition.x <= (_tileTopLeft.x + _inAreaWidth)) &&
		(_tilePosition.y >= _tileTopLeft.y && _tilePosition.y <= (_tileTopLeft.y + _inAreaHeight)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/// <summary>
/// converts the position vector to the int equivalent
/// before checking if the position is within the spawn area
/// </summary>
/// <param name="_tilePosition"></param>
/// <param name="_tileTopLeft"></param>
/// <param name="_inAreaWidth"></param>
/// <param name="_inAreaHeight"></param>
/// <returns></returns>
bool COverlayManager::IsInSpawnArea(sf::Vector2f& _tilePosition, sf::Vector2u& _tileTopLeft, unsigned int& _inAreaWidth, unsigned int& _inAreaHeight)
{
	sf::Vector2u targetTile((unsigned int)(_tilePosition.x / m_iTileSize),
							(unsigned int)(_tilePosition.y / m_iTileSize));

	return IsInSpawnArea(targetTile, _tileTopLeft, _inAreaWidth, _inAreaHeight);
}

/// <summary>
/// Checks if tileTarget is within range of tilePosition
/// </summary>
/// <param name="_tilePosition"></param>
/// <param name="_tileTarget"></param>
/// <param name="_inRange"></param>
/// <returns></returns>
bool COverlayManager::IsInRange(sf::Vector2u& _tilePosition, sf::Vector2u& _tileTarget, int _inRange)
{
	int tileDistanceX = _tileTarget.x - _tilePosition.x;
	int tileDistanceY = _tileTarget.y - _tilePosition.y;

	if (abs(tileDistanceX) + abs(tileDistanceY) <= _inRange)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/// <summary>
/// Converts position vector of tileTarget to int equivalent
/// before checking if it is in range of tilePosition
/// </summary>
/// <param name="_tilePosition"></param>
/// <param name="_tileTarget"></param>
/// <param name="_inRange"></param>
/// <returns></returns>
bool COverlayManager::IsInRange(sf::Vector2u& _tilePosition, sf::Vector2f& _tileTarget, int _inRange)
{
	sf::Vector2u targetTile(	(unsigned int)(_tileTarget.x / m_iTileSize),
								(unsigned int)(_tileTarget.y / m_iTileSize) );

	return IsInRange(_tilePosition,targetTile,_inRange);
}

