#include "CUIManager.h"

std::vector<sf::Sprite*> CUIManager::m_vecButtons_UnitPlacementPanel;
std::vector<sf::Sprite*> CUIManager::m_vecOverlays;
std::vector<sf::Text*> CUIManager::m_vecText_UnitPlacementPanel;
std::vector<int*> CUIManager::m_vecText_DisplayVariables;
sf::Texture* CUIManager::m_ButtonUnitTexture;
sf::Texture* CUIManager::m_ButtonFinish;
sf::Font* CUIManager::m_pFont;
sf::RenderTexture* CUIManager::m_pPanelBackground;
sf::Sprite* CUIManager::m_pSpriteBackground;
unsigned int CUIManager::m_uSceneWidth;
bool CUIManager::m_bEndTurn;

UIEnums::TURN CUIManager::m_eCurrentTurn;
UIEnums::GAMESTATE CUIManager::m_eCurrentUIState;
UIEnums::MOUSESTATE CUIManager::m_eCurrentMouseState;
CUnitEnums::TYPE CUIManager::m_eCurrentTypeChosen;

const CUnitEnums::TYPE CUIManager::m_UnitOnButton[]= {	CUnitEnums::TYPE::INFANTRY,
														CUnitEnums::TYPE::TANK,
														CUnitEnums::TYPE::ARTILLERY,
														CUnitEnums::TYPE::NONE };

const std::string CUIManager::m_strUnitButtonSpriteMap("assets/spritemaps/UnitButtons.png");
const std::string CUIManager::m_strEmblemSpriteMap("assets/spritemaps/FactionEmblems.png");
const std::string CUIManager::m_strGameButtonsSpriteMap("assets/spritemaps/GameButtons.png");
const std::string CUIManager::m_strTileSelectorSpriteMap("assets/spritemaps/tileSelection.png");
const std::string CUIManager::m_strFinishButtonSprite("assets/spritemaps/FinishButton.png");
const sf::IntRect CUIManager::m_ButtonUnitRect_Blue({ 0, 0, 32, 32 });
const sf::IntRect CUIManager::m_ButtonUnitRect_Red({ 0, 32, 32, 32 });

CUIManager::CUIManager()
{
	m_pPanelBackground = nullptr;
	m_pSpriteBackground = nullptr;
	m_ButtonUnitTexture = nullptr;
	m_pFont = nullptr;
	m_uSceneWidth = 0;
	m_bEndTurn = false;
	m_eCurrentTurn = UIEnums::TURN::BLUE;

	//Is const realy a god idea? Maybe have this set from ini?
	//m_strUnitButtonSpriteMap = "assets/spritemaps/UnitButtons.png";
	//m_strEmblemSpriteMap = "assets/spritemaps/FactionEmblems.png";
	//m_strGameButtonsSpriteMap = "assets/spritemaps/GameButtons.png";
	//m_strTileSelectorSpriteMap = "assets/spritemaps/tileSelection.png";
	//m_ButtonUnitRect_Blue = { 0, 0, 32, 32 };
	//m_ButtonUnitRect_Red = { 0, 32, 32, 32 };
}

CUIManager::~CUIManager()
{
	ClearUIElements();

	delete m_pPanelBackground;
	delete m_pSpriteBackground;
	delete m_ButtonUnitTexture;
	delete m_ButtonFinish;

	m_pPanelBackground = nullptr;
	m_pSpriteBackground = nullptr;
	m_ButtonUnitTexture = nullptr;
	m_ButtonFinish = nullptr;
}

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

	if (m_ButtonUnitTexture != nullptr)
	{
		delete m_ButtonUnitTexture;
		m_ButtonUnitTexture = nullptr;
	}

	if (m_ButtonFinish != nullptr)
	{
		delete m_ButtonFinish;
		m_ButtonFinish = nullptr;
	}

	m_ButtonUnitTexture = new sf::Texture;
	if (!m_ButtonUnitTexture->loadFromFile(m_strUnitButtonSpriteMap))
	{
		//std::cout << "\nUnable to initialize textures for unit buttons.\n" << std::endl;
		return false;
	}

	m_ButtonFinish = new sf::Texture;
	if (!m_ButtonFinish->loadFromFile(m_strFinishButtonSprite))
	{
		//std::cout << "\nUnable to initialize textures for unit buttons.\n" << std::endl;
		return false;
	}

	m_uSceneWidth = _inSceneWidth;
	m_pPanelBackground = new sf::RenderTexture();
	m_pPanelBackground->create(_inWindowSize.x- m_uSceneWidth, _inWindowSize.y);
	m_pPanelBackground->clear(sf::Color::Black);

	m_pSpriteBackground = new sf::Sprite();
	m_pSpriteBackground->setTexture(m_pPanelBackground->getTexture(), true);
	m_pSpriteBackground->setPosition((float)(m_uSceneWidth),0.0f);

	m_pFont = _inFont;

	return true;
}

