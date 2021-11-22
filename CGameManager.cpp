#include "CGameManager.h"

CGameManager::CGameManager()
{
	m_GameWindowSize_Current = m_GameWindowSize_Default;

	m_pGameWindow = nullptr;
	m_pFont = nullptr;
	m_pGameBackground = nullptr;
	m_pSpriteBackground = nullptr;
	m_pUIMgr = nullptr;
	m_pSceneMgr = nullptr;
	m_pUnitMgr = nullptr;
	m_pOverlayMgr = nullptr;
	m_pUnitsToPlace = nullptr;
	m_pSelectedUnit = nullptr;
	m_eCurrentTurn = CUIEnums::TURN::NONE;
	m_eCurrentState = CUIEnums::GAMESTATE::NONE;
	m_eCurrentUIMouseState = CUIEnums::MOUSESTATE::NONE;
	m_eCurrentTypeChosen = CUnitEnums::TYPE::NONE;
	m_bAttackOverlayShown = false;
	m_bExecutingActions = false;
	m_bWaitingForClick = false;
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
	m_pSelectedUnit = nullptr;
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
							sf::VideoMode(m_GameWindowSize_Current.x,m_GameWindowSize_Current.y),
							"Simple Wars"
						);

	m_eCurrentState = CUIEnums::GAMESTATE::NONE;
	m_eCurrentTurn = CUIEnums::TURN::NONE;

	m_pGameWindow->setVerticalSyncEnabled(false);
	m_pGameWindow->setFramerateLimit(60);
	CUIManager::IntializeUI(m_pGameWindow->getSize() , m_pFont,m_GameWindowSize_Current.x - 192);
	m_pGameWindow->clear();
	m_pGameWindow->display();

	m_refDebug.Initialize(m_pFont);

	return true;
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
void CGameManager::SetPointersToOtherSystems(CUIManager* _inputUI,
	CSceneManager* _inputSceneMgr,
	CUnitManager* _inputUnit,
	COverlayManager* _inputOverlay)
{
	m_pUIMgr = _inputUI;
	m_pSceneMgr = _inputSceneMgr;
	m_pUnitMgr = _inputUnit;
	m_pOverlayMgr = _inputOverlay;
}

bool CGameManager::UpdateManagers(double& _inElapsedTime)
{
	sf::Vector2f mousePosition = m_pGameWindow->mapPixelToCoords(sf::Mouse::getPosition(*(m_pGameWindow)));

	CUnitManager::Update(_inElapsedTime);

	if (CUIManager::GetIfForfeitChosen())
	{
		switch (m_eCurrentTurn)
		{
			case CUIEnums::TURN::BLUE:
			{
				CUIManager::VictoryAchieved(CUIEnums::TURN::RED);
				break;
			}
			case CUIEnums::TURN::RED:
			{
				CUIManager::VictoryAchieved(CUIEnums::TURN::BLUE);
				break;
			}
			default:
			case CUIEnums::TURN::NONE:
			{
				break;
			}
		}
		ChangeCurrentState(CUIEnums::GAMESTATE::GAMEEND);
	}
	else if (CUIManager::GetIfTurnEndClicked())
	{
		SwitchTurns();
		CUIManager::SetCurrentTurn(m_eCurrentTurn);
	}

	if (!CUnitManager::CheckIfAnyUnitsLeft(CParseConfigCommon::Convert(m_eCurrentTurn)) && m_bExecutingActions)
	{
		//GameEnds
		std::cout << "\nAll Enemy units  have died!" << std::endl;
		m_bExecutingActions = false;
		CUIManager::VictoryAchieved(m_eCurrentTurn);
		ChangeCurrentState(CUIEnums::GAMESTATE::GAMEEND);
	}

	m_eCurrentUIMouseState = CUIManager::GetMouseCurrentState();

	if (CheckIfMouseOverTile(mousePosition))
	{
		CTile* tileUnderMouse = CSceneManager::GetTileInScene(mousePosition);
		if (tileUnderMouse->GetUnitOnTile() != nullptr)
		{
			UpdateSidePanelInfo(tileUnderMouse->GetUnitOnTile());
		}
		else
		{
			UpdateSidePanelInfo();
		}

		if (m_eCurrentUIMouseState == CUIEnums::MOUSESTATE::MOVE && m_pSelectedUnit != nullptr)
		{
			//Need to rethink the structure of the managers
			if (tileUnderMouse != nullptr)
			{
				CSceneEnums::TILETYPE tileType = tileUnderMouse->GetTileType();
				CTerrainEffects* tileEffects = CUnitManager::ResolveTerrainEffects(m_pSelectedUnit->GetType(), tileType);
				float moveMod = tileEffects->GetModifierMovement();
				COverlayManager::UpdateMoveMod(mousePosition, moveMod, true);
			}
		}
		COverlayManager::Update(mousePosition);
	}
	else
	{
		UpdateSidePanelInfo();
	}
	
	return false;
}

