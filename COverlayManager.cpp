#include "COverlayManager.h"

sf::Texture*	COverlayManager::m_pOverlayTexture;
std::string		COverlayManager::m_strTileSelector;
int				COverlayManager::m_iTileSize;
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

	m_pOverlayTexture = nullptr;

	ClearTileOverlays();
}

void COverlayManager::InitializeOverlays(const std::string& _inConfigPath, int _inTileSize)
{
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

	currentRect = nullptr;
}

void COverlayManager::Update(double& _elapsedTime)
{

}

void COverlayManager::DisplayOverlays(sf::RenderWindow& _inWindow, sf::Vector2f& _inMouse)
{

	for (auto& element : m_vecOverlayTileSelector)
	{
		_inWindow.draw(*element);
	}
}

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
	//convert the position to pixels
	newRangeOverlay->setPosition(positionOffScreen);
	m_vecOverlayTileSelector.push_back(newRangeOverlay);
	newRangeOverlay = nullptr;

	newRangeOverlay = new sf::Sprite;
	newRangeOverlay->setTexture(*m_pOverlayTexture);
	newRangeOverlay->setTextureRect(m_mapSpriteRect.find("Move")->second);
	//convert the position to pixels
	newRangeOverlay->setPosition(positionOffScreen);
	m_vecOverlayTileSelector.push_back(newRangeOverlay);
	newRangeOverlay = nullptr;

	newRangeOverlay = new sf::Sprite;
	newRangeOverlay->setTexture(*m_pOverlayTexture);
	newRangeOverlay->setTextureRect(m_mapSpriteRect.find("Attack")->second);
	//convert the position to pixels
	newRangeOverlay->setPosition(positionOffScreen);
	m_vecOverlayTileSelector.push_back(newRangeOverlay);
	newRangeOverlay = nullptr;
}

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

	for (int row = minY; row < (minY + (2 * _inRange)); row++)
	{
		for (int column = minX; column < (minX + (2 * _inRange)); column++)
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

void COverlayManager::ClearRangeOverlay()
{
	for (unsigned int i = 2; i < m_vecOverlayTileSelector.size(); i++)
	{
		delete m_vecOverlayTileSelector[i];
		m_vecOverlayTileSelector[i] = nullptr;
	}

	m_vecOverlayTileSelector.erase( m_vecOverlayTileSelector.begin()+3,m_vecOverlayTileSelector.end() );
}

void COverlayManager::ShowUnitSelector(sf::Vector2u& _inTileLocation)
{
	//convert the pixel location to tile index to figure out which tile
	//the overlay will be on. Then convert back to get the top-left pixel location of
	//that tile.
	//sf::Vector2i tileAtLocation( (int)(_inTileLocation.x/m_iTileSize), (int)(_inTileLocation.y/m_iTileSize) );
	m_vecOverlayTileSelector[0]->setPosition(sf::Vector2f((float)(_inTileLocation.x * m_iTileSize), (float)(_inTileLocation.y * m_iTileSize) ));
}

void COverlayManager::ShowMoveSelector(sf::Vector2f& _mouseLocation)
{
	m_vecOverlayTileSelector[1]->setPosition(_mouseLocation);
}

void COverlayManager::ShowAttackSelector(sf::Vector2f& _mouseLocation)
{
	m_vecOverlayTileSelector[2]->setPosition(_mouseLocation);
}

void COverlayManager::HideUnitSelector()
{
	m_vecOverlayTileSelector[0]->setPosition(sf::Vector2f(-32.0f, 0.0f));
}

void COverlayManager::HideMoveSelector()
{
	m_vecOverlayTileSelector[1]->setPosition(sf::Vector2f(-32.0f, 0.0f));
}

void COverlayManager::HideAttackSelector()
{
	m_vecOverlayTileSelector[2]->setPosition(sf::Vector2f(-32.0f, 0.0f));
}

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

bool COverlayManager::IsInRange(sf::Vector2u& _tilePosition, sf::Vector2f& _tileTarget, int _inRange)
{
	sf::Vector2u targetTile(	(unsigned int)(_tileTarget.x / m_iTileSize),
								(unsigned int)(_tileTarget.y / m_iTileSize) );

	return IsInRange(_tilePosition,targetTile,_inRange);
}

