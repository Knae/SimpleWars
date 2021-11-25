#include "CGameManager.h"

/// <summary>
/// Mainly calls the update function in Debug
/// and also to check if a unit is marked for death
/// in the Debug window
/// </summary>
void CGameManager::UpdateDebugWorld()
{
	m_refDebug.Update();
	CUnit* unitMarkedForDeath = m_refDebug.GetUnitToDestoy();
	if (unitMarkedForDeath != nullptr)
	{
		if (m_pSelectedUnit == unitMarkedForDeath)
		{
			m_pSelectedUnit = nullptr;
		}
		ProcessUnitAsDead(unitMarkedForDeath);
	}
	m_refDebug.DisplayWindow();
}

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
	m_eChosenFaction_Blue = CUnitEnums::FACTION::TALONS;
	m_eChosenFaction_Red = CUnitEnums::FACTION::LYNXES;
	m_bAttackOverlayShown = false;
	m_bExecutingActions = false;
	m_bWaitingForClick = false;
	m_bAIEnabled = false;

	m_uiSpawnAreaWidth = 0;
	m_uiSpawnAreaHeight = 0;
	m_uiWins_Blue = 0;
	m_uiWins_Red = 0;
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

	RecordGameStats();
}

/// <summary>
/// Initializes game manager by reading the game.ini to get the 
/// file paths of all the config files and also other
/// settings. then initializes game UI and the debug window.
/// Finally, sets the game state to Mode selection
/// </summary>
/// <returns></returns>
bool CGameManager::IntializeGame()
{
	ParseGameSettings();

	m_pFont = new sf::Font();
	if (!m_pFont->loadFromFile(m_strFontFile))
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

	m_eCurrentTurn = CUIEnums::TURN::NONE;

	m_pGameWindow->setVerticalSyncEnabled(false);
	m_pGameWindow->setFramerateLimit(m_uiWindowFrameLimit);
	CUIManager::IntializeUI(m_pGameWindow->getSize() , m_pFont,m_GameWindowSize_Current.x - 192);
	ChangeCurrentState(CUIEnums::GAMESTATE::MODE);
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

/// <summary>
/// Runs various checks and updates necessary for other managers to function.
/// eg: if end turn was clicked, if all enemies dead, for AI functionality, etc..
/// </summary>
/// <param name="_inElapsedTime"></param>
/// <returns></returns>
bool CGameManager::Update(double& _inElapsedTime)
{
	sf::Vector2f mousePosition = m_pGameWindow->mapPixelToCoords(sf::Mouse::getPosition(*(m_pGameWindow)));

	if (m_eCurrentState != CUIEnums::GAMESTATE::MODE && m_eCurrentState != CUIEnums::GAMESTATE::MAPSELECTION && m_eCurrentState != CUIEnums::GAMESTATE::GAMEEND)
	{
		if (m_bAIEnabled && m_eCurrentTurn == CUIEnums::TURN::RED /*&& m_eCurrentState == CUIEnums::GAMESTATE::GAMELOOP*/)
		{
			if (m_eCurrentState == CUIEnums::GAMESTATE::UNITPLACEMENT)
			{
				CUnitEnums::SIDE AiController = CUnitEnums::SIDE::RED;
				std::map< CUnitEnums::TYPE, std::vector<sf::Vector2u>*>& AIUnitLocations = CSceneManager::GetCurrentScene()->GetAIUnitLocations();
				for (auto& type : AIUnitLocations)
				{
					for (auto& location : (*type.second))
					{
						sf::Vector2u unitLocation = location;
						CUnit* newUnit = CUnitManager::CreateUnit(type.first, m_eChosenFaction_Red, AiController);
						CTile* targetTile = m_pSceneMgr->GetTileInScene(location);
						targetTile->UnitEntersTile(newUnit);
						newUnit->SetLocation(location);
						newUnit->SetCurrentTileType(targetTile->GetTileType());
					}
				}
				SwitchTurns();
				CUIManager::SetCurrentTurn(m_eCurrentTurn);
			}
			else if (m_eCurrentState == CUIEnums::GAMESTATE::GAMELOOP)
			{
				if (CAI_Controller::StartAITurn())
				{
					SwitchTurns();
					CUIManager::SetCurrentTurn(m_eCurrentTurn);
				}
			}
		}
		else
		{
			if (CUIManager::GetIfForfeitChosen())
			{
				switch (m_eCurrentTurn)
				{
					case CUIEnums::TURN::BLUE:
					{
						EndGame(CUIEnums::TURN::RED);
						break;
					}
					case CUIEnums::TURN::RED:
					{
						EndGame(CUIEnums::TURN::BLUE);
						break;
					}
					default:
					case CUIEnums::TURN::NONE:
					{
						std::cout << "\nERROR:Someone forfeited...but we don't know who." << std::endl;
						break;
					}
				}
			}
			else if (CUIManager::GetIfTurnEndClicked())
			{
				SwitchTurns();
				CUIManager::SetCurrentTurn(m_eCurrentTurn);
			}

			if (m_eCurrentState == CUIEnums::GAMESTATE::GAMELOOP && !CUnitManager::CheckIfAnyUnitsLeft(CParseConfigCommon::Convert(m_eCurrentTurn))/* && m_bExecutingActions*/)
			{
				//GameEnds
				std::cout << "\nAll Enemy units  have died!" << std::endl;
				EndGame(m_eCurrentTurn);
			}

			m_eCurrentUIMouseState = CUIManager::GetMouseCurrentState();

			//Updates the sidepanel based on the tile under the mouse
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
		}
	}
	else
	{
		//not in game loop
	}

	CUnitManager::Update(_inElapsedTime);

	return true;
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
		case CUIEnums::GAMESTATE::MODE:
		{
			//CSceneManager::GetCurrentScene()->ResetTiles();
			LoadScene(CSceneEnums::SCENETYPE::MAINMENU);
			CUIManager::ResetChecks();
			ClearUnitsToPlace();
			m_eCurrentTurn = CUIEnums::TURN::NONE;
			SetUIToModeSelection();
			//ChangeCurrentState(CUIEnums::GAMESTATE::FACTION);
			break;
		}
		case CUIEnums::GAMESTATE::FACTION:
		{
			//CSceneManager::GetCurrentScene()->ResetTiles();
			//CUIManager::ResetChecks();
			//ClearUnitsToPlace();
			ChangeCurrentState(CUIEnums::GAMESTATE::MAPSELECTION);
			break;
		}
		case CUIEnums::GAMESTATE::MAPSELECTION:
		{
			//CSceneManager::GetCurrentScene()->ResetTiles();
			CUIManager::ResetChecks();
			//ClearUnitsToPlace();
			//ChangeCurrentState(CUIEnums::GAMESTATE::UNITPLACEMENT);
			SetUIToMapSelection();
			break;
		}
		case CUIEnums::GAMESTATE::UNITPLACEMENT:
		{
			LoadScene(m_eCurrentSelectedMap);
			//Set up for the first player to place units
			m_eCurrentTurn = CUIEnums::TURN::BLUE;
			m_pSceneMgr->GetCurrentScene()->GetUnitsToPlace(&m_mapUnitsToPlaced_B, &m_mapUnitsToPlaced_R);
			m_pUnitsToPlace = &m_mapUnitsToPlaced_B;
			m_pSceneMgr->GetCurrentScene()->WriteSpawnAreaDetails(	CParseConfigCommon::Convert( m_eCurrentTurn),
																	m_SpawnAreaTopLeft,
																	m_uiSpawnAreaWidth,
																	m_uiSpawnAreaHeight);
			COverlayManager::InitializeOverlays(m_strUnitConfig, m_pFont, CSceneManager::GetTileSize());
			COverlayManager::ClearRangePlacementOverlay();
			COverlayManager::CreateUnitPlacementOverlay(m_SpawnAreaTopLeft,
														m_uiSpawnAreaWidth,
														m_uiSpawnAreaHeight);
			SetUIToUnitPlacement();
			break;
		}
		case CUIEnums::GAMESTATE::GAMELOOP:
		{
			if (m_bAIEnabled)
			{
				CAI_Controller::IntializeAi(CUnitManager::GetUnits_Red(),CUnitManager::GetUnits_Blue());
			}
			m_eCurrentTurn = CUIEnums::TURN::BLUE;
			SetUIToGameLoop();
			break;
		}
		case CUIEnums::GAMESTATE::GAMEEND:
		{
			COverlayManager::ClearTileOverlays();
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

/// <summary>
/// Switch turns. Runs additional functions based on current game state
/// eg: showing overlay for unit placement area, update number of units
/// to place
/// </summary>
void CGameManager::SwitchTurns()
{
	switch (m_eCurrentState)
	{
		case CUIEnums::GAMESTATE::UNITPLACEMENT:
		{
			if (m_eCurrentTurn == CUIEnums::TURN::BLUE)
			{
				//if(!m_bAIEnabled)
				m_pUnitsToPlace = &m_mapUnitsToPlaced_R;
				m_eCurrentTurn = CUIEnums::TURN::RED;
				int* infantryAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::INFANTRY)->second);
				int* tankAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::TANK)->second);
				int* artilleryAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::ARTILLERY)->second);
				CUIManager::SwitchTurnForUnitPlacment(infantryAmount, tankAmount, artilleryAmount);
				m_pSceneMgr->GetCurrentScene()->WriteSpawnAreaDetails(	CParseConfigCommon::Convert(m_eCurrentTurn),
																		m_SpawnAreaTopLeft,
																		m_uiSpawnAreaWidth,
																		m_uiSpawnAreaHeight);
				COverlayManager::ClearRangePlacementOverlay();
				COverlayManager::CreateUnitPlacementOverlay(m_SpawnAreaTopLeft,
															m_uiSpawnAreaWidth,
															m_uiSpawnAreaHeight);
			}
			else
			{
				COverlayManager::ClearRangePlacementOverlay();
				ChangeCurrentState(CUIEnums::GAMESTATE::GAMELOOP);
			}
			break;
		}
		case CUIEnums::GAMESTATE::GAMELOOP:
		{
			m_eCurrentTurn = ((m_eCurrentTurn == CUIEnums::TURN::BLUE) ? (CUIEnums::TURN::RED) : (CUIEnums::TURN::BLUE));
			CUnitManager::SwitchTurns();
			/*if (m_bAIEnabled && m_eCurrentTurn == CUIEnums::TURN::RED)
			{
				CAI_Controller::StartAITurn();
				SwitchTurns();
			}*/
			break;
		}
		case CUIEnums::GAMESTATE::MODE:
		case CUIEnums::GAMESTATE::MAPSELECTION:
		default:
		{
			break;
		}
	}
}

