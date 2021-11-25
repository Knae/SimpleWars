#include "CDebug.h"
#include "CUnit.h"

CDebug::CDebug()
{
	m_pEmptyPortrait = nullptr;
	m_pUnitPortrait = nullptr;
	m_pInfoBackground = nullptr;
	m_pUnitTexture = nullptr;
	m_pDebugWindow = nullptr;
	m_pDebugButtonTex = nullptr;
	m_pDebugKillTex = nullptr;
	m_pCurrentUnit = nullptr;
	m_pUnitToDestroy = nullptr;
}

int CDebug::isButtonClicked(sf::Vector2f& _input)
{
	for (unsigned int i = 0; i < m_vecButtons.size(); i++)
	{

		if ((m_vecButtons[i])->getGlobalBounds().contains(_input))
		{
			return i;
		}
	}
	return -1;
}

CDebug::~CDebug()
{
	delete m_pEmptyPortrait;
	delete m_pUnitPortrait;
	delete m_pInfoBackground;
	delete m_pUnitTexture;
	delete m_pDebugWindow;
	delete m_pDebugButtonTex;
	delete m_pDebugKillTex;

	m_pEmptyPortrait = nullptr;
	m_pUnitPortrait = nullptr;
	m_pInfoBackground = nullptr;
	m_pUnitTexture = nullptr;
	m_pDebugWindow = nullptr;
	m_pDebugButtonTex = nullptr;
	m_pDebugKillTex = nullptr;
	m_pCurrentUnit = nullptr;
	m_pUnitToDestroy = nullptr;
}

/// <summary>
/// Creates all the sprites and text to be 
/// displayed on the debug window
/// </summary>
/// <param name="_inFont"></param>
/// <returns></returns>
bool CDebug::Initialize(sf::Font* _inFont)
{
	m_pEmptyPortrait = new sf::Texture();
	m_pUnitTexture = new sf::Texture();
	m_pDebugButtonTex = new sf::Texture();
	m_pDebugKillTex = new sf::Texture();

	if (!m_pEmptyPortrait->loadFromFile(m_strEmptyUnitSpritemap))
	{
		std::cout << "\nDEBUG:Unable to load empty portrait" << std::endl;
		return false;
	}

	if (!m_pUnitTexture->loadFromFile(m_strUnitButtonSpritemap))
	{
		std::cout << "\nDEBUG:Unable to load unit sprites" << std::endl;
		return false;
	}

	if (!m_pDebugButtonTex->loadFromFile(m_strDebugButtonSpriteMap))
	{
		std::cout << "\nDEBUG:Unable to load button sprites" << std::endl;
		return false;
	}

	if (!m_pDebugKillTex->loadFromFile(m_strDebugKill))
	{
		std::cout << "\nDEBUG:Unable to load kill button sprite" << std::endl;
		return false;
	}

	m_pUnitPortrait = new sf::Sprite;
	m_pUnitPortrait->setTexture(*m_pEmptyPortrait);
	m_pUnitPortrait->setTextureRect(m_PortraitIntRect_Blue);
	m_pUnitPortrait->setScale(3.0f, 3.0f);
	m_pUnitPortrait->setPosition(sf::Vector2f(0.f,0.0f));

	m_pInfoBackground = new sf::Sprite;
	m_pInfoBackground->setTexture(*m_pEmptyPortrait);
	m_pInfoBackground->setTextureRect(m_PortraitIntRect_Blue);
	m_pInfoBackground->setScale(3.0f, 3.0f);
	m_pInfoBackground->setPosition(sf::Vector2f(96.0f,0.0f));

	m_UnitInfo.Initialize(96.0f,0.0f,_inFont,5);
	m_UnitInfo.SetTextAtIndex(0, "Unit:", "NONE");
	m_UnitInfo.SetTextAtIndex(1, "HP  :", "N/A");
	m_UnitInfo.SetTextAtIndex(2, "MOV :", "N/A");
	m_UnitInfo.SetTextAtIndex(3, "DMG :", "N/A");
	m_UnitInfo.SetTextAtIndex(4, "RGE :", "N/A");

	m_HP.setFont(*_inFont);
	m_HP.setCharacterSize(m_uiFontSize);
	m_HP.setFillColor(m_FontColor);
	m_HP.setPosition(16.0f, 112.0f);
	m_HP.setString("HP     :");

	m_Movements.setFont(*_inFont);
	m_Movements.setCharacterSize(m_uiFontSize);
	m_Movements.setFillColor(m_FontColor);
	m_Movements.setPosition(16.0f, 160.0f);
	m_Movements.setString("MOV Pts:");

	m_Damage.setFont(*_inFont);
	m_Damage.setCharacterSize(m_uiFontSize);
	m_Damage.setFillColor(m_FontColor);
	m_Damage.setPosition(16.0f, 208.0f);
	m_Damage.setString("DMG    :");

	m_Range.setFont(*_inFont);
	m_Range.setCharacterSize(m_uiFontSize);
	m_Range.setFillColor(m_FontColor);
	m_Range.setPosition(16.0f, 256.0f);
	m_Range.setString("RGE    :");

	//Create + and - buttons for each text
	//[0]&[1] - HP, [2]&[3] - MOV, [4]&[5] - DMG,[6]&[7]
	//[6] Kill me
	for (size_t i = 0; i < 4; i++)
	{
		sf::Sprite* newButton = new sf::Sprite();
		newButton->setTexture(*m_pDebugButtonTex);
		newButton->setTextureRect(m_ButtonMinusIntRect);
		newButton->setScale(2.0f, 2.0f);
		newButton->setPosition( sf::Vector2f(96.0f, 112.0f + (i * 48.0f)) );
		m_vecButtons.push_back(newButton);

		newButton = new sf::Sprite();
		newButton->setTexture(*m_pDebugButtonTex);
		newButton->setTextureRect(m_ButtonPlusIntRect);
		newButton->setScale(2.0f, 2.0f);
		newButton->setPosition(sf::Vector2f(128.0f, 112.0f + (i * 48.0f)) );
		m_vecButtons.push_back(newButton);

		newButton = nullptr;
	}

	sf::Sprite* newButton = new sf::Sprite();
	newButton = new sf::Sprite();
	newButton->setTexture(*m_pDebugKillTex);
	newButton->setTextureRect(m_ButtonPlusIntRect);
	newButton->setScale(2.0f, 2.0f);
	newButton->setPosition(sf::Vector2f(64.0f, 304.0f));
	m_vecButtons.push_back(newButton);

	newButton = nullptr;

	m_pDebugWindow = new sf::RenderWindow(sf::VideoMode(192, 352), "Debug Window");
	m_pDebugWindow->setVerticalSyncEnabled(false);
	m_pDebugWindow->setFramerateLimit(60);
	DisplayWindow();
	return true;
}

