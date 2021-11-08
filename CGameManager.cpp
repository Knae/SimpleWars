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
	m_pOverlayMgr = nullptr;
	m_pUnitsToPlace = nullptr;
	m_SelectedUnit = nullptr;
	m_eCurrentTurn = UIEnums::TURN::NONE;
	m_eCurrentState = UIEnums::GAMESTATE::NONE;
	m_eCurrentUIMouseState = UIEnums::MOUSESTATE::NONE;
	m_eCurrentTypeChosen = CUnitEnums::TYPE::NONE;
	m_bAttackOverlayShown = false;
	m_bExecutingActions = false;
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
	
	m_pUIMgr = nullptr;
	m_pSceneMgr = nullptr;
	m_pUnitMgr = nullptr;
	m_pOverlayMgr = nullptr;
	m_SelectedUnit = nullptr;
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

	CUnitManager::ParseConfig(m_strUnitConfig, m_strFactionConfig);

	m_pGameWindow = new sf::RenderWindow
						(
							sf::VideoMode(m_v2uGameWindowSize_Current.x,m_v2uGameWindowSize_Current.y),
							"Simple Wars"
						);

	m_eCurrentState = UIEnums::GAMESTATE::NONE;
	m_eCurrentTurn = UIEnums::TURN::NONE;

	m_pGameWindow->setVerticalSyncEnabled(false);
	m_pGameWindow->setFramerateLimit(30);
	CUIManager::IntializeUI(m_pGameWindow->getSize() , m_pFont,m_v2uGameWindowSize_Current.x - 192);
	m_pGameWindow->clear();

	m_pGameWindow->display();

	return true;
}

bool CGameManager::UpdateManagers(double& _inElapsedTime)
{
	CUnitManager::Update(_inElapsedTime);

	if (CUIManager::GetIfTurnEndClicked())
	{
		SwitchTurns();
		CUIManager::SetCurrentTurn(m_eCurrentTurn);
	}
	m_eCurrentUIMouseState = CUIManager::GetCurrentState();
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
			m_eCurrentTurn = UIEnums::TURN::BLUE;
			SetUIToGameLoop();
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
				CUIManager::SwitchTurnForUnitPlacment(infantryAmount, tankAmount, artilleryAmount);
			}
			else
			{
				ChangeCurrentState(UIEnums::GAMESTATE::GAMELOOP);
			}

			break;
		}
		case UIEnums::GAMESTATE::GAMELOOP:
		{
			m_eCurrentTurn = ((m_eCurrentTurn == UIEnums::TURN::BLUE)?
							UIEnums::TURN::RED : 
							UIEnums::TURN::BLUE);
			break;
		}
		default:
		{
			break;
		}
	}
}

/// <summary>
/// Assigns the pointers to the other managers
/// *Realized too late that this should have been ref inputs, not
/// pointers. And also i'm confused between setting up 
/// singletons and static classes......C'est la vie
/// </summary>
/// <param name="_inputUI"></param>
/// <param name="_inputSceneMgr"></param>
/// <param name="_inputUnit"></param>
/// <param name="_inputOverlay"></param>
void CGameManager::SetPointersToOtherSystems(	CUIManager* _inputUI,

												CSceneManager* _inputSceneMgr,
												CUnitManager* _inputUnit,
												COverlayManager* _inputOverlay)
{
	m_pUIMgr = _inputUI;
	m_pSceneMgr = _inputSceneMgr;
	m_pUnitMgr = _inputUnit;
	m_pOverlayMgr = _inputOverlay;
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
		ChangeCurrentState(UIEnums::GAMESTATE::UNITPLACEMENT);
		m_pOverlayMgr->InitializeOverlays(m_strUnitConfig, CSceneManager::GetTileSize() );
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
	sf::Vector2f mousePosition = m_pGameWindow->mapPixelToCoords(sf::Mouse::getPosition(*(m_pGameWindow)));

	CSceneManager::DisplayScene(*m_pGameWindow);
	CUnitManager::DisplayUnits(*m_pGameWindow);
	COverlayManager::DisplayOverlays(*m_pGameWindow, mousePosition);
}

bool CGameManager::InitializeUI()
{
	if (m_pSceneMgr != nullptr)
	{
		return CUIManager::IntializeUI(m_pGameWindow->getSize() , m_pFont, m_pSceneMgr->GetCurrentScene()->GetSceneWidth_Pixels());
	}
	else
	{
		std::cout << "/nScene Manager not yet set up!" << std::endl;
		return false;
	}
}