/// <summary>
/// Shows the corresponding victory banner
/// and updates game stats. Then moves game state to 
/// GameEnd
/// </summary>
/// <param name="_inWinSide"></param>
void CGameManager::EndGame(CUIEnums::TURN _inWinSide)
{
	m_bExecutingActions = false;
	CUIManager::VictoryAchieved(_inWinSide);
	if (_inWinSide == CUIEnums::TURN::BLUE)
	{
		m_uiWins_Blue++;
	}
	else if (_inWinSide == CUIEnums::TURN::RED)
	{
		m_uiWins_Red++;
	}
	ChangeCurrentState(CUIEnums::GAMESTATE::GAMEEND);
}

void CGameManager::DrawObject(sf::Drawable* _object)
{
}

/// <summary>
/// Clears window to black and then displays the scene and UI
/// </summary>
void CGameManager::DisplayGameWorld()
{
	m_pGameWindow -> clear(sf::Color::Black);
	DisplayScene();
	DisplayUI();
	m_pGameWindow->display();
}

/// <summary>
/// Game go boom
/// </summary>
void CGameManager::DestroyGameWorld()
{
	m_refDebug.GetWindows()->close();
	m_pGameWindow->close();
}

void CGameManager::DisplayDebug()
{
}

/// <summary>
/// Clear the record of units to place
/// </summary>
void CGameManager::ClearUnitsToPlace()
{
	m_mapUnitsToPlaced_B.clear();
	m_mapUnitsToPlaced_R.clear();
}