/// <summary>
/// Change the current game state to the given state
/// </summary>
/// <param name="_inState">Game state to change to. CUIEnums::GAMESTATE</param>
/// <returns></returns>
bool CGameManager::ChangeCurrentState(CUIEnums::GAMESTATE _inState)
{
	m_eCurrentState = _inState;

	switch (m_eCurrentState)
	{
		case CUIEnums::GAMESTATE::MAPSELECTION:
		{
			CSceneManager::GetCurrentScene()->ResetTiles();
			CUIManager::ResetChecks();
			ClearUnitsToPlace();
			break;
		}
		case CUIEnums::GAMESTATE::UNITPLACEMENT:
		{
			CSceneManager::GetCurrentScene()->ResetTiles();	//for testing, skipping map selection
			CUIManager::ResetChecks();						//This too
			ClearUnitsToPlace();							//Same for this line. Delete after
			//Set up for the first player to place units
			m_eCurrentTurn = CUIEnums::TURN::BLUE;
			m_pSceneMgr->GetCurrentScene()->GetUnitsToPlace(&m_mapUnitsToPlaced_B, &m_mapUnitsToPlaced_R);
			m_pUnitsToPlace = &m_mapUnitsToPlaced_B;
			SetUIToUnitPlacement();
			break;
		}
		case CUIEnums::GAMESTATE::GAMELOOP:
		{
			m_eCurrentTurn = CUIEnums::TURN::BLUE;
			SetUIToGameLoop();
			break;
		}
		case CUIEnums::GAMESTATE::GAMEEND:
		{
			m_eCurrentState = CUIEnums::GAMESTATE::GAMEEND;
			m_bWaitingForClick = true;
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

		case CUIEnums::GAMESTATE::MAPSELECTION:
		{
			break;
		}
		case CUIEnums::GAMESTATE::UNITPLACEMENT:
		{
			if (m_eCurrentTurn == CUIEnums::TURN::BLUE)
			{
				m_pUnitsToPlace = &m_mapUnitsToPlaced_R;
				m_eCurrentTurn = CUIEnums::TURN::RED;
				int* infantryAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::INFANTRY)->second);
				int* tankAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::TANK)->second);
				int* artilleryAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::ARTILLERY)->second);
				CUIManager::SwitchTurnForUnitPlacment(infantryAmount, tankAmount, artilleryAmount);
			}
			else
			{
				ChangeCurrentState(CUIEnums::GAMESTATE::GAMELOOP);
			}

			break;
		}
		case CUIEnums::GAMESTATE::GAMELOOP:
		{
			m_eCurrentTurn = ((m_eCurrentTurn == CUIEnums::TURN::BLUE) ? (CUIEnums::TURN::RED) : (CUIEnums::TURN::BLUE));
			CUnitManager::SwitchTurns();
			break;
		}
		default:
		{
			break;
		}
	}
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

void CGameManager::ClearUnitsToPlace()
{
	m_mapUnitsToPlaced_B.clear();
	m_mapUnitsToPlaced_R.clear();
}