/// <summary>
/// Process what to do when a left click is registered.
/// Depends on location of click and what state the game
/// is currently running at.
/// Only does something if no actions are being executed
/// </summary>
void CGameManager::ProcessMouseClick()
{
	if (!m_bExecutingActions)
	{
		sf::Vector2f mousePosition = m_pGameWindow->mapPixelToCoords(sf::Mouse::getPosition(*(m_pGameWindow)) );
		if (CUIManager::ProcessClick(mousePosition))
		{
			switch (m_eCurrentState)
			{
				case UIEnums::GAMESTATE::UNITPLACEMENT:
				{	
					m_eCurrentTypeChosen = CUIManager::GetChosenUnit();
					break;
				}
				case UIEnums::GAMESTATE::GAMELOOP:
				{
					if (m_SelectedUnit != nullptr)
					{
						m_eCurrentUIMouseState = CUIManager::GetCurrentState();
						if (m_eCurrentUIMouseState == UIEnums::MOUSESTATE::ATTACK && !m_bAttackOverlayShown)
						{
							sf::Vector2u currentUnitPosition = m_SelectedUnit->GetCurrentTile();
							COverlayManager::CreateRangeOverlay(currentUnitPosition, m_SelectedUnit->GetRange());
							COverlayManager::ShowAttackSelector(mousePosition);
							COverlayManager::HideMoveSelector();
							m_bAttackOverlayShown = true;
						}
						else if(m_eCurrentUIMouseState == UIEnums::MOUSESTATE::MOVE)
						{
							if (m_bAttackOverlayShown)
							{
								COverlayManager::ClearRangeOverlay();
								COverlayManager::HideAttackSelector();
								m_bAttackOverlayShown = false;
							}
							COverlayManager::ShowMoveSelector(mousePosition);
						}
						else if (m_eCurrentUIMouseState == UIEnums::MOUSESTATE::SELECT)
						{
							if (m_bAttackOverlayShown)
							{
								COverlayManager::ClearRangeOverlay();
								m_bAttackOverlayShown = false;
							}
							COverlayManager::HideAttackSelector();
							COverlayManager::HideMoveSelector();
						}
					}
					else
					{
						if (m_bAttackOverlayShown)
						{
							COverlayManager::ClearRangeOverlay();
							m_bAttackOverlayShown = false;
						}
						COverlayManager::HideAttackSelector();
						COverlayManager::HideMoveSelector();
					}
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
					if (m_eCurrentTypeChosen != CUnitEnums::TYPE::NONE)
					{
						//Get the amount of this unit that has already been placed
						std::map<CUnitEnums::TYPE, int>::iterator element = m_pUnitsToPlace->find(m_eCurrentTypeChosen);

						int currentAmountPlaced = element->second;
						CUnitEnums::SIDE controllingPlayer = (m_eCurrentTurn == UIEnums::TURN::BLUE) ? (CUnitEnums::SIDE::BLUE) : (CUnitEnums::SIDE::RED);

						if (currentAmountPlaced > 0)
						{
							CTile* clickedTile = m_pSceneMgr->GetTileInScene(mousePosition);

							if (clickedTile != nullptr && clickedTile->GetUnitOnTile() == nullptr &&
								m_pSceneMgr->GetCurrentScene()->GetTileType(mousePosition) != CSceneEnums::TILETYPE::MOUNTAIN)
							{
								CUnit* newUnit = CUnitManager::CreateUnit(m_eCurrentTypeChosen, CUnitEnums::FACTION::TALONS, controllingPlayer);
								clickedTile->UnitEntersTile(newUnit);
								newUnit->SetLocation(mousePosition);

								//Update number of units placed
								(element->second)--;

								clickedTile = nullptr;
								newUnit = nullptr;
							}
						}
					}
					break;
				}
				case UIEnums::GAMESTATE::GAMELOOP:
				{
					//Could move this to its own function
					switch (m_eCurrentUIMouseState)
					{
						case UIEnums::MOUSESTATE::SELECT:
						{
							CTile* clickedTile = m_pSceneMgr->GetTileInScene(mousePosition);
							if (clickedTile != nullptr)
							{
								m_SelectedUnit = clickedTile->GetUnitOnTile();
								if (m_SelectedUnit != nullptr)
								{
									sf::Vector2u unitTilePosition = m_SelectedUnit->GetCurrentTile();								
									COverlayManager::ShowUnitSelector(unitTilePosition);
									CUIManager::UpdateInfoDisplay(clickedTile->GetTileType(),
																m_SelectedUnit->GetSide(),
																m_SelectedUnit->GetType(),
																m_SelectedUnit->GetFaction()
																);
								}	
							}
							else
							{
								COverlayManager::HideUnitSelector();
							}

							clickedTile = nullptr;
							break;
						}
						case UIEnums::MOUSESTATE::MOVE:
						{
							if (m_SelectedUnit != nullptr)
							{
								CTile* targetTile = m_pSceneMgr->GetTileInScene(mousePosition);
								if (targetTile->GetUnitOnTile()== nullptr)
								{
									CTile* currentTileUnitOccupies = m_pSceneMgr->GetTileInScene(m_SelectedUnit->GetSprite()->getPosition());
									if (CUnitManager::MoveUnit(m_SelectedUnit, mousePosition))
									{
										sf::Vector2u unitNewPosition = m_SelectedUnit->GetCurrentTile();
										COverlayManager::ShowUnitSelector(unitNewPosition);
										currentTileUnitOccupies->UnitLeavesTile();
										CTile* clickedTile = m_pSceneMgr->GetTileInScene(mousePosition);
										clickedTile->UnitEntersTile(m_SelectedUnit);
									}
									currentTileUnitOccupies = nullptr;
								}
								targetTile = nullptr;
							}
							break;
						}
						case UIEnums::MOUSESTATE::ATTACK:
						{
							sf::Vector2u currentUnitPosition = m_SelectedUnit->GetCurrentTile();
							if (COverlayManager::IsInRange(currentUnitPosition,mousePosition,m_SelectedUnit->GetRange()))
							{
								//target in range
								std::cout << "\nAttack possible" << std::endl;
							}
							break;
						}
						default:
						{
							break;
						}
					}
				}
				default:
				{
					break;
				}
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
	//m_pUIMgr->ClearUIElements();
	//For now there are only 3 buttons to place,
	//Infantry, tanks and artillery in that order
	int* infantryAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::INFANTRY)->second);
	int* tankAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::TANK)->second);
	int* artilleryAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::ARTILLERY)->second);

	CUIManager::SetUpUnitPlacementPanel(infantryAmount, tankAmount, artilleryAmount);
}

void CGameManager::SetUIToGameLoop()
{
	CUIManager::SetUpGameLoopPanel();
}

void CGameManager::DisplayUI()
{
	CUIManager::DisplayUI(*m_pGameWindow);
}