/// <summary>
/// Load the selected scene map or the main menu
/// </summary>
/// <param name="_inScene"></param>
/// <returns></returns>
bool CGameManager::LoadScene(CSceneEnums::SCENETYPE _inScene)
{
	std::string configPath;
	switch (_inScene)
	{
		case CSceneEnums::SCENETYPE::MAINMENU:
		{
			configPath = m_strMainMenuConfig;
			break;
		}
		case CSceneEnums::SCENETYPE::MOUNTAINVILLAGE:
		{
			configPath = m_strMountainVillageConfig;
			break;
		}
		case CSceneEnums::SCENETYPE::MOUNTAINPASS:
		{
			configPath = m_strMountainPassConfig;
			break;
		}
		case CSceneEnums::SCENETYPE::BRIDGE:
		{
			configPath = m_strBridgeConfig;
			break;
		}
		case CSceneEnums::SCENETYPE::NONE:
		default:
		{
			break;
		}
	}

	if (CSceneManager::CreateScene(configPath))
	{
		std::cout << "\nSuccesfully loaded selected map." << std::endl;
		COverlayManager::InitializeOverlays(m_strUnitConfig, m_pFont, CSceneManager::GetTileSize() );
		//ChangeCurrentState(CUIEnums::GAMESTATE::UNITPLACEMENT);
		return true;
	}
	else
	{
		std::cout << "\nUnable to selected map." << std::endl;
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

/// <summary>
/// Runs the UI manager intialize function. Must be run
/// after the scene manager is initialize.
/// </summary>
/// <returns></returns>
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
	sf::Vector2f mousePosition = m_pGameWindow->mapPixelToCoords(sf::Mouse::getPosition(*(m_pGameWindow)));
	std::cout << "\nClicking at " << mousePosition.x << "x " << mousePosition.y << "y" << std::endl;

	if (!m_bExecutingActions && !m_bWaitingForClick)
	{
		//Will get 'true' if we clicked on the side panel
		//and false if we clicked on the map instead, since this is
		//from the perspective of the UIManager
		//*outside of gameloop, true means a button was pressed. This was done
		//since there's nothing else on the window.
		int buttonIndex = 0;
		if (CUIManager::ProcessClick(mousePosition, buttonIndex))
		{
			switch (m_eCurrentState)
			{
				case CUIEnums::GAMESTATE::MODE:
				{
					if (buttonIndex == 0)
					{
						m_bAIEnabled = true;
						ChangeCurrentState(CUIEnums::GAMESTATE::MAPSELECTION);
					}
					else if (buttonIndex == 1)
					{
						m_bAIEnabled = false;
						ChangeCurrentState(CUIEnums::GAMESTATE::MAPSELECTION);
					}
					else if (buttonIndex == 2)
					{
						DestroyGameWorld();
					}
					break;
				}
				case CUIEnums::GAMESTATE::MAPSELECTION:
				{
					if (buttonIndex == 0)
					{
						m_eCurrentSelectedMap = CSceneEnums::SCENETYPE::MOUNTAINVILLAGE;
					}
					else if (buttonIndex == 1)
					{
						m_eCurrentSelectedMap = CSceneEnums::SCENETYPE::MOUNTAINPASS;
					}
					else if (buttonIndex == 2)
					{
						m_eCurrentSelectedMap = CSceneEnums::SCENETYPE::BRIDGE;
					}
					else if (buttonIndex == 3)
					{
						ChangeCurrentState(CUIEnums::GAMESTATE::MODE);
						break;
					}
					ChangeCurrentState(CUIEnums::GAMESTATE::UNITPLACEMENT);
					break;
				}
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
								COverlayManager::ClearRangePlacementOverlay();
								COverlayManager::HideAttackSelector();
								m_bAttackOverlayShown = false;
							}
							COverlayManager::ShowMoveSelector(mousePosition);
						}
						else if (m_eCurrentUIMouseState == CUIEnums::MOUSESTATE::SELECT)
						{
							if (m_bAttackOverlayShown)
							{
								COverlayManager::ClearRangePlacementOverlay();
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
							COverlayManager::ClearRangePlacementOverlay();
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
				case CUIEnums::GAMESTATE::MODE:
				{
					break;
				}
				case CUIEnums::GAMESTATE::UNITPLACEMENT:
				{
					if (COverlayManager::IsInSpawnArea(mousePosition, m_SpawnAreaTopLeft, m_uiSpawnAreaWidth, m_uiSpawnAreaHeight))
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

								if (clickedTile != nullptr && clickedTile->GetUnitOnTile() == nullptr)
								{
									
									CUnitEnums::FACTION currentPlayerFaction = (controllingPlayer == CUnitEnums::SIDE::BLUE) ? m_eChosenFaction_Blue : m_eChosenFaction_Red;
									CUnit * newUnit = CUnitManager::CreateUnit(m_eCurrentTypeChosen, currentPlayerFaction, controllingPlayer);
									clickedTile->UnitEntersTile(newUnit);
									newUnit->SetLocation(mousePosition);
									newUnit->SetCurrentTileType(clickedTile->GetTileType());

									//UpdateInfoDisplay number of units placed
									(element->second)--;

									clickedTile = nullptr;
									newUnit = nullptr;
								}
							}
						}
						else
						{
							//no unit type selected
						}
					}
					else
					{
						std::cout << "\nERROR:Trying to place unit outside spawn area" << std::endl;
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
											COverlayManager::ClearRangePlacementOverlay();
											COverlayManager::CreateRangeOverlay(currentUnitPosition, unitRange);
											m_bAttackOverlayShown = true;
										}
										else
										{
											CUIManager::SetCurrentMouseState(CUIEnums::MOUSESTATE::SELECT);
											COverlayManager::ClearRangePlacementOverlay();
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

										//If the target is destroyed, set the tile to have no
										//units. UnitManager should be doing this
										if (CUnitManager::Attack(m_pSelectedUnit, targetUnit))
										{
											std::cout << "\nTarget was destroyed!" << std::endl;
											ProcessUnitAsDead(targetUnit);
										}
										CUIManager::SetCurrentMouseState(CUIEnums::MOUSESTATE::SELECT);
										COverlayManager::ClearRangePlacementOverlay();
										m_bAttackOverlayShown = false;
										CUIManager::UpdateUI();
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
		ChangeCurrentState(CUIEnums::GAMESTATE::MODE);
		m_bAIEnabled = false;
		m_bWaitingForClick = false;
	}
	else if (m_bExecutingActions)
	{
		m_bExecutingActions = false;
	}
}

/// <summary>
/// Set the sidepanel to Mode selection
/// </summary>
void CGameManager::SetUIToModeSelection()
{
	CUIManager::SetUpModeSelectionPanel();
	CUIManager::SetCurrentGameState(m_eCurrentState);
	CUIManager::SetCurrentTurn(m_eCurrentTurn);
	CUIManager::SetToDisplayStats(m_uiWins_Blue, m_uiWins_Red);
}

/// <summary>
/// Set sidepanel to map selection
/// </summary>
void CGameManager::SetUIToMapSelection()
{
	CUIManager::SetUpMapSelection();
	CUIManager::SetCurrentGameState(m_eCurrentState);
	CUIManager::SetCurrentTurn(m_eCurrentTurn);
}

/// <summary>
/// Tell UI manager to set up the side panel
/// for placing units.
/// This HAS TO BE DONE LAST.
/// Otherwise the UI manager can't get the right numbers to display
/// </summary>
void CGameManager::SetUIToUnitPlacement()
{
	CUIManager::ClearUIElements();
	CUIManager::SetToHideStats();
	//For now there are only 3 buttons to place,
	//Infantry, tanks and artillery in that order
	int* infantryAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::INFANTRY)->second);
	int* tankAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::TANK)->second);
	int* artilleryAmount = &(m_pUnitsToPlace->find(CUnitEnums::TYPE::ARTILLERY)->second);

	CUIManager::SetUpUnitPlacementPanel(infantryAmount, tankAmount, artilleryAmount);
}