bool CGameManager::LoadScene()
{
	CSceneEnums::SCENETYPE sceneType = CSceneEnums::SCENETYPE::MOUNTAINGRASS;
	if (CSceneManager::CreateScene(sceneType, m_strMountainVillageConfig))
	{
		std::cout << "\nSuccesfully loaded Mountain Village map." << std::endl;
		ChangeCurrentState(CUIEnums::GAMESTATE::UNITPLACEMENT);
		m_pOverlayMgr->InitializeOverlays(m_strUnitConfig, m_pFont, CSceneManager::GetTileSize() );
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
	CSceneManager::DisplayScene(*m_pGameWindow);
	CUnitManager::DisplayUnits(*m_pGameWindow);
	COverlayManager::DisplayOverlays(*m_pGameWindow);
}

bool CGameManager::InitializeUI()
{
	if (m_pSceneMgr != nullptr)
	{
		return CUIManager::IntializeUI(m_pGameWindow->getSize() , m_pFont, m_pSceneMgr->GetCurrentScene()->GetSceneWidth_Pixels());
	}
	else
	{
		std::cout << "\nScene Manager not yet set up!" << std::endl;
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
	if (!m_bExecutingActions && !m_bWaitingForClick)
	{
		sf::Vector2f mousePosition = m_pGameWindow->mapPixelToCoords(sf::Mouse::getPosition(*(m_pGameWindow)) );
		std::cout << "\nClicking at " << mousePosition.x << "x " << mousePosition.y << "y" << std::endl;
		//Will get 'true' if we clicked on the side panel
		//and false if we clicked on the map instead, since this is
		//from the perspective of the UIManager
		if (CUIManager::ProcessClick(mousePosition))
		{
			switch (m_eCurrentState)
			{
				case CUIEnums::GAMESTATE::UNITPLACEMENT:
				{	
					m_eCurrentTypeChosen = CUIManager::GetChosenUnit();
					if (m_eCurrentTypeChosen != CUnitEnums::TYPE::NONE)
					{
						COverlayManager::ShowMoveSelector(mousePosition);
					}
					else
					{
						COverlayManager::HideMoveSelector();
					}
					break;
				}
				case CUIEnums::GAMESTATE::GAMELOOP:
				{
					if (m_pSelectedUnit != nullptr)
					{
						m_eCurrentUIMouseState = CUIManager::GetMouseCurrentState();
						//Need to move overlay toggling to its own function
						if (m_eCurrentUIMouseState == CUIEnums::MOUSESTATE::ATTACK && !m_bAttackOverlayShown)
						{
							sf::Vector2u currentUnitPosition = m_pSelectedUnit->GetCurrentTile();
							int unitRange = CUnitManager::GetUnitRange(m_pSelectedUnit);
							COverlayManager::CreateRangeOverlay(currentUnitPosition, unitRange);
							COverlayManager::ShowAttackSelector(mousePosition);
							COverlayManager::HideMoveSelector();
							m_bAttackOverlayShown = true;
						}
						else if(m_eCurrentUIMouseState == CUIEnums::MOUSESTATE::MOVE)
						{
							if (m_bAttackOverlayShown)
							{
								COverlayManager::ClearRangeOverlay();
								COverlayManager::HideAttackSelector();
								m_bAttackOverlayShown = false;
							}
							COverlayManager::ShowMoveSelector(mousePosition);
						}
						else if (m_eCurrentUIMouseState == CUIEnums::MOUSESTATE::SELECT)
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
				case CUIEnums::GAMESTATE::UNITPLACEMENT:
				{
					if (m_eCurrentTypeChosen != CUnitEnums::TYPE::NONE)
					{
						//Get the amount of this unit that has already been placed
						std::map<CUnitEnums::TYPE, int>::iterator element = m_pUnitsToPlace->find(m_eCurrentTypeChosen);

						int currentAmountPlaced = element->second;
						CUnitEnums::SIDE controllingPlayer = (m_eCurrentTurn == CUIEnums::TURN::BLUE) ? (CUnitEnums::SIDE::BLUE) : (CUnitEnums::SIDE::RED);

						if (currentAmountPlaced > 0)
						{
							CTile* clickedTile = m_pSceneMgr->GetTileInScene(mousePosition);

							if (clickedTile != nullptr && clickedTile->GetUnitOnTile() == nullptr /*&&
								m_pSceneMgr->GetCurrentScene()->GetTileType(mousePosition) != CSceneEnums::TILETYPE::MOUNTAIN*/)
							{
								CUnit* newUnit = CUnitManager::CreateUnit(m_eCurrentTypeChosen, CUnitEnums::FACTION::TALONS, controllingPlayer);
								clickedTile->UnitEntersTile(newUnit);
								newUnit->SetLocation(mousePosition);
								newUnit->SetCurrentTileType(m_pSceneMgr->GetCurrentScene()->GetTileType(mousePosition));

								//Update number of units placed
								(element->second)--;

								clickedTile = nullptr;
								newUnit = nullptr;
							}
						}
					}
					break;
				}
				case CUIEnums::GAMESTATE::GAMELOOP:
				{
					//Could move this to its own function
					switch (m_eCurrentUIMouseState)
					{
						case CUIEnums::MOUSESTATE::SELECT:
						{
							CTile* clickedTile = m_pSceneMgr->GetTileInScene(mousePosition);
							if (clickedTile != nullptr)
							{
								m_pSelectedUnit = clickedTile->GetUnitOnTile();
								if (m_pSelectedUnit != nullptr)
								{
									sf::Vector2u unitTilePosition = m_pSelectedUnit->GetCurrentTile();								
									COverlayManager::ShowUnitSelector(unitTilePosition);
									UpdateSidePanelInfo();
									CUIManager::SetCurrentUnitHasAttacked(m_pSelectedUnit->GetHasAtacked());
									if (m_pSelectedUnit->GetMovePoints() <= 0)
									{
										CUIManager::SetCurrentUnitHasNoMovePoints(true);
									}
									else
									{
										CUIManager::SetCurrentUnitHasNoMovePoints(false);
									}
									CUIManager::UpdateUI();
								}
								else
								{
									COverlayManager::HideUnitSelector();
								}
							}
							else
							{
								COverlayManager::HideUnitSelector();
							}

							clickedTile = nullptr;
							break;
						}
						case CUIEnums::MOUSESTATE::MOVE:
						{
							CTile* targetTile = m_pSceneMgr->GetTileInScene(mousePosition);
							if (m_pSelectedUnit != nullptr && targetTile != nullptr)
							{
								if (targetTile->GetUnitOnTile()== nullptr)
								{
									CTile* currentTileUnitOccupies = m_pSceneMgr->GetTileInScene(m_pSelectedUnit->GetSprite()->getPosition());
									if (CUnitManager::MoveUnit(m_pSelectedUnit, mousePosition,targetTile->GetTileType()))
									{
										sf::Vector2u unitNewPosition = m_pSelectedUnit->GetCurrentTile();
										COverlayManager::ShowUnitSelector(unitNewPosition);
										currentTileUnitOccupies->UnitLeavesTile();
										CTile* clickedTile = m_pSceneMgr->GetTileInScene(mousePosition);
										clickedTile->UnitEntersTile(m_pSelectedUnit);

										if (m_pSelectedUnit->GetMovePoints() <= 0)
										{
											CUIManager::SetCurrentUnitHasNoMovePoints(true);
											CUIManager::UpdateUI();
										}
									}
									currentTileUnitOccupies = nullptr;
								}
								//If you clicked on a controllable unit, select it.
								else if (targetTile->GetUnitOnTile()->GetSide() == CParseConfigCommon::Convert(m_eCurrentTurn))
								{
									m_pSelectedUnit = targetTile->GetUnitOnTile();
									sf::Vector2u unitTilePosition = m_pSelectedUnit->GetCurrentTile();
									COverlayManager::ShowUnitSelector(unitTilePosition);
									CUIManager::SetCurrentUnitHasAttacked(m_pSelectedUnit->GetHasAtacked());
									m_pSelectedUnit->GetMovePoints() <= 0 ? CUIManager::SetCurrentUnitHasNoMovePoints(true) : CUIManager::SetCurrentUnitHasNoMovePoints(false);
									UpdateSidePanelInfo();
									CUIManager::UpdateUI();
								}
								targetTile = nullptr;
							}
							else
							{
								CUIManager::SetCurrentMouseState(CUIEnums::MOUSESTATE::SELECT);
								//m_eCurrentUIMouseState = CUIManager::GetMouseCurrentState();
							}
							break;
						}
						case CUIEnums::MOUSESTATE::ATTACK:
						{
							CTile* targetTile = CSceneManager::GetTileInScene(mousePosition);
							if (m_pSelectedUnit!=nullptr && targetTile!=nullptr)
							{
								sf::Vector2u currentUnitPosition = m_pSelectedUnit->GetCurrentTile();
								CUnit* targetUnit = targetTile->GetUnitOnTile();
								if (targetUnit!=nullptr)
								{
									int unitRange = CUnitManager::GetUnitRange(m_pSelectedUnit);
									//it is one of yours, then switch to it
									if (targetUnit->GetSide() == CParseConfigCommon::Convert(m_eCurrentTurn))
									{
										std::cout << "\nSwitching unit.." << std::endl;
										m_pSelectedUnit = targetUnit;
										currentUnitPosition = m_pSelectedUnit->GetCurrentTile();
										UpdateSidePanelInfo(targetUnit);
										CUIManager::SetCurrentUnitHasAttacked(m_pSelectedUnit->GetHasAtacked());

										if (!m_pSelectedUnit->GetHasAtacked())
										{
											unitRange = CUnitManager::GetUnitRange(m_pSelectedUnit);
											COverlayManager::ClearRangeOverlay();
											COverlayManager::CreateRangeOverlay(currentUnitPosition, unitRange);
											m_bAttackOverlayShown = true;
										}
										else
										{
											CUIManager::SetCurrentMouseState(CUIEnums::MOUSESTATE::SELECT);
											COverlayManager::ClearRangeOverlay();
											m_bAttackOverlayShown = false;
										}
										m_pSelectedUnit->GetMovePoints() <= 0 ? CUIManager::SetCurrentUnitHasNoMovePoints(true) : CUIManager::SetCurrentUnitHasNoMovePoints(false);
										CUIManager::UpdateUI();
									}
									else if (m_pSelectedUnit->GetHasAtacked())
									{
										std::cout << "\nUnit has already attacked." << std::endl;
									}
									else if (COverlayManager::IsInRange(currentUnitPosition, mousePosition, unitRange))
									{
										//target in range
										std::cout << "\nTile In Range!" << std::endl;
										std::cout << "\nAttacking target in tile." << std::endl;
										//float damageCaused = m_pSelectedUnit->GetDamageDealt();
										//float remainingHP = targetUnit->TakeDamage(damageCaused);
										//m_pSelectedUnit->SetHasAttacked();
										CUnitManager::Attack(m_pSelectedUnit, targetUnit);
										CUIManager::SetCurrentUnitHasAttacked(true);
										if (targetUnit->GetHP() <= 0)
										{
											std::cout << "\nTarget was destroyed!" << std::endl;
											targetUnit->ExplodeInFlamingGlory();
											sf::Vector2u targetTilePosition = targetUnit->GetCurrentTile();
											CSceneManager::GetTileInScene(targetTilePosition)->UnitLeavesTile();
										}
										CUIManager::SetCurrentMouseState(CUIEnums::MOUSESTATE::SELECT);
										COverlayManager::ClearRangeOverlay();
										m_bAttackOverlayShown = false;
									}
								}
								targetUnit = nullptr;
							}
							targetTile = nullptr;
							m_bExecutingActions = true;
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
	else if(m_bWaitingForClick)
	{
		//ResetAll units, clear map, go back to main menu
		//ChangeCurrentState(CUIEnums::GAMESTATE::MAPSELECTION);
		CUnitManager::ClearUnits();
		ChangeCurrentState(CUIEnums::GAMESTATE::UNITPLACEMENT);
		m_bWaitingForClick = false;
	}
	else if (m_bExecutingActions)
	{
		m_bExecutingActions = false;
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

void CGameManager::UpdateSidePanelInfo(	CUnit* _inViewedUnit)
{
	if (m_eCurrentState == CUIEnums::GAMESTATE::UNITPLACEMENT)
	{

	}
	else if (m_eCurrentState == CUIEnums::GAMESTATE::GAMELOOP)
	{
		CTerrainEffects* selectedUnitTerrain = CUnitManager::ResolveTerrainEffects(CUnitEnums::TYPE::NONE, CSceneEnums::TILETYPE::NONE);
		CTerrainEffects* viewedUnitTerrain = CUnitManager::ResolveTerrainEffects(CUnitEnums::TYPE::NONE, CSceneEnums::TILETYPE::NONE);

		if (m_pSelectedUnit != nullptr)
		{
			selectedUnitTerrain = CUnitManager::ResolveTerrainEffects(m_pSelectedUnit->GetType(), m_pSelectedUnit->GetCurrentTileType());
		}

		if(_inViewedUnit != nullptr)
		{
			viewedUnitTerrain = CUnitManager::ResolveTerrainEffects(_inViewedUnit->GetType(), _inViewedUnit->GetCurrentTileType());
		}

		CUIManager::UpdateInfoDisplay(	m_pSelectedUnit,
										_inViewedUnit,
										selectedUnitTerrain,
										viewedUnitTerrain);

		//Update the debug window as well
		m_refDebug.Update(m_pSelectedUnit);
	}
}

bool CGameManager::CheckIfMouseOverTile(sf::Vector2f _inPosition)
{
	if (CSceneManager::GetTileInScene(_inPosition) != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}