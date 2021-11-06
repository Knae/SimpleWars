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
	m_pUnitsToPlace = nullptr;
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

	m_eCurrentState = UIEnums::GAMESTATE::NONE;
	m_eCurrentTurn = UIEnums::TURN::NONE;

	m_pGameWindow->setVerticalSyncEnabled(false);
	m_pGameWindow->setFramerateLimit(30);
	m_pUIMgr->IntializeUI(m_pGameWindow->getSize() , m_pFont,m_v2uGameWindowSize_Current.x - 192);
	m_pGameWindow->clear();

	m_pGameWindow->display();

	return true;
}

bool CGameManager::CheckManagers()
{
	if (m_pUIMgr->GetIfTurnEndClicked())
	{
		SwitchTurns();
		m_pUIMgr->SetCurrentTurn(m_eCurrentTurn);
	}
	return false;
}

/// <summary>
/// Change the current game state to the given state
/// </summary>
/// <param name="_inState">Game state to change to. UIEnums::GAMESTATE</param>
/// <returns></returns>
bool CGameManager::ChangeCurrentState(UIEnums::GAMESTATE _inState)
{
	m_eCurrentState = _inState;
	switch (m_eCurrentState)
	{
		case UIEnums::GAMESTATE::MAPSELECTION:
		{
			break;
		}
		case UIEnums::GAMESTATE::UNITPLACEMENT:
		{
			//Set up for the first player to place units
			m_eCurrentTurn = UIEnums::TURN::BLUE;
			m_pSceneMgr->GetCurrentScene()->GetUnitsToPlace(&m_iUnitsToPlaced_B, &m_iUnitsToPlaced_R);
			m_pUnitsToPlace = &m_iUnitsToPlaced_B;
			SetUIToUnitPlacement();
			break;
		}
		case UIEnums::GAMESTATE::GAMELOOP:
		{
			break;
		}
		default:
		{
			break;
		}
	}
	return false;
}

void CGameManager::SwitchTurns()
{
	switch (m_eCurrentState)
	{
		case UIEnums::GAMESTATE::MAPSELECTION:
		{
			break;
		}
		case UIEnums::GAMESTATE::UNITPLACEMENT:
		{
			if (m_eCurrentTurn == UIEnums::TURN::BLUE)
			{
				m_pUnitsToPlace = &m_iUnitsToPlaced_R;
				m_eCurrentTurn = UIEnums::TURN::RED;
				int* infantryAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::INFANTRY)->second);
				int* tankAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::TANK)->second);
				int* artilleryAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::ARTILLERY)->second);
				m_pUIMgr->SwitchTurnForUnitPlacment(infantryAmount, tankAmount, artilleryAmount);
			}
			else
			{
				m_eCurrentState = UIEnums::GAMESTATE::GAMELOOP;
			}

			break;
		}
		case UIEnums::GAMESTATE::GAMELOOP:
		{
			break;
		}
		default:
		{
			break;
		}
	}
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
	CheckManagers();
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
		ChangeCurrentState(UIEnums::GAMESTATE::UNITPLACEMENT);
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
		return m_pUIMgr->IntializeUI(m_pGameWindow->getSize() , m_pFont, m_pSceneMgr->GetCurrentScene()->GetSceneWidth_Pixels());
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
					//Get the amount of this unit that has already been placed
					std::map<CUnitEnums::TYPE, int>::iterator element = m_pUnitsToPlace->find(m_eCurrentUnitChosen);
					//Shouldn't need to worry about a not already in the list
					/*if (element == m_pUnitsToPlace->end())
					{
						m_iUnitsToPlaced.emplace(m_eCurrentUnitChosen, 0);
						element = m_pUnitsToPlace->end()--;
					}*/
					int currentAmountPlaced = element->second;
					CUnitEnums::SIDE controllingPlayer = (m_eCurrentTurn == UIEnums::TURN::BLUE) ? (CUnitEnums::SIDE::BLUE) : (CUnitEnums::SIDE::RED);

					if (currentAmountPlaced > 0/*< m_pSceneMgr->GetCurrentScene()->GetUnitAmount(controllingPlayer, m_eCurrentUnitChosen)*/)
					{
						CTile* clickedTile = m_pSceneMgr->GetTileInScene(mousePosition);

						if (clickedTile != nullptr && clickedTile->GetUnitOnTile() == nullptr &&
							m_pSceneMgr->GetCurrentScene()->GetTileType(mousePosition) != CSceneEnums::TILETYPE::MOUNTAIN)
						{
							CUnit* newUnit = m_pUnitMgr->CreateUnit(m_eCurrentUnitChosen, CUnitEnums::FACTION::TALONS, controllingPlayer);
							clickedTile->UnitEntersTile(newUnit);
							newUnit->MoveTo(mousePosition);

							//Update number of units placed
							(element->second)--;

							//make player choose again what unit to place, to avoid accidental
							//placement
							m_pUIMgr->SetChosenUnitToNone();
							m_eCurrentUnitChosen = CUnitEnums::TYPE::NONE;
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

/// <summary>
/// Tell UI manager to set up the side panel
/// for placing units.
/// This HAS TO BE DONE LAST.
/// Otherwise the UI manager can't get the right numbers to display
/// </summary>
void CGameManager::SetUIToUnitPlacement()
{
	m_pUIMgr->ClearUIElements();
	//For now there are only 3 buttons to place,
	//Infantry, tanks and artillery in that order
	int* infantryAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::INFANTRY)->second);
	int* tankAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::TANK)->second);
	int* artilleryAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::ARTILLERY)->second);

	m_pUIMgr->SetUpUnitPlacementPanel(infantryAmount, tankAmount, artilleryAmount);
}

void CGameManager::DisplayUI()
{
	m_pUIMgr->DisplayUI(*m_pGameWindow);
}