/// <summary>
/// Set to side panel to game loop mode
/// </summary>
void CGameManager::SetUIToGameLoop()
{
	CUIManager::SetUpGameLoopPanel();
}

/// <summary>
/// Calls the disply funtion of the UI manager
/// </summary>
void CGameManager::DisplayUI()
{
	CUIManager::DisplayUI(*m_pGameWindow);
}

/// <summary>
/// updates the information in the side panel
/// </summary>
/// <param name="_inViewedUnit"></param>
void CGameManager::UpdateSidePanelInfo(	CUnit* _inViewedUnit)
{
	if (m_eCurrentState == CUIEnums::GAMESTATE::UNITPLACEMENT)
	{

	}
	else if (m_eCurrentState == CUIEnums::GAMESTATE::GAMELOOP)
	{
		CUnitEnums::StatBonus_Add* selectedUnitFaction = nullptr;
		CTerrainEffects* selectedUnitTerrain = CUnitManager::ResolveTerrainEffects(CUnitEnums::TYPE::NONE, CSceneEnums::TILETYPE::NONE);
		CTerrainEffects* viewedUnitTerrain = CUnitManager::ResolveTerrainEffects(CUnitEnums::TYPE::NONE, CSceneEnums::TILETYPE::NONE);

		if (m_pSelectedUnit != nullptr)
		{
			selectedUnitTerrain = CUnitManager::ResolveTerrainEffects(m_pSelectedUnit->GetType(), m_pSelectedUnit->GetCurrentTileType());
			selectedUnitFaction = CUnitManager::GetFactionBonuses( m_pSelectedUnit->GetFaction() );
				
		}

		if(_inViewedUnit != nullptr)
		{
			viewedUnitTerrain = CUnitManager::ResolveTerrainEffects(_inViewedUnit->GetType(), _inViewedUnit->GetCurrentTileType());
		}

		CUIManager::UpdateInfoDisplay(	m_pSelectedUnit,
										_inViewedUnit,
										selectedUnitTerrain,
										viewedUnitTerrain,
										selectedUnitFaction);

		//UpdateInfoDisplay the debug window as well
		m_refDebug.UpdateInfoDisplay(m_pSelectedUnit);
	}
}

