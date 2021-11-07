#include "COverlayManager.h"

sf::Texture*	COverlayManager::m_pOverlayTexture;
std::string		COverlayManager::m_strTileSelector;
std::map<std::string*, sf::IntRect*>	COverlayManager::m_mapSpriteRect;
std::vector<sf::Sprite*>				COverlayManager::m_vecOverlayTileSelector;


COverlayManager::COverlayManager()
{
	m_pOverlayTexture = nullptr;
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

	//Quick and dirty method to add IntRects for the 4 selector overlays
	sf::IntRect* currentRect = new sf::IntRect(0,0,32,32);
	std::string* RectLabel= new std::string("Move");
	m_mapSpriteRect.emplace(RectLabel,currentRect);

	currentRect = new sf::IntRect(32, 0, 32, 32);
	RectLabel = new std::string("Attack");
	m_mapSpriteRect.emplace(RectLabel, currentRect);

	currentRect = new sf::IntRect(64, 0, 32, 32);
	RectLabel = new std::string("Selected");
	m_mapSpriteRect.emplace(RectLabel, currentRect);

	currentRect = new sf::IntRect(96, 0, 32, 32);
	RectLabel = new std::string("Range");
	m_mapSpriteRect.emplace(RectLabel, currentRect);
}

void COverlayManager::DisplayOverlays(sf::RenderWindow& _inWindow)
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
}

void COverlayManager::CreateRangeOverlay(sf::Vector2i& _tilePosition, int _inRange)
{

}