void CUIManager::UpdateUI()
{
	switch (m_eCurrentUIState)
	{
		case UIEnums::GAMESTATE::UNITPLACEMENT:
		{
			if (m_eCurrentTurn == UIEnums::TURN::BLUE)
			{
				m_pPanelBackground->clear(sf::Color::Blue);
			}
			else if (m_eCurrentTurn == UIEnums::TURN::RED)
			{
				m_pPanelBackground->clear(sf::Color::Red);
			}
			else
			{
				m_pPanelBackground->clear(sf::Color::Black);
			}

			for (unsigned short i = 0; i < 3; i++)
			{
				m_vecText_UnitPlacementPanel[i]->setString(std::to_string(*m_vecText_DisplayVariables[i]) + " unit(s) left\nto place");
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
				currentRect = (m_eCurrentTurn==UIEnums::TURN::BLUE)?m_ButtonUnitRect_Blue: m_ButtonUnitRect_Red;
				currentRect.left += (i * 32) + offsetArray[i];
				m_vecButtons_UnitPlacementPanel[i]->setTextureRect(currentRect);
			}
			break;
		}
		default:
		{
			break;
		}
	}
	
}

void CUIManager::DisplayUI(sf::RenderWindow& _inWindow)
{
	UpdateUI();
	_inWindow.draw(*m_pSpriteBackground);
	/// <summary>
	/// TODO: CHECK HERE!
	/// </summary>
	/// <param name="_inWindow"></param>
	for (auto& element : m_vecButtons_UnitPlacementPanel)
	{
		_inWindow.draw(*element);
	}

	for (auto& element : m_vecText_UnitPlacementPanel)
	{
		_inWindow.draw(*element);
	}
}

/// <summary>
/// Empty all vectors containing UI elements such as
/// buttons and text
/// </summary>
void CUIManager::ClearUIElements()
{
	for (auto& element : m_vecButtons_UnitPlacementPanel)
	{
		delete element;
		element = nullptr;
	}

	for (auto& element : m_vecText_UnitPlacementPanel)
	{
		delete element;
		element = nullptr;
	}
}