/// <summary>
/// Updates the window, to evaluate events
/// </summary>
void CDebug::Update()
{
	if (m_pDebugWindow->hasFocus())
	{
		sf::Event debugEvent;
		while (m_pDebugWindow->pollEvent(debugEvent))
		{
			if (debugEvent.type == sf::Event::Closed)
			{
				//m_pDebugWindow->setVisible(false);
			}

			if (m_pCurrentUnit!=nullptr && debugEvent.type == sf::Event::MouseButtonReleased && debugEvent.mouseButton.button == sf::Mouse::Button::Left)
			{
				sf::Vector2f mousePosition = m_pDebugWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_pDebugWindow));
				int buttonClicked = isButtonClicked(mousePosition);
				//[0]&[1] - HP, [2]&[3] - MOV, [4]&[5] - DMG, [6]&[7] - Range
				switch (buttonClicked)
				{
					//Damage the unit
					case 0:
					{
						float damage = 1.0f;
						m_pCurrentUnit->TakeDamage(damage);
						break;
					}
					//Heal the unit
					case 1:
					{
						float damage = -1.0f;
						m_pCurrentUnit->TakeDamage(damage);
						break;
					}
					//remove MOV point
					case 2:
					{
						m_pCurrentUnit->IncrementMovementPoints(-1.0f);
						break;
					}
					//add MOV point
					case 3:
					{
						m_pCurrentUnit->IncrementMovementPoints(1.0f);
						break;
					}
					//Reduce damage dealt
					case 4:
					{
						m_pCurrentUnit->IncrementDamageDealt(-1);
						break;
					}
					//Increase damage dealt
					case 5:
					{
						m_pCurrentUnit->IncrementDamageDealt(1);
						break;
					}
					//Decrease range
					case 6:
					{
						m_pCurrentUnit->IncrementRange(-1);
						break;
					}
					//Increase range
					case 7:
					{
						m_pCurrentUnit->IncrementRange(1);
						break;
					}
					//Kill Unit
					case 8:
					{
						m_pUnitToDestroy = m_pCurrentUnit;
						m_pCurrentUnit = nullptr;
						break;
					}
					default:
						break;
				}
			}
		}
	}
}

