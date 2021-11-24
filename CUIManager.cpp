#include "CUIManager.h"
#include "CUnit.h"
#include "CTerrainEffects.h"

std::vector<CButton*> CUIManager::m_vecCustomButtons;
std::vector<sf::Sprite*> CUIManager::m_vecButtons_ControlPanel;
std::vector<sf::Sprite*> CUIManager::m_vecOverlays;
std::vector<sf::Text*> CUIManager::m_vecText_ControlPanel;
std::vector<int*> CUIManager::m_vecText_DisplayVariables;
sf::RenderTexture* CUIManager::m_pPanelBackground;
sf::Texture* CUIManager::m_pEmblemTexture;
sf::Texture* CUIManager::m_pButtonUnitTexture;
sf::Texture* CUIManager::m_pEmptyUnitPortrait;
sf::Texture* CUIManager::m_pButtonsGameLoop;
sf::Texture* CUIManager::m_pButtonFinish;
sf::Font* CUIManager::m_pFont;
sf::Sprite* CUIManager::m_pSpriteBackground;
sf::Texture CUIManager::m_VictoryTexture;
sf::Sprite CUIManager::m_VictorySprite;
sf::IntRect CUIManager::m_VictoryRect_Blue;
sf::IntRect CUIManager::m_VictoryRect_Red;
unsigned int CUIManager::m_uSceneWidth;
bool CUIManager::m_bUnitControllable;
bool CUIManager::m_bUnitHasNoMovePoints;
bool CUIManager::m_bUnitHasAttacked;
bool CUIManager::m_bEndTurn;
bool CUIManager::m_bForfeitChosen;
bool CUIManager::m_bDisplayVictory;
bool CUIManager::m_bDisplayInfoText;

CUIEnums::TURN CUIManager::m_eCurrentTurn;
CUIEnums::GAMESTATE CUIManager::m_eCurrentUIState;
CUIEnums::MOUSESTATE CUIManager::m_eCurrentMouseState;
CUnitEnums::TYPE CUIManager::m_eCurrentTypeChosen;
CUnitEnums::SIDE CUIManager::m_eCurrentUnitSide;
CInfoTextDisplay CUIManager::m_Info_FactionBonus;
CInfoTextDisplay CUIManager::m_Info_UnitStats;
CInfoTextDisplay CUIManager::m_Info_OccupiedTerrain;
CInfoTextDisplay CUIManager::m_Info_ViewedTerrain;

const CUnitEnums::TYPE CUIManager::m_eUnitOnButton[]= {	CUnitEnums::TYPE::INFANTRY,
														CUnitEnums::TYPE::TANK,
														CUnitEnums::TYPE::ARTILLERY,
														CUnitEnums::TYPE::NONE };

const std::string CUIManager::m_strUnitButtonSpriteMap("assets/spritemaps/UnitButtons.png");
const std::string CUIManager::m_strEmptyUnitSprite("assets/spritemaps/EmptyUnit.png");
const std::string CUIManager::m_strEmblemSpriteMap("assets/spritemaps/FactionEmblems.png");
const std::string CUIManager::m_strGameButtonsSpriteMap("assets/spritemaps/GameButtons.png");
const std::string CUIManager::m_strTileSelectorSpriteMap("assets/spritemaps/tileSelection.png");
const std::string CUIManager::m_strFinishButtonSprite("assets/spritemaps/FinishButton.png");
const std::string CUIManager::m_strVictorySprite("assets/spritemaps/DisplaysVictory.png");
const sf::IntRect CUIManager::m_ButtonUnitRect_Blue({ 0, 0, 32, 32 });
const sf::IntRect CUIManager::m_ButtonUnitRect_Red({ 0, 32, 32, 32 });
const sf::IntRect CUIManager::m_ButtonFactionEmblem({ 0, 0, 32, 32 });
const sf::IntRect CUIManager::m_ButtonGameLoop({ 0, 0, 64, 32 });

CUIManager::CUIManager()
{
	m_pPanelBackground = nullptr;
	m_pSpriteBackground = nullptr;
	m_pEmblemTexture = nullptr;
	m_pButtonUnitTexture = nullptr;
	m_pEmptyUnitPortrait = nullptr;
	m_pButtonsGameLoop = nullptr;
	m_pFont = nullptr;
	m_uSceneWidth = 0;
	m_bUnitControllable = false;
	m_bUnitHasNoMovePoints = false;
	m_bUnitHasAttacked = false;
	m_bEndTurn = false;
	m_bDisplayVictory = false;
	m_bDisplayInfoText = false;
	m_eCurrentTurn = CUIEnums::TURN::BLUE;
	m_eCurrentUnitSide = CUnitEnums::SIDE::NONE;
}