/// <summary>
/// Process the clicks to determine whether a function in the side panel
/// is being called. 
/// </summary>
/// <param name="_inCoords"></param>
/// <returns></returns>
bool CUIManager::ProcessClick(sf::Vector2f& _inCoords)
{
	//If the mouse is this far right, then it's not hovering over any tiles, but is over the
	//side panel
	if (_inCoords.x > m_uSceneWidth)
	{
		int buttonClicked = ProcessClickInCtrlPanel(_inCoords);
		switch (m_eCurrentUIState)
		{
			case UIEnums::GAMESTATE::UNITPLACEMENT:
			{
				if (buttonClicked != 3)
				{
					m_eCurrentTypeChosen = m_UnitOnButton[buttonClicked];
				}
				else if((*m_vecText_DisplayVariables[0] + *m_vecText_DisplayVariables[1] + *m_vecText_DisplayVariables[2]) == 0)
				{
					m_bEndTurn = true;
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
		return true;
	}
	else
	{
		return false;
	}
	//else if (m_eCurrentMouseState != MOUSESTATE::NONE)
	//{
	//	std::cout << "\nAttempting to retrieve sprite in position [" << std::to_string(mousePosition.x) << "][" << std::to_string(mousePosition.y) << "] !\n" << std::endl;
	//	sf::Sprite* clickeSprite = m_pSceneMgr->GetTileSprite(mousePosition);

	//	switch (m_eCurrentMouseState)
	//	{
	//		case CWindowBuilder::MOUSESTATE::START:
	//		{
	//			if (!m_pSceneMgr->ChangeTileState(mousePosition, CTile::STATE::START))
	//			{
	//				std::cout << "\n----Failed to change tile state to START!.\n" << std::endl;
	//			}
	//			break;
	//		}
	//		case CWindowBuilder::MOUSESTATE::FINISH:
	//		{
	//			if (!m_pSceneMgr->ChangeTileState(mousePosition, CTile::STATE::FINISH))
	//			{
	//				std::cout << "\n----Failed to change tile state to FINISH!.\n" << std::endl;
	//			}
	//			break;
	//		}
	//		case CWindowBuilder::MOUSESTATE::OBSTACLE:
	//		{
	//			if (!m_pSceneMgr->ChangeTileState(mousePosition, CTile::STATE::OBSTACLE))
	//			{
	//				std::cout << "\n----Failed to change tile state to OBSTACLE!.\n" << std::endl;
	//			}
	//			break;
	//		}
	//		case CWindowBuilder::MOUSESTATE::NEUTRAL:
	//		{
	//			if (!m_pSceneMgr->ChangeTileState(mousePosition, CTile::STATE::STANDARD))
	//			{
	//				std::cout << "\n----Failed to change tile state to NEUTRAL!.\n" << std::endl;
	//			}
	//			break;
	//		}
	//		default:
	//		{
	//			break;
	//		}
	//	}
	//}
	return true;
}

int CUIManager::ProcessClickInCtrlPanel(sf::Vector2f& _inCoords)
{
	for (unsigned int i = 0; i < m_vecButtons_UnitPlacementPanel.size(); i++)
	{

		if ((m_vecButtons_UnitPlacementPanel[i])->getGlobalBounds().contains(_inCoords))
		{

			return i;
		}
	}
	return -1;
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
	sf::Sprite* currentButton = nullptr;
	sf::Text* currentButtonText = nullptr;
	sf::Text* currentText = nullptr;
	sf::IntRect currentRect;

	m_vecText_DisplayVariables.push_back(_inAmountA);
	m_vecText_DisplayVariables.push_back(_inAmountB);
	m_vecText_DisplayVariables.push_back(_inAmountC);

	for (int i = 0; i < 3; i++)
	{
		currentButton = new sf::Sprite;
		currentButton->setTexture(*m_ButtonUnitTexture);
		currentRect = m_ButtonUnitRect_Blue;
		currentRect.left += (i * 32);
		currentButton->setTextureRect(currentRect);
		currentButton->setPosition( sf::Vector2f(m_uSceneWidth+16.0f,288.0f+(i*48)) );
		m_vecButtons_UnitPlacementPanel.push_back(currentButton);

		currentButtonText = new sf::Text( std::to_string(*m_vecText_DisplayVariables[i]) + " unit(s) left\nto place", *m_pFont );
		currentButtonText->setFillColor(sf::Color::White);
		currentButtonText->setCharacterSize(16);
		currentButtonText->setPosition(sf::Vector2f(m_uSceneWidth + 64.0f, 288.0f + (i * 48)));
		m_vecText_UnitPlacementPanel.push_back(currentButtonText);
	}

	currentButton = new sf::Sprite;
	currentButton->setTexture(*m_ButtonFinish);
	currentRect = m_ButtonUnitRect_Blue; 
	currentButton->setPosition(sf::Vector2f(m_uSceneWidth + 48.0f, 432));
	m_vecButtons_UnitPlacementPanel.push_back(currentButton);

	currentText = new sf::Text("Blue Player",*m_pFont);
	currentText->setFillColor(sf::Color::White);
	currentText->setCharacterSize(16);
	currentText->setPosition(sf::Vector2f(m_uSceneWidth + 64.0f, 16.0f));
	m_vecText_UnitPlacementPanel.push_back(currentText);

	currentText = new sf::Text("Click a unit to select it,\nthen click a tile to place it.\nClick 'Finish' when done", *m_pFont);
	currentText->setFillColor(sf::Color::White);
	currentText->setCharacterSize(16);
	currentText->setPosition(sf::Vector2f((float)(m_uSceneWidth), 464.0f));
	m_vecText_UnitPlacementPanel.push_back(currentText);

	m_eCurrentUIState = UIEnums::GAMESTATE::UNITPLACEMENT;

	currentButton = nullptr;
	currentButtonText = nullptr;
	currentText = nullptr;
}

void CUIManager::SetUpGameLoopPanel()
{
}

void CUIManager::SwitchTurnForUnitPlacment(int* _inAmountA, int* _inAmountB, int* _inAmountC)
{
	m_vecText_UnitPlacementPanel[3]->setString("Red Player");

	for (unsigned short i = 0; i < 3; i++)
	{
		m_vecButtons_UnitPlacementPanel[i]->setTextureRect(m_ButtonUnitRect_Red);
	}
	m_vecText_DisplayVariables.clear();
	m_vecText_DisplayVariables.push_back(_inAmountA);
	m_vecText_DisplayVariables.push_back(_inAmountB);
	m_vecText_DisplayVariables.push_back(_inAmountC);
}