/// <summary>
/// Display all the debug elements
/// </summary>
void CDebug::DisplayWindow()
{
	m_pDebugWindow->clear(sf::Color::Black);
	m_pDebugWindow->draw(*m_pUnitPortrait);
	m_pDebugWindow->draw(*m_pInfoBackground);
	m_pDebugWindow->draw(m_HP);
	m_pDebugWindow->draw(m_Movements);
	m_pDebugWindow->draw(m_Damage);
	m_pDebugWindow->draw(m_Range);
	m_UnitInfo.DisplayInfo(*m_pDebugWindow);

	for (auto& sprite : m_vecButtons)
	{
		m_pDebugWindow->draw(*sprite);
	}

	m_pDebugWindow->display();
}

/// <summary>
/// set the pointer to the selected unit to mark it for death
/// </summary>
/// <returns></returns>
CUnit* CDebug::GetUnitToDestoy()
{
	CUnit* unitMarked = m_pUnitToDestroy;
	m_pUnitToDestroy = nullptr;
	return unitMarked;
}

/// <summary>
/// Update the info display with the selected unit's stats
/// </summary>
/// <param name="_inUnit"></param>
void CDebug::UpdateInfoDisplay(CUnit* _inUnit)
{
	if (_inUnit != nullptr)
	{
		m_pCurrentUnit = _inUnit;
		float valueToUpdate = 0.0f;
		float value2 = 0.0f;
		int intToUpdate = 0;
		std::string stringToDisplay;
		auto ConvertToString = [](auto input) {std::stringstream str; str << std::setprecision(1) << std::fixed << input; return str.str(); };
		//=============================
		//Updating Unit stats display
		//=============================
		CUnitEnums::TYPE unitType = m_pCurrentUnit->GetType();
		CParseConfigCommon::ConvertUnitTypeToString(unitType, stringToDisplay);
		m_UnitInfo.SetValueDisplayAtIndex(0, stringToDisplay);
		valueToUpdate = m_pCurrentUnit->GetHP();
		stringToDisplay = ConvertToString(valueToUpdate);
		m_UnitInfo.SetValueDisplayAtIndex(1, stringToDisplay);
		//[Value2->maxMovePoints, ValueToUpdate->CurrentMovePoints]//
		m_pCurrentUnit->GetMovementStat(value2, valueToUpdate);
		stringToDisplay = ConvertToString(valueToUpdate) + "/" + ConvertToString(value2);
		m_UnitInfo.SetValueDisplayAtIndex(2, stringToDisplay);
		valueToUpdate = m_pCurrentUnit->GetDamageDealt();
		stringToDisplay = ConvertToString(valueToUpdate);
		m_UnitInfo.SetValueDisplayAtIndex(3, stringToDisplay);
		intToUpdate = m_pCurrentUnit->GetRange();
		stringToDisplay = ConvertToString(intToUpdate);
		m_UnitInfo.SetValueDisplayAtIndex(4, stringToDisplay);

		CUnitEnums::SIDE m_eCurrentUnitSide = _inUnit->GetSide();
		CUnitEnums::TYPE selectedUnitType = _inUnit->GetType();

		if (selectedUnitType != CUnitEnums::TYPE::NONE)
		{
			m_pUnitPortrait->setTexture(*m_pUnitTexture);
			sf::IntRect currentRect;

			switch (m_eCurrentUnitSide)
			{
				case CUnitEnums::SIDE::BLUE:
				{
					currentRect = m_PortraitIntRect_Blue;
					break;
				}
				case CUnitEnums::SIDE::RED:
				{
					currentRect = m_PortraitIntRect_Red;
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
			m_pUnitPortrait->setTextureRect(currentRect);
		}
		else
		{
			m_pUnitPortrait->setTexture(*m_pEmptyPortrait);
			m_pUnitPortrait->setTextureRect(m_PortraitIntRect_Blue);
		}
	}
	else
	{
		m_UnitInfo.SetValueDisplayAtIndex(0, "NONE");
		m_UnitInfo.SetValueDisplayAtIndex(1, "N/A");
		m_UnitInfo.SetValueDisplayAtIndex(2, "N/A");
		m_UnitInfo.SetValueDisplayAtIndex(3, "N/A");
		m_UnitInfo.SetValueDisplayAtIndex(4, "N/A");

		m_pUnitPortrait->setTexture(*m_pEmptyPortrait);
		m_pUnitPortrait->setTextureRect(m_PortraitIntRect_Blue);
	}
}