CUIManager::~CUIManager()
{
	ClearUIElements();

	delete m_pPanelBackground;
	delete m_pSpriteBackground;
	delete m_pEmblemTexture;
	delete m_pButtonUnitTexture;
	delete m_pEmptyUnitPortrait;
	delete m_pButtonsGameLoop;
	delete m_pButtonFinish;

	m_pPanelBackground = nullptr;
	m_pSpriteBackground = nullptr;
	m_pEmblemTexture = nullptr;
	m_pButtonUnitTexture = nullptr;
	m_pEmptyUnitPortrait = nullptr;
	m_pButtonsGameLoop = nullptr;
	m_pButtonFinish = nullptr;
}

/// <summary>
/// Initialize all the pointers and size of the UI side panel
/// </summary>
/// <param name="_inWindowSize"></param>
/// <param name="_inFont"></param>
/// <param name="_inSceneWidth"></param>
/// <returns></returns>
bool CUIManager::IntializeUI(sf::Vector2u _inWindowSize, sf::Font* _inFont, const unsigned int _inSceneWidth )
{
	if (m_pPanelBackground != nullptr)
	{
		delete m_pPanelBackground;
		m_pPanelBackground = nullptr;
	}

	if (m_pSpriteBackground != nullptr)
	{
		delete m_pSpriteBackground;
		m_pSpriteBackground = nullptr;
	}

	if (m_pEmblemTexture != nullptr)
	{
		delete m_pEmblemTexture;
		m_pEmblemTexture = nullptr;
	}

	if (m_pButtonUnitTexture != nullptr)
	{
		delete m_pButtonUnitTexture;
		m_pButtonUnitTexture = nullptr;
	}

	if (m_pButtonFinish != nullptr)
	{
		delete m_pButtonFinish;
		m_pButtonFinish = nullptr;
	}

	if (m_pButtonsGameLoop != nullptr )
	{
		delete m_pButtonsGameLoop;
		m_pButtonsGameLoop = nullptr;
	}

	if (m_pEmptyUnitPortrait != nullptr)
	{
		delete m_pEmptyUnitPortrait;
		m_pEmptyUnitPortrait = nullptr;
	}

	m_pEmblemTexture = new sf::Texture;
	if (!m_pEmblemTexture->loadFromFile(m_strEmblemSpriteMap))
	{
		std::cout << "\nUnable to initialize textures for faction emblems.\n" << std::endl;
		return false;
	}

	m_pButtonUnitTexture = new sf::Texture;
	if (!m_pButtonUnitTexture->loadFromFile(m_strUnitButtonSpriteMap))
	{
		std::cout << "\nUnable to initialize textures for unit buttons.\n" << std::endl;
		return false;
	}

	m_pEmptyUnitPortrait = new sf::Texture;
	if (!m_pEmptyUnitPortrait->loadFromFile(m_strEmptyUnitSprite))
	{
		std::cout << "\nUnable to initialize textures for unit buttons.\n" << std::endl;
		return false;
	}

	m_pButtonFinish = new sf::Texture;
	if (!m_pButtonFinish->loadFromFile(m_strFinishButtonSprite))
	{
		std::cout << "\nUnable to initialize textures for unit buttons.\n" << std::endl;
		return false;
	}
	
	m_pButtonsGameLoop = new sf::Texture;
	if (!m_pButtonsGameLoop->loadFromFile(m_strGameButtonsSpriteMap))
	{
		std::cout << "\nUnable to initialize textures for unit buttons.\n" << std::endl;
		return false;
	}

	if (!m_VictoryTexture.loadFromFile(m_strVictorySprite))
	{
		std::cout << "\nUnable to initialize textures victory display.\n" << std::endl;
		return false;
	}
	m_VictoryRect_Blue = sf::IntRect(0, 0, 640, 192);
	m_VictoryRect_Red = sf::IntRect(640, 0, 640, 192);
	m_VictorySprite.setTexture(m_VictoryTexture);
	m_VictorySprite.setTextureRect(m_VictoryRect_Blue);
	m_VictorySprite.setPosition(sf::Vector2f(96.0f,96.0f));

	m_uSceneWidth = _inSceneWidth;
	m_pPanelBackground = new sf::RenderTexture();
	m_pPanelBackground->create(_inWindowSize.x- m_uSceneWidth, _inWindowSize.y);
	m_pPanelBackground->clear(sf::Color::Black);

	m_pSpriteBackground = new sf::Sprite();
	m_pSpriteBackground->setTexture(m_pPanelBackground->getTexture(), true);
	m_pSpriteBackground->setPosition((float)(m_uSceneWidth),0.0f);

	m_pFont = _inFont;

	m_Info_FactionBonus.Initialize(m_uSceneWidth + 96.0f, 0.0f, m_pFont, 2);

	m_Info_UnitStats.Initialize(m_uSceneWidth + 96.0f, 96.0f, m_pFont, 5);
	m_Info_UnitStats.SetTextAtIndex(0, "UnitStats", "");
	m_Info_UnitStats.SetTextAtIndex(1, "HP :", "N/A");
	m_Info_UnitStats.SetTextAtIndex(2, "MOV:", "N/A");
	m_Info_UnitStats.SetTextAtIndex(3, "DMG:", "N/A");
	m_Info_UnitStats.SetTextAtIndex(4, "RGE:", "N/A");

	m_Info_OccupiedTerrain.Initialize(m_uSceneWidth * 1.0f, 192.0f, m_pFont, 5);
	m_Info_OccupiedTerrain.SetTextAtIndex(0, "UnitTerrBonus", "");
	m_Info_OccupiedTerrain.SetTextAtIndex(1, "MOV:", "N/A");
	m_Info_OccupiedTerrain.SetTextAtIndex(2, "DMG->", "N/A");
	m_Info_OccupiedTerrain.SetTextAtIndex(3, "DMG<-", "N/A");
	m_Info_OccupiedTerrain.SetTextAtIndex(4, "RGE:", "N/A");

	m_Info_ViewedTerrain.Initialize(m_uSceneWidth + 96.0f, 192.0f, m_pFont, 5);
	m_Info_ViewedTerrain.SetTextAtIndex(0, "", "N/A");
	m_Info_ViewedTerrain.SetTextAtIndex(1, "MOV:", "N/A");
	m_Info_ViewedTerrain.SetTextAtIndex(2, "DMG->", "N/A");
	m_Info_ViewedTerrain.SetTextAtIndex(3, "DMG<-", "N/A");
	m_Info_ViewedTerrain.SetTextAtIndex(4, "RGE:", "N/A");

	return true;
}

