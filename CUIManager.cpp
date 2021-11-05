#include "CUIManager.h"

std::vector<sf::Sprite*> CUIManager::m_vecButtons_UnitPlacementPanel;
std::vector<sf::Text*> CUIManager::m_vecText_UnitPlacementPanel;
sf::Texture* CUIManager::m_ButtonUnitTexture;
sf::Font* CUIManager::m_pFont;
sf::RenderTexture* CUIManager::m_pPanelBackground;
sf::Sprite* CUIManager::m_pSpriteBackground;
unsigned int CUIManager::m_uSceneWidth;
UIEnums::TURN CUIManager::m_eCurrentTurn;
UIEnums::GAMESTATE CUIManager::m_eCurrentUIState;
UIEnums::MOUSESTATE CUIManager::m_eCurrentMouseState;
CUnitEnums::TYPE CUIManager::m_eCurrentUnitChosen;

const CUnitEnums::TYPE CUIManager::m_UnitOnButton[]= {	CUnitEnums::TYPE::INFANTRY,
														CUnitEnums::TYPE::TANK,
														CUnitEnums::TYPE::ARTILLERY,
														CUnitEnums::TYPE::NONE };

const std::string CUIManager::m_strUnitButtonSpriteMap("assets/spritemaps/UnitButtons.png");
const std::string CUIManager::m_strEmblemSpriteMap("assets/spritemaps/FactionEmblems.png");
const std::string CUIManager::m_strGameButtonsSpriteMap("assets / spritemaps / GameButtons.png");
const std::string CUIManager::m_strTileSelectorSpriteMap("assets/spritemaps/tileSelection.png");
const sf::IntRect CUIManager::m_ButtonUnitRect_Blue({ 0, 0, 32, 32 });
const sf::IntRect CUIManager::m_ButtonUnitRect_Red({ 0, 32, 32, 32 });

CUIManager::CUIManager()
{
	m_pPanelBackground = nullptr;
	m_pSpriteBackground = nullptr;
	m_ButtonUnitTexture = nullptr;
	m_eCurrentTurn = UIEnums::TURN::BLUE;
	//m_strUnitButtonSpriteMap = "assets/spritemaps/UnitButtons.png";
	//m_strEmblemSpriteMap = "assets/spritemaps/FactionEmblems.png";
	//m_strGameButtonsSpriteMap = "assets/spritemaps/GameButtons.png";
	//m_strTileSelectorSpriteMap = "assets/spritemaps/tileSelection.png";
	//m_ButtonUnitRect_Blue = { 0, 0, 32, 32 };
	//m_ButtonUnitRect_Red = { 0, 32, 32, 32 };
}

CUIManager::~CUIManager()
{
	delete m_pPanelBackground;
	delete m_pSpriteBackground;
	delete m_ButtonUnitTexture;

	m_pPanelBackground = nullptr;
	m_pSpriteBackground = nullptr;
	m_ButtonUnitTexture = nullptr;
}

bool CUIManager::IntializeUI(sf::Vector2u _inWindowSize, const unsigned int _inSceneWidth)
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

	m_ButtonUnitTexture = new sf::Texture;
	if (!m_ButtonUnitTexture->loadFromFile(m_strUnitButtonSpriteMap))
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

	SetUpUnitPlacementPanel();

	//Testing
	m_eCurrentUIState = UIEnums::GAMESTATE::UNITPLACEMENT;
	return true;
}

void CUIManager::UpdateUI()
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

	int offsetArray[3] = { 0 };
	switch (m_eCurrentUnitChosen)
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
		currentRect = m_ButtonUnitRect_Blue;
		currentRect.left += (i * 32) + offsetArray[i];
		m_vecButtons_UnitPlacementPanel[i]->setTextureRect(currentRect);
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
}

/// <summary>
/// Process the clicks to determine whether a function in the side panel
/// is being called. 
/// </summary>
/// <param name="_inCoords"></param>
/// <returns></returns>
bool CUIManager::ProcessClick(sf::Vector2f& _inCoords)
{
	//sf::Vector2f mousePosition = m_pWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_pWindow));
	//If the mouse is this far right, then it's not hovering over any tiles, but is over the
	//side panel
	if (_inCoords.x > m_uSceneWidth)
	{
		int buttonClicked = ProcessClickInCtrlPanel(_inCoords);
		switch (m_eCurrentUIState)
		{
			case UIEnums::GAMESTATE::UNITPLACEMENT:
			{
				m_eCurrentUnitChosen = m_UnitOnButton[buttonClicked];
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
/// TODO: Separate them into separate sprite objects in a map
///		Then we can be certain even if the images are changed or the
///		the order is changed, it'll still be what we expect it to be.
/// </summary>
void CUIManager::SetUpUnitPlacementPanel()
{
	sf::Sprite* currentButton = nullptr;
	sf::Text* currentButtonText = nullptr;
	sf::Text* currentText = nullptr;
	sf::IntRect currentRect;
	for (int i = 0; i < 4; i++)
	{
		currentButton = new sf::Sprite;
		currentButton->setTexture(*m_ButtonUnitTexture);
		currentRect = m_ButtonUnitRect_Blue;
		currentRect.left += (i * 32);
		currentButton->setTextureRect(currentRect);
		currentButton->setPosition( sf::Vector2f(m_uSceneWidth+16.0f,320.0f+(i*48)) );
		m_vecButtons_UnitPlacementPanel.push_back(currentButton);
	}


	currentButton = nullptr;
}

void CUIManager::ChangeState()
{
}
