#include "CGameManager.h"

CGameManager::CGameManager()
{
	m_v2uGameWindowSize_Current = m_kv2uGameWindowSize_Default;

	m_pGameWindow = nullptr;
	m_pFont = nullptr;
	m_pGameBackground = nullptr;
	m_pSpriteBackground = nullptr;
	m_pUIMgr = nullptr;
	m_pSceneMgr = nullptr;

}

CGameManager::~CGameManager()
{
	delete m_pGameWindow;
	m_pGameWindow = nullptr;

	delete m_pFont;
	m_pFont = nullptr;

	delete m_pGameBackground;
	m_pGameBackground = nullptr;

	delete m_pSpriteBackground;
	m_pSpriteBackground = nullptr;
}

bool CGameManager::IntializeGame()
{
	m_pFont = new sf::Font();
	if (!m_pFont->loadFromFile("font/OpenSans-Regular.ttf"))
	{
		//ERROR: unable to load font file
		std::cout << "\nERROR: unable to load font file\n";
		return false;
	}

	m_pGameWindow = new sf::RenderWindow
						(
							sf::VideoMode(m_v2uGameWindowSize_Current.x,m_v2uGameWindowSize_Current.y),
							"Simple Wars"
						);

	m_pGameWindow->setVerticalSyncEnabled(false);
	m_pGameWindow->setFramerateLimit(30);
	m_pGameWindow->clear();

	m_pGameBackground = new sf::RenderTexture();
	m_pGameBackground->create(m_kv2uGameWindowSize_Default.x, m_kv2uGameWindowSize_Default.y);
	m_pGameBackground->clear(sf::Color::White);

	m_pSpriteBackground = new sf::Sprite();
	m_pSpriteBackground->setTexture(m_pGameBackground->getTexture(),true);

	m_pGameBackground->display();
	m_pGameWindow->display();

	return true;
}

void CGameManager::SetPointersToOtherSystems(CUIManager* _inputUI, CSceneManager* _inputSceneMgr)
{
	m_pSceneMgr = _inputSceneMgr;
}

void CGameManager::DrawObject(sf::Drawable* _object)
{
}

void CGameManager::DisplayGameWorld()
{
	m_pGameWindow->draw( (*m_pSpriteBackground) );
	m_pGameWindow->display();
}

void CGameManager::DestroyGameWorld()
{
	m_pGameWindow->close();
}

void CGameManager::DisplayDebug()
{
}

bool CGameManager::LoadScene()
{
	CSceneEnums::SCENETYPE sceneType = CSceneEnums::SCENETYPE::MOUNTAINGRASS;
	if (CSceneManager::CreateScene(sceneType, m_strMountainVillageConfig))
	{
		std::cout << "\nSuccesfully loaded Mountain Village map.\n";
		return true;
	}
	else
	{
		std::cout << "\nUnable to Mountain Village map.\n";
		return false;
	}
	
}

void CGameManager::DisplayScene()
{
	m_pSceneMgr->DisplayScene(*m_pGameWindow);
}