/// <summary>
/// UpdateInfoDisplay the visual elements in the control panel.
/// </summary>
void CUIManager::UpdateUI()
{
	if (m_eCurrentUnitSide == CParseConfigCommon::Convert(m_eCurrentTurn))
	{
		m_bUnitControllable = true; 
	}
	else
	{
		m_bUnitControllable = false;
	}

	if (m_eCurrentTurn == CUIEnums::TURN::BLUE)
	{
		m_pPanelBackground->clear(sf::Color::Blue);
	}
	else if (m_eCurrentTurn == CUIEnums::TURN::RED)
	{
		m_pPanelBackground->clear(sf::Color::Red);
	}
	else
	{
		m_pPanelBackground->clear(sf::Color::Black);
	}

	switch (m_eCurrentUIState)
	{
		case CUIEnums::GAMESTATE::UNITPLACEMENT:
		{
			for (unsigned short i = 0; i < 3; i++)
			{
				m_vecText_ControlPanel[i]->setString(std::to_string(*m_vecText_DisplayVariables[i]) + " unit(s) left\nto place");
			}

			int offsetArray[3] = { 0 };
			switch (m_eCurrentTypeChosen)
			{
				case CUnitEnums::TYPE::INFANTRY:
				{
					offsetArray[0] = 96;
					break;
				}
				case CUnitEnums::TYPE::TANK:
				{
					offsetArray[1] = 96;
					break;
				}
				case CUnitEnums::TYPE::ARTILLERY:
				{
					offsetArray[2] = 96;
					break;
				}
				default:
				{
					break;
				}
			}
			sf::IntRect currentRect;
			for (int i = 0; i < 3; i++)
			{
				currentRect = (m_eCurrentTurn==CUIEnums::TURN::BLUE)?m_ButtonUnitRect_Blue: m_ButtonUnitRect_Red;
				currentRect.left += (i * 32) + offsetArray[i];
				m_vecButtons_ControlPanel[i]->setTextureRect(currentRect);
			}
			break;
		}
		case CUIEnums::GAMESTATE::GAMELOOP:
		{
			//If the mouseState is moving unit and unit has no move points; OR
			//if the mouseState is attacking a target and unit has already attacked;
			//then set mouseState back to selecting a unit
			if ((m_bUnitHasNoMovePoints && m_eCurrentMouseState == CUIEnums::MOUSESTATE::MOVE) ||
				(m_bUnitHasAttacked		&& m_eCurrentMouseState == CUIEnums::MOUSESTATE::ATTACK) )
			{
				m_eCurrentMouseState = CUIEnums::MOUSESTATE::SELECT;
			}


			int offsetArray[2] = { 0 };
			sf::IntRect currentRect(0, 0, 0, 0);
			switch (m_eCurrentMouseState)
			{
				case CUIEnums::MOUSESTATE::MOVE:
				{
					offsetArray[0] = (!m_bUnitControllable || m_bUnitHasAttacked) ? 64 : 0;
					offsetArray[1] = 32;
					break;
				}
				case CUIEnums::MOUSESTATE::ATTACK:
				{
					offsetArray[0] = 32;
					offsetArray[1] = (!m_bUnitControllable || m_bUnitHasNoMovePoints) ? 64 : 0;
					break;
				}
				case CUIEnums::MOUSESTATE::NONE:
				case CUIEnums::MOUSESTATE::SELECT:
				default:
				{
					offsetArray[0] = (!m_bUnitControllable || m_bUnitHasAttacked) ? 64 : 0;
					offsetArray[1] = (!m_bUnitControllable || m_bUnitHasNoMovePoints) ? 64 : 0;
					break;
				}
			}

			for (int i = 0; i < 2; i++)
			{
				currentRect = m_ButtonGameLoop;
				currentRect.left += (i * 64);
				currentRect.top += offsetArray[i];
				m_vecButtons_ControlPanel[i]->setTextureRect(currentRect);
			}
			break;
		}
		default:
		{
			break;
		}
	}	
}