/// <summary>
/// Checks if a tile exists under the mouse
/// </summary>
/// <param name="_inPosition"></param>
/// <returns></returns>
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

/// <summary>
/// Proceses the currently unit as dead and removes it
/// from the tile's record
/// </summary>
/// <param name="_inUnit"></param>
void CGameManager::ProcessUnitAsDead(CUnit* _inUnit)
{
	_inUnit->ExplodeInFlamingGlory();
	sf::Vector2u targetTilePosition = _inUnit->GetCurrentTile();
	CSceneManager::GetTileInScene(targetTilePosition)->UnitLeavesTile();
}

/// <summary>
/// Parse the text in the game settings file into
/// usable data.
/// </summary>
void CGameManager::ParseGameSettings()
{
	std::fstream gameSettings;
	std::string currentLine;

	gameSettings.open(m_strGameConfig);
	if (gameSettings.is_open())
	{
		while (std::getline(gameSettings, currentLine))
		{
			std::string currentType;
			std::string currentValue;
			if (currentLine.compare("<WindowSize>") == 0)
			{
				//Getting the window size from config
				std::cout << "\nReading window size\n";
				std::getline(gameSettings, currentLine);
				while (currentLine.compare("</WindowSize>") != 0)
				{
					currentType = CParseConfigCommon::ParseLineGetLabel(currentLine, currentValue);
					if (currentType.compare("x")==0)
					{
						m_GameWindowSize_Current.x = std::stol(currentValue);
					}
					else if (currentType.compare("y") == 0)
					{
						m_GameWindowSize_Current.y = std::stol(currentValue);
					}
					else
					{
						std::cout << "\nWarning:Unrecognised window size label" << std::endl;
					}
					std::getline(gameSettings, currentLine);
				}
			}
			else if (currentLine.compare("<FrameLimit>") == 0)
			{
				//Getting the frame limit
				std::cout << "\nReading frame limit\n";
				std::getline(gameSettings, currentLine);
				while (currentLine.compare("</FrameLimit>") != 0)
				{
					 currentType = CParseConfigCommon::ParseLineGetLabel(currentLine, currentValue);
					 if (currentType.compare("Limit") == 0)
					 {
						 m_uiWindowFrameLimit = std::stol( currentValue);
					 }
					 std::getline(gameSettings, currentLine);
				}
			}
			else if (currentLine.compare("<Font>") == 0)
			{
				//Getting the Font file
				std::cout << "\nReading Font file location\n";
				std::getline(gameSettings, currentLine);
				while (currentLine.compare("</Font>") != 0)
				{
					currentType = CParseConfigCommon::ParseLineGetLabel(currentLine, currentValue);
					if (currentType.compare("Path") == 0)
					{
						m_strFontFile = currentValue;
					}
					std::getline(gameSettings, currentLine);
				}
			}
			else if (currentLine.compare("<ConfigLocation>") == 0)
			{
				//Getting the Font file
				std::cout << "\nReading config file locations\n";
				std::getline(gameSettings, currentLine);
				while (currentLine.compare("</ConfigLocation>") != 0)
				{
					currentType = CParseConfigCommon::ParseLineGetLabel(currentLine, currentValue);
					if (currentType.compare("Unit") == 0)
					{
						m_strUnitConfig = currentValue;
					}
					else if (currentType.compare("Faction") == 0)
					{
						m_strFactionConfig = currentValue;
					}
					else if (currentType.compare("MainMenu") == 0)
					{
						m_strMainMenuConfig = currentValue;
					}
					else if (currentType.compare("MountainVillage") == 0)
					{
						m_strMountainVillageConfig = currentValue;
					}
					else if (currentType.compare("MountainPass") == 0)
					{
						m_strMountainPassConfig = currentValue;
					}
					else if (currentType.compare("Bridge") == 0)
					{
						m_strBridgeConfig = currentValue;
					}
					else if (currentType.compare("Bridge") == 0)
					{
						m_strGameStatsConfig = currentValue;
					}
					std::getline(gameSettings, currentLine);
				}
			}

			std::getline(gameSettings, currentLine);
		}
	}

	ParseGameStats();
}

