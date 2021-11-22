#include "CDebug.h"
#include "CUnit.h"

CDebug::CDebug()
{
	m_pEmptyPortrait = nullptr;
	m_pUnitPortrait = nullptr;
	m_pInfoBackground = nullptr;
	m_pUnitTexture = nullptr;
	m_pDebugWindow = nullptr;
}

CDebug::~CDebug()
{
	delete m_pEmptyPortrait;
	delete m_pUnitPortrait;
	delete m_pInfoBackground;
	delete m_pUnitTexture;
	delete m_pDebugWindow;

	m_pEmptyPortrait = nullptr;
	m_pUnitPortrait = nullptr;
	m_pInfoBackground = nullptr;
	m_pUnitTexture = nullptr;
	m_pDebugWindow = nullptr;
}

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
	m_pUnitPortrait->setTextureRect(m_PortraitIntRect);
	m_pUnitPortrait->setScale(3.0f, 3.0f);
	m_pUnitPortrait->setPosition(sf::Vector2f(0.f,0.0f));

	m_pInfoBackground = new sf::Sprite;
	m_pInfoBackground->setTexture(*m_pEmptyPortrait);
	m_pInfoBackground->setTextureRect(m_PortraitIntRect);
	m_pInfoBackground->setScale(3.0f, 3.0f);
	m_pInfoBackground->setPosition(sf::Vector2f(96.0f,0.0f));

	m_UnitInfo.Initialize(96.0f,0.0f,_inFont,5);
	m_UnitInfo.SetTextAtIndex(0, "UnitStats", "");
	m_UnitInfo.SetTextAtIndex(1, "HP :", "N/A");
	m_UnitInfo.SetTextAtIndex(2, "MOV:", "N/A");
	m_UnitInfo.SetTextAtIndex(3, "DMG:", "N/A");
	m_UnitInfo.SetTextAtIndex(4, "RGE:", "N/A");

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

	//Create + and - buttons for each text
	//[0]&[1] - HP, [2]&[3] - MOV, [4]&[5] - DMG
	for (size_t i = 0; i < 6; i++)
	{
		sf::Sprite* newButton = new sf::Sprite();
		newButton->setTexture(*m_pDebugButtonTex);
		newButton->setTextureRect(m_ButtonPlusIntRect);
		newButton->setScale(2.0f, 2.0f);
		newButton->setPosition( sf::Vector2f(96.0f, 112.0f + (i * 48.0f)) );
		m_vecButtons.push_back(newButton);

		newButton = new sf::Sprite();
		newButton->setTexture(*m_pDebugButtonTex);
		newButton->setTextureRect(m_ButtonMinusIntRect);
		newButton->setScale(2.0f, 2.0f);
		newButton->setPosition(sf::Vector2f(128.0f, 112.0f + (i * 48.0f)) );
		m_vecButtons.push_back(newButton);

		newButton = nullptr;
	}

	m_pDebugWindow = new sf::RenderWindow(sf::VideoMode(192, 256), "Debug Window");
	m_pDebugWindow->setVerticalSyncEnabled(false);
	m_pDebugWindow->setFramerateLimit(60);
	DisplayWindow();
	return true;
}

void CDebug::DisplayWindow()
{
	m_pDebugWindow->clear(sf::Color::Black);
	m_pDebugWindow->draw(*m_pUnitPortrait);
	m_pDebugWindow->draw(*m_pInfoBackground);
	m_pDebugWindow->draw(m_HP);
	m_pDebugWindow->draw(m_Movements);
	m_pDebugWindow->draw(m_Damage);
	m_UnitInfo.DisplayInfo(*m_pDebugWindow);

	for (auto& sprite : m_vecButtons)
	{
		m_pDebugWindow->draw(*sprite);
	}

	m_pDebugWindow->display();
}

void CDebug::Update(CUnit* _inUnit)
{
	if (_inUnit != nullptr)
	{
		float valueToUpdate = 0.0f;
		float value2 = 0.0f;
		int intToUpdate = 0;
		std::string stringToDisplay;
		auto ConvertToString = [](auto input) {std::stringstream str; str << std::setprecision(1) << std::fixed << input; return str.str(); };
		//=============================
		//Updating Unit stats display
		//=============================
		valueToUpdate = _inUnit->GetHP();
		stringToDisplay = ConvertToString(valueToUpdate);
		m_UnitInfo.SetValueDisplayAtIndex(1, stringToDisplay);
		//[Value2->maxMovePoints, ValueToUpdate->CurrentMovePoints]//
		_inUnit->GetMovementStat(value2, valueToUpdate);
		stringToDisplay = ConvertToString(valueToUpdate) + "/" + ConvertToString(value2);
		m_UnitInfo.SetValueDisplayAtIndex(2, stringToDisplay);
		valueToUpdate = _inUnit->GetDamageDealt();
		stringToDisplay = ConvertToString(valueToUpdate);
		m_UnitInfo.SetValueDisplayAtIndex(3, stringToDisplay);
		intToUpdate = _inUnit->GetRange();
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
					currentRect = m_PortraitIntRect;
					break;
				}
				case CUnitEnums::SIDE::RED:
				{
					currentRect = m_PortraitIntRect;
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
			m_pUnitPortrait->setTextureRect(m_PortraitIntRect);
		}
	}
}