/// <summary>
/// UpdateInfoDisplay and then display the UI elements
/// </summary>
/// <param name="_inWindow"></param>
void CUIManager::DisplayUI(sf::RenderWindow& _inWindow)
{
	UpdateUI();
	_inWindow.draw(*m_pSpriteBackground);

	for (auto& element : m_vecButtons_ControlPanel)
	{
		_inWindow.draw(*element);
	}

	for (auto& element : m_vecText_ControlPanel)
	{
		_inWindow.draw(*element);
	}
	
	if (m_bDisplayVictory)
	{
		_inWindow.draw(m_VictorySprite);
	}

	for (auto& button : m_vecCustomButtons)
	{
		button->Display(_inWindow);
	}

	if (m_bDisplayInfoText)
	{
		m_Info_FactionBonus.DisplayInfo(_inWindow);
		m_Info_UnitStats.DisplayInfo(_inWindow);
		m_Info_OccupiedTerrain.DisplayInfo(_inWindow);
		m_Info_ViewedTerrain.DisplayInfo(_inWindow);
	}
}

/// <summary>
/// Empty all vectors containing UI elements such as
/// buttons and text
/// </summary>
void CUIManager::ClearUIElements()
{
	for (auto& element : m_vecCustomButtons)
	{
		delete element;
		element = nullptr;
	}

	for (auto& element : m_vecButtons_ControlPanel)
	{
		delete element;
		element = nullptr;
	}

	for (auto& element : m_vecText_ControlPanel)
	{
		delete element;
		element = nullptr;
	}

	for (auto& element : m_vecOverlays)
	{
		delete element;
		element = nullptr;
	}

	m_vecCustomButtons.clear();
	m_vecButtons_ControlPanel.clear();
	m_vecText_ControlPanel.clear();
	m_vecOverlays.clear();

	m_bDisplayInfoText = false;
}