/// <summary>
/// Parse the gamestats.ini into usable data
/// </summary>
void CGameManager::ParseGameStats()
{
	std::fstream gameStats;

	gameStats.open(m_strGameStatsConfig);
	if (gameStats.is_open())
	{
		std::string currentLine;
		std::string currentLabel;
		std::string currentValue;
		while (std::getline(gameStats, currentLine))
		{
			currentLabel = CParseConfigCommon::ParseLineGetLabel(currentLine,currentValue);
			if (currentLabel.compare("Red") == 0)
			{
				m_uiWins_Red = std::stol(currentValue);
			}
			else if(currentLabel.compare("Blue") == 0)
			{
				m_uiWins_Blue = std::stol(currentValue);
			}
		}
	}
	else
	{
		m_uiWins_Blue = 0;
		m_uiWins_Red = 0;
	}
}

/// <summary>
/// Write the gamestats into file
/// </summary>
void CGameManager::RecordGameStats()
{
	std::ofstream gameStats;

	gameStats.open(m_strGameStatsConfig, std::ios::trunc);
	if (gameStats.is_open())
	{
		std::string currentLine;

		gameStats << "Blue=" << m_uiWins_Blue << std::endl;
		gameStats << "Red=" << m_uiWins_Red << std::endl;

	}
}
