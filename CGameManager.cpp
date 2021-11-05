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
	m_pUnitMgr = nullptr;
	m_eCurrentTurn = UIEnums::TURN::NONE;
	m_eCurrentState = UIEnums::GAMESTATE::NONE;
	m_eCurrentUnitChosen = CUnitEnums::TYPE::NONE;

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
		std::cout << "\nERROR: unable to load font file" << std::endl;
		return false;
	}

	m_pUnitMgr->ParseConfig(m_strUnitConfig, m_strFactionConfig);

	m_pGameWindow = new sf::RenderWindow
						(
							sf::VideoMode(m_v2uGameWindowSize_Current.x,m_v2uGameWindowSize_Current.y),
							"Simple Wars"
						);

	m_eCurrentState = UIEnums::GAMESTATE::UNITPLACEMENT;
	m_eCurrentTurn = UIEnums::TURN::BLUE;

	m_pGameWindow->setVerticalSyncEnabled(false);
	m_pGameWindow->setFramerateLimit(30);
	m_pUIMgr->IntializeUI(m_pGameWindow->getSize() , m_v2uGameWindowSize_Current.x - 192);
	m_pGameWindow->clear();

	m_pGameWindow->display();

	return true;
}

void CGameManager::SetPointersToOtherSystems(	CUIManager* _inputUI,
												CSceneManager* _inputSceneMgr,
												CUnitManager* _inputUnit)
{
	m_pUIMgr = _inputUI;
	m_pSceneMgr = _inputSceneMgr;
	m_pUnitMgr = _inputUnit;
}

void CGameManager::DrawObject(sf::Drawable* _object)
{
}

void CGameManager::DisplayGameWorld()
{
	m_pGameWindow -> clear(sf::Color::Black);
	DisplayScene();
	DisplayUI();
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
		std::cout << "\nSuccesfully loaded Mountain Village map." << std::endl;
		return true;
	}
	else
	{
		std::cout << "\nUnable to Mountain Village map." << std::endl;
		return false;
	}
	
}

//Display the map and the units in it
void CGameManager::DisplayScene()
{
	m_pSceneMgr->DisplayScene(*m_pGameWindow);
	m_pUnitMgr->DisplayUnits(*m_pGameWindow);
}

bool CGameManager::InitializeUI()
{
	if (m_pSceneMgr != nullptr)
	{
		return m_pUIMgr->IntializeUI(m_pGameWindow->getSize() , m_pSceneMgr->GetCurrentScene()->GetSceneWidth_Pixels());
	}
	else
	{
		std::cout << "/nScene Manager not yet set up!" << std::endl;
		return false;
	}

}

void CGameManager::ProcessMouseClick()
{
	sf::Vector2f mousePosition = m_pGameWindow->mapPixelToCoords(sf::Mouse::getPosition(*(m_pGameWindow)) );
	if (m_pUIMgr->ProcessClick(mousePosition))
	{
		//m_eCurrentState = m_pUIMgr->GetCurrentState();
		switch (m_eCurrentState)
		{
			case UIEnums::GAMESTATE::UNITPLACEMENT:
			{	
				m_eCurrentUnitChosen = m_pUIMgr->GetChosenUnit();
				break;
			}
			default:
			{
				break;
			}
		}
	}
	else
	{
		switch (m_eCurrentState)
		{
			case UIEnums::GAMESTATE::UNITPLACEMENT:
			{
				if (m_eCurrentUnitChosen != CUnitEnums::TYPE::NONE)
				{
					CTile* clickedTile = m_pSceneMgr->GetTileInScene(mousePosition);
					if (clickedTile != nullptr && clickedTile->GetUnitOnTile() == nullptr &&
						m_pSceneMgr->GetCurrentScene()->GetTileType(mousePosition)!= CSceneEnums::TILETYPE::MOUNTAIN)
					{
						CUnitEnums::SIDE controllingPlayer = (m_eCurrentTurn == UIEnums::TURN::BLUE) ? (CUnitEnums::SIDE::BLUE) : (CUnitEnums::SIDE::RED);
						CUnit* newUnit = m_pUnitMgr->CreateUnit(m_eCurrentUnitChosen, CUnitEnums::FACTION::TALONS, controllingPlayer);
						clickedTile->UnitEntersTile(newUnit);
						newUnit->MoveTo(mousePosition);

						//Update number of units placed
						std::map<CUnitEnums::TYPE, int>::iterator element = m_iUnitPlaced.find(m_eCurrentUnitChosen);
						if (element != m_iUnitPlaced.end())
						{
							(element->second)++;
						}
						else
						{
							m_iUnitPlaced.emplace(m_eCurrentUnitChosen,1);
						}
					}
				}
				break;
			}
			default:
			{
				break;
			}
		}
	}

}

void CGameManager::DisplayUI()
{
	m_pUIMgr->DisplayUI(*m_pGameWindow);
}
