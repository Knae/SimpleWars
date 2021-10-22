#include "CGameManager.h"

CGameManager::CGameManager()
{
	m_v2uGameWindowSize_Current = m_kv2uGameWindowSize_Default;

	m_ptrUIManager = nullptr;

	m_ptrGameWindow = nullptr;
	m_ptrFont = nullptr;
	m_ptrSpriteBackground = nullptr;
}

CGameManager::~CGameManager()
{
	delete m_ptrUIManager;
	m_ptrUIManager = nullptr;

	delete m_ptrGameWindow;
	m_ptrGameWindow = nullptr;

	delete m_ptrFont;
	m_ptrFont = nullptr;

	delete m_ptrSpriteBackground;
	m_ptrSpriteBackground = nullptr;
}

bool CGameManager::IntializeGame()
{
	if (!m_ptrFont->loadFromFile("font/OpenSans-Regular.ttf"))
	{
		//ERROR: unable to load font file
		std::cout << "\nERROR: unable to load font file\n";
		return false;
	}

	m_ptrGameWindow = new sf::RenderWindow(sf::VideoMode(m_v2uGameWindowSize_Current.x,m_v2uGameWindowSize_Current.y),"Simple Wars");
	m_ptrGameWindow->setVerticalSyncEnabled(false);
	m_ptrGameWindow->setFramerateLimit(30);
	m_ptrGameWindow->clear();

	return true;
}

void CGameManager::SetPointersToOtherSystems(CUIManager* _inputUI)
{
}

void CGameManager::DrawObject(sf::Drawable* _object)
{
}

void CGameManager::DisplayGameWorld()
{
}

void CGameManager::DestroyGameWorld()
{
}

void CGameManager::DisplayDebug()
{
}