/// <summary>
/// Process the clicks to determine whether a function in the side panel
/// is being called.
/// </summary>
/// <param name="_inCoords"></param>
/// <returns>Returns true if click is in the side panel</returns>
bool CUIManager::ProcessClick(sf::Vector2f& _inCoords, int& _outIndex)
{
	//If the mouse is this far right, then it's not hovering over any tiles, but is over the
	//side panel
	if (_inCoords.x > m_uSceneWidth)
	{
		int buttonClicked = ProcessClickInCtrlPanel(_inCoords);
		switch (m_eCurrentUIState)
		{
			case CUIEnums::GAMESTATE::MODE:
			{
				return GetCButtonClicked(_inCoords, _outIndex);
				break;
			}
			case CUIEnums::GAMESTATE::UNITPLACEMENT:
			{
				if (buttonClicked != 3)
				{
					m_eCurrentTypeChosen = m_eUnitOnButton[buttonClicked];
				}
				//Mark to end turn if all units have been placed
				else if((*m_vecText_DisplayVariables[0] + *m_vecText_DisplayVariables[1] + *m_vecText_DisplayVariables[2]) == 0)
				{
					m_bEndTurn = true;
				}

				break;
			}
			case CUIEnums::GAMESTATE::GAMELOOP:
			{
				if (buttonClicked < 4)
				{
					switch (buttonClicked)
					{
						//This is assumming Controllable is only set when there's a selected unit
						case 0:
						{
							m_eCurrentMouseState = (m_bUnitControllable) ? CUIEnums::MOUSESTATE::ATTACK : CUIEnums::MOUSESTATE::SELECT;
							break;
						}
						case 1:
						{
							m_eCurrentMouseState = (m_bUnitControllable) ? CUIEnums::MOUSESTATE::MOVE : CUIEnums::MOUSESTATE::SELECT;
							break;
						}
						case 2:
						{
							m_bEndTurn = true;
							break;
						}
						case 3:
						{
							m_bForfeitChosen = true;
							m_eCurrentUIState = CUIEnums::GAMESTATE::GAMEEND;
							break;
						}
						default:
						{
							m_eCurrentMouseState = CUIEnums::MOUSESTATE::SELECT;
							break; 
						}
					}
				}
			}
			default:
			{
				break;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
	//return true;
}

/// <summary>
/// Determines which button in the control panel is clicked
/// </summary>
/// <param name="_inCoords">Vector2f - assumed to be the coordinates of the mouse</param>
/// <returns>the index corresponding to the button clicked</returns>
int CUIManager::ProcessClickInCtrlPanel(sf::Vector2f& _inCoords)
{
	for (unsigned short i = 0; i < m_vecButtons_ControlPanel.size(); i++)
	{

		if ((m_vecButtons_ControlPanel[i])->getGlobalBounds().contains(_inCoords))
		{

			return i;
		}
	}
	return -1;
}

bool CUIManager::GetCButtonClicked(sf::Vector2f _inPosition, int& _outButtonIndex)
{
	for (size_t i=0; i<m_vecCustomButtons.size();i++)
	{
		if (m_vecCustomButtons[i]->CheckIfClicked(_inPosition))
		{
			_outButtonIndex = i;
			return true;
		}
	}
	_outButtonIndex = 0;
	return false;
}

void CUIManager::SetUpModeSelectionPanel()
{
	ClearUIElements();
	sf::Text* currentText = nullptr;
	CButton* newButton = nullptr;

	currentText = new sf::Text;
	currentText->setFont(*m_pFont);
	currentText->setCharacterSize(24);
	currentText->setFillColor(sf::Color::White);
	currentText->setPosition(m_uSceneWidth + 32.0f,8.0f);
	m_vecText_ControlPanel.push_back(currentText);

	newButton = new CButton;
	newButton->Initialize(std::string("Solo"), m_pFont, std::string("1 Player"), sf::Vector2f(m_uSceneWidth + 32.0f, 128.0f), 32);
	m_vecCustomButtons.push_back(newButton);

	newButton = new CButton;
	newButton->Initialize(std::string("Vs"), m_pFont, std::string("2 Players"), sf::Vector2f(m_uSceneWidth + 32.0f, 208), 32);
	m_vecCustomButtons.push_back(newButton);

	newButton = new CButton;
	newButton->Initialize(std::string("Exit"), m_pFont, std::string("Exit"), sf::Vector2f(m_uSceneWidth + 32.0f, 288.0f), 32);
	m_vecCustomButtons.push_back(newButton);

	newButton = nullptr;
}

/// <summary>
/// Create the button in the panel for placing units
/// According to current sprite sheet:
/// 0 = Infantry
/// 1 = Tank
/// 2 = Artillery
/// 
/// thus the finish button will be the fourth
/// TODO: Separate them into separate sprite objects in a map
///		Then we can be certain even if the images are changed or the
///		the order is changed, it'll still be what we expect it to be.
/// </summary>
void CUIManager::SetUpUnitPlacementPanel(int* _inAmountA, int* _inAmountB, int* _inAmountC)
{
	ClearUIElements();
	sf::Sprite* currentButton = nullptr;
	sf::Text* currentButtonText = nullptr;
	sf::Text* currentText = nullptr;
	sf::IntRect currentRect;

	m_vecText_DisplayVariables.clear();
	m_vecText_DisplayVariables.push_back(_inAmountA);
	m_vecText_DisplayVariables.push_back(_inAmountB);
	m_vecText_DisplayVariables.push_back(_inAmountC);

	for (unsigned short  i = 0; i < 3; i++)
	{
		currentButton = new sf::Sprite;
		currentButton->setTexture(*m_pButtonUnitTexture);
		currentRect = m_ButtonUnitRect_Blue;
		currentRect.left += (i * 32);
		currentButton->setTextureRect(currentRect);
		currentButton->setPosition( sf::Vector2f(m_uSceneWidth+16.0f,288.0f+(i*48)) );
		m_vecButtons_ControlPanel.push_back(currentButton);

		currentButtonText = new sf::Text( std::to_string(*m_vecText_DisplayVariables[i]) + " unit(s) left\nto place", *m_pFont );
		currentButtonText->setFillColor(sf::Color::White);
		currentButtonText->setCharacterSize(16);
		currentButtonText->setPosition(sf::Vector2f(m_uSceneWidth + 64.0f, 288.0f + (i * 48)));
		m_vecText_ControlPanel.push_back(currentButtonText);
	}

	currentButton = new sf::Sprite;
	currentButton->setTexture(*m_pButtonFinish);
	currentRect = m_ButtonUnitRect_Blue; 
	currentButton->setPosition(sf::Vector2f(m_uSceneWidth + 48.0f, 432));
	m_vecButtons_ControlPanel.push_back(currentButton);

	currentText = new sf::Text("Blue Player",*m_pFont);
	currentText->setFillColor(sf::Color::White);
	currentText->setCharacterSize(16);
	currentText->setPosition(sf::Vector2f(m_uSceneWidth + 64.0f, 16.0f));
	m_vecText_ControlPanel.push_back(currentText);

	currentText = new sf::Text("Click a unit to select it,\nthen click a tile to place it.\nClick 'Finish' when done", *m_pFont);
	currentText->setFillColor(sf::Color::White);
	currentText->setCharacterSize(16);
	currentText->setPosition(sf::Vector2f((float)(m_uSceneWidth), 464.0f));
	m_vecText_ControlPanel.push_back(currentText);

	m_eCurrentUIState = CUIEnums::GAMESTATE::UNITPLACEMENT;
	m_eCurrentTurn = CUIEnums::TURN::BLUE;

	currentButton = nullptr;
	currentButtonText = nullptr;
	currentText = nullptr;
}

/// <summary>
/// Create buttons for game loop panel
/// According to current sprite sheet:
/// 0 = Attack
/// 1 = Move
/// 2 = End Turn
/// 3 = Forfeit
/// 4 = Faction Emblem
/// 5 = Faction Bonuses
/// 6 = Selected unit portrait
/// 7 = Selected unit stats
/// 8 = Tile under selected unit
/// 9 = Tile under pointer
/// 
/// 'buttons' 4-9 are info displays
/// </summary>
void CUIManager::SetUpGameLoopPanel()
{
	ClearUIElements();
	sf::Sprite* currentButton = nullptr;
	sf::Text* currentText = nullptr;
	sf::IntRect currentRect;

	m_vecText_DisplayVariables.clear();

	for (unsigned short  i = 0; i < 2; i++)
	{
		currentButton = new sf::Sprite;
		currentButton->setTexture(*m_pButtonsGameLoop);
		currentRect = m_ButtonGameLoop;
		currentRect.left += (i * 64);
		currentButton->setTextureRect(currentRect);
		currentButton->setPosition(sf::Vector2f(m_uSceneWidth + 32.0f +(i*64.0f), 352.0f));
		m_vecButtons_ControlPanel.push_back(currentButton);
	}

	for (unsigned short  i = 0; i < 2; i++)
	{
		currentButton = new sf::Sprite;
		currentButton->setTexture(*m_pButtonsGameLoop);
		currentRect = m_ButtonGameLoop;
		currentRect.left += 128 + (i * 64);
		currentButton->setTextureRect(currentRect);
		currentButton->setPosition(sf::Vector2f(m_uSceneWidth + (i * 128.0f),400.0f));
		m_vecButtons_ControlPanel.push_back(currentButton);
	}

	for (unsigned short i = 0; i < 3; i++)
	{
		currentButton = new sf::Sprite;
		currentButton->setTexture(*m_pEmptyUnitPortrait);
		currentRect = m_ButtonUnitRect_Blue;
		currentButton->setTextureRect(currentRect);
		currentButton->setScale(3.0f, 3.0f);
		currentButton->setPosition(sf::Vector2f((float)(m_uSceneWidth ), 0.0f + (i*96.0f) ));
		m_vecButtons_ControlPanel.push_back(currentButton);

		currentButton = new sf::Sprite;
		currentButton->setTexture(*m_pEmptyUnitPortrait);
		currentRect = m_ButtonUnitRect_Blue;
		currentButton->setTextureRect(currentRect);
		currentButton->setScale(3.0f, 3.0f);
		currentButton->setPosition(sf::Vector2f((float)(m_uSceneWidth + 96.0f), 0.0f + (i * 96.0f) ));
		m_vecButtons_ControlPanel.push_back(currentButton);
	}

	m_eCurrentUIState = CUIEnums::GAMESTATE::GAMELOOP;
	m_eCurrentTurn = CUIEnums::TURN::BLUE;
	m_eCurrentMouseState = CUIEnums::MOUSESTATE::SELECT;

	m_bDisplayInfoText = true;
	currentButton = nullptr;
	currentText = nullptr;
}

/// <summary>
/// Updates the top section of the control panel with info on the
/// selected unit. UIManager also records relevant info like the 
/// unit's controlling player (side).
/// Inputs are expected to be based on the tile the unit is on.
/// </summary>
/// <param name="_inTerrain">Tile type. CSceneEnums::TILETYPE</param>
/// <param name="_inSide">Controlling player. CUnitEnums::SIDE</param>
/// <param name="_inType">Unit Type. CUnitEnums::TYPE</param>
/// <param name="_inFaction">Faction. CUnitEnums::FACTION</param>
/// <returns></returns>
bool CUIManager::UpdateInfoDisplay(	CUnit* _inSelectedUnit,
									CUnit* _inViewedUnit,
									CTerrainEffects* _inSelectedUnitTerrain,
									CTerrainEffects* _inViewedUnitTerrain//,
									/*CUnitEnums::SIDE _inSide = CUnitEnums::SIDE::NONE,*/
									/*CUnitEnums::FACTION _inFaction = CUnitEnums::FACTION::NONE*/)
{
	auto ConvertToString = [](auto input) {std::stringstream str; str << std::setprecision(1) << std::fixed << input; return str.str(); };

	if(_inSelectedUnit!=nullptr)
	{
		m_eCurrentUnitSide = _inSelectedUnit->GetSide();
		CUnitEnums::TYPE selectedUnitType = _inSelectedUnit->GetType();
		CUnitEnums::FACTION selectUnitFaction = _inSelectedUnit->GetFaction();

		if (selectedUnitType != CUnitEnums::TYPE::NONE)
		{
			m_vecButtons_ControlPanel[6]->setTexture(*m_pButtonUnitTexture);
			sf::IntRect currentRect;

			switch (m_eCurrentUnitSide)
			{
				case CUnitEnums::SIDE::BLUE:
				{
					currentRect = m_ButtonUnitRect_Blue;
					break;
				}
				case CUnitEnums::SIDE::RED:
				{
					currentRect = m_ButtonUnitRect_Red;
					break;
				}
				default:
				{
					break;
				}
			}

			switch (selectedUnitType)
			{
				case CUnitEnums::TYPE::INFANTRY:
				{
					break;
				}
				case CUnitEnums::TYPE::TANK:
				{
					currentRect.left += 32;
					break;
				}
				case CUnitEnums::TYPE::ARTILLERY:
				{
					currentRect.left += 64;
					break;
				}
				default:
				{
					break;
				}
			}
			m_vecButtons_ControlPanel[6]->setTextureRect(currentRect);

			currentRect = m_ButtonFactionEmblem;
			switch (selectUnitFaction)	
			{
				case CUnitEnums::FACTION::TALONS:
				{
					break;
				}
				case CUnitEnums::FACTION::URSINE:
				{
					currentRect.left += 32;
					break;
				}
				case CUnitEnums::FACTION::LYNXES:
				{
					currentRect.left += 64;
					break;
				}
				case CUnitEnums::FACTION::NONE:
				default:
					break;
			}

			m_vecButtons_ControlPanel[4]->setTexture(*m_pEmblemTexture);
			m_vecButtons_ControlPanel[4]->setTextureRect(currentRect);
		}
		else
		{
			m_vecButtons_ControlPanel[4]->setTexture(*m_pEmptyUnitPortrait);
			m_vecButtons_ControlPanel[4]->setTextureRect(m_ButtonUnitRect_Blue);

			m_vecButtons_ControlPanel[6]->setTexture(*m_pEmptyUnitPortrait);
			m_vecButtons_ControlPanel[6]->setTextureRect(m_ButtonUnitRect_Blue);
		}

		//UpdateInfoDisplay unit stats if applicable
		float valueToUpdate = 0.0f;
		float value2 = 0.0f;
		int intToUpdate = 0;
		std::string stringToDisplay;

		//=============================
		//Updating Unit stats display
		//=============================
		valueToUpdate = _inSelectedUnit->GetHP();
		stringToDisplay = ConvertToString(valueToUpdate);
		m_Info_UnitStats.SetValueDisplayAtIndex(1, stringToDisplay);
		//[Value2->maxMovePoints, ValueToUpdate->CurrentMovePoints]//
		_inSelectedUnit->GetMovementStat(value2, valueToUpdate);
		stringToDisplay = ConvertToString(valueToUpdate) + "/" + ConvertToString(value2);
		m_Info_UnitStats.SetValueDisplayAtIndex(2, stringToDisplay);
		valueToUpdate = _inSelectedUnit->GetDamageDealt();
		stringToDisplay = ConvertToString(valueToUpdate);
		m_Info_UnitStats.SetValueDisplayAtIndex(3, stringToDisplay);
		intToUpdate = _inSelectedUnit->GetRange();
		stringToDisplay = ConvertToString(intToUpdate);
		m_Info_UnitStats.SetValueDisplayAtIndex(4, stringToDisplay);

		//======================================
		//Updating Ocupied Terrain Modifiers
		//======================================
		if (_inSelectedUnitTerrain != nullptr)
		{
			valueToUpdate = _inSelectedUnitTerrain->GetModifierMovement();
			stringToDisplay = ConvertToString(valueToUpdate);
			m_Info_OccupiedTerrain.SetValueDisplayAtIndex(1, stringToDisplay);
			valueToUpdate = _inSelectedUnitTerrain->GetModifierDamageDealt();
			stringToDisplay = " x" + ConvertToString(valueToUpdate);
			m_Info_OccupiedTerrain.SetValueDisplayAtIndex(2, stringToDisplay);
			valueToUpdate = _inSelectedUnitTerrain->GetModifierDamageTaken();
			stringToDisplay = " x" + ConvertToString(valueToUpdate);
			m_Info_OccupiedTerrain.SetValueDisplayAtIndex(3, stringToDisplay);
			intToUpdate = _inSelectedUnitTerrain->GetRangeOffset();
			stringToDisplay = ConvertToString(intToUpdate);
			m_Info_OccupiedTerrain.SetValueDisplayAtIndex(4, stringToDisplay);
		}
		else
		{
			m_Info_OccupiedTerrain.SetValueDisplayAtIndex(1, "N/A");
			m_Info_OccupiedTerrain.SetValueDisplayAtIndex(2, "N/A");
			m_Info_OccupiedTerrain.SetValueDisplayAtIndex(3, "N/A");
			m_Info_OccupiedTerrain.SetValueDisplayAtIndex(4, "N/A");
		}

	}
	else
	{
		m_Info_UnitStats.SetValueDisplayAtIndex(1, "N/A");
		m_Info_UnitStats.SetValueDisplayAtIndex(2, "N/A");
		m_Info_UnitStats.SetValueDisplayAtIndex(3, "N/A");
		m_Info_UnitStats.SetValueDisplayAtIndex(4, "N/A");

		m_Info_OccupiedTerrain.SetValueDisplayAtIndex(1, "N/A");
		m_Info_OccupiedTerrain.SetValueDisplayAtIndex(2, "N/A");
		m_Info_OccupiedTerrain.SetValueDisplayAtIndex(3, "N/A");
		m_Info_OccupiedTerrain.SetValueDisplayAtIndex(4, "N/A");

		m_vecButtons_ControlPanel[6]->setTexture(*m_pEmptyUnitPortrait);
		m_vecButtons_ControlPanel[6]->setTextureRect(m_ButtonUnitRect_Blue);
	}

	//======================================
	//Updating Viewed Terrain Modifiers
	//======================================
	if (_inViewedUnitTerrain != nullptr && _inViewedUnit != nullptr)
	{
		float valueToUpdate = 0.0f;
		int intToUpdate = 0;
		std::string stringToDisplay;
		
		CUnitEnums::TYPE unitType = _inViewedUnit->GetType();
		CParseConfigCommon::ConvertUnitTypeToString(unitType, stringToDisplay);
		m_Info_ViewedTerrain.SetValueDisplayAtIndex(0, stringToDisplay);
		valueToUpdate = _inViewedUnitTerrain->GetModifierMovement();
		stringToDisplay = ConvertToString(valueToUpdate);
		m_Info_ViewedTerrain.SetValueDisplayAtIndex(1, stringToDisplay);
		valueToUpdate = _inViewedUnitTerrain->GetModifierDamageDealt();
		stringToDisplay = " x" + ConvertToString(valueToUpdate);
		m_Info_ViewedTerrain.SetValueDisplayAtIndex(2, stringToDisplay);
		valueToUpdate = _inViewedUnitTerrain->GetModifierDamageTaken();
		stringToDisplay = " x" + ConvertToString(valueToUpdate);
		m_Info_ViewedTerrain.SetValueDisplayAtIndex(3, stringToDisplay);
		intToUpdate = _inViewedUnitTerrain->GetRangeOffset();
		stringToDisplay = ConvertToString(intToUpdate);
		m_Info_ViewedTerrain.SetValueDisplayAtIndex(4, stringToDisplay);
	}
	else
	{
		m_Info_ViewedTerrain.SetValueDisplayAtIndex(0, "N/A");
		m_Info_ViewedTerrain.SetValueDisplayAtIndex(1, "N/A");
		m_Info_ViewedTerrain.SetValueDisplayAtIndex(2, "N/A");
		m_Info_ViewedTerrain.SetValueDisplayAtIndex(3, "N/A");
		m_Info_ViewedTerrain.SetValueDisplayAtIndex(4, "N/A");
	}



	return true;
}

void CUIManager::VictoryAchieved(CUIEnums::TURN _inSide)
{
	switch (_inSide)	
	{

		case CUIEnums::TURN::BLUE:
		{
			m_VictorySprite.setTextureRect(m_VictoryRect_Blue);
			break;
		}
		case CUIEnums::TURN::RED:
		{
			m_VictorySprite.setTextureRect(m_VictoryRect_Red);
			break;
		}
		default:
		case CUIEnums::TURN::NONE:
		{
			break;
		}
	}
	m_bDisplayVictory = true;
}

void CUIManager::SwitchTurnForUnitPlacment(int* _inAmountA, int* _inAmountB, int* _inAmountC)
{
	m_vecText_ControlPanel[3]->setString("Red Player");

	for (unsigned short i = 0; i < 3; i++)
	{
		m_vecButtons_ControlPanel[i]->setTextureRect(m_ButtonUnitRect_Red);
	}
	m_vecText_DisplayVariables.clear();
	m_vecText_DisplayVariables.push_back(_inAmountA);
	m_vecText_DisplayVariables.push_back(_inAmountB);
	m_vecText_DisplayVariables.push_back(_inAmountC);
}
