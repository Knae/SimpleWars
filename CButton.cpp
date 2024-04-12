
#include "CButton.h"

CButton::CButton()
{
} 

CButton::~CButton()
{
}

/// <summary>
/// Sets up the button's text and rectangle 
/// </summary>
/// <param name="_inName"></param>
/// <param name="_inFont"></param>
/// <param name="_inText"></param>
/// <param name="_inPosition"></param>
/// <param name="in_TileWidth"></param>
void CButton::Initialize_Text(std::string _inName, sf::Font* _inFont, std::string _inText, sf::Vector2f _inPosition, sf::Vector2f _inSize)
{
	sf::Vector2f position = sf::Vector2f(_inPosition.x , _inPosition.y);

	m_strButtonName = _inName;

	m_ButtonText.setString(_inText);
	m_ButtonText.setFont(*_inFont);
	m_ButtonText.setCharacterSize(m_uiFontSize);
	m_ButtonText.setFillColor(m_FontColor);
	m_ButtonText.setPosition(position.x + 8.0f, position.y + 8.0f);

	sf::Vector2f buttonSize = m_ButtonDimensions;
	if (_inSize.x != 0)
	{
		buttonSize.x = _inSize.x;
	}

	if (_inSize.y != 0)
	{
		buttonSize.y = _inSize.y;
	}

	m_ButtonBorder.setSize(buttonSize);
	m_ButtonBorder.setPosition(position.x, position.y);
	m_ButtonBorder.setOutlineColor(m_FontColor);
	m_ButtonBorder.setOutlineThickness(m_fOutlineThickness);
	m_ButtonBorder.setFillColor(sf::Color::Transparent);

	m_bTextOnly = true;
}

void CButton::Initialize_Sprite(std::string _inName, sf::Texture* _inTexture, sf::Vector2f _inPosition, sf::Vector2f _inSize)
{
	sf::Vector2f position = sf::Vector2f(_inPosition.x, _inPosition.y);

	m_strButtonName = _inName;

	m_ButtonSprite.setTexture(*_inTexture);
	m_ButtonSprite.setPosition(position);

	m_ButtonBorder.setSize(_inSize);
	m_ButtonBorder.setPosition(position.x, position.y);
	m_ButtonBorder.setOutlineColor(sf::Color::Black);
	m_ButtonBorder.setOutlineThickness(m_fOutlineThickness);
	m_ButtonBorder.setFillColor(sf::Color::Transparent);
	m_bTextOnly = false;
}

void CButton::SetActiveInactiveRect(sf::IntRect _inActiveRect, sf::IntRect _inInactiveRect)
{
	m_SpriteRect[0] = _inActiveRect;
	m_SpriteRect[1] = _inInactiveRect;

	//Set current rect to inactive
	m_ButtonSprite.setTextureRect( m_SpriteRect[1]);
}

/// <summary>
/// Specifically set button active/inactive
/// </summary>
/// <param name="_inSetting"></param>
/// <param name="_inBorderColor"></param>
void CButton::SetActive(bool _inSetting, sf::Color _inBorderColor)
{
	if (!m_bTextOnly)
	{
		m_bIsActive = _inSetting;
		//If active set rect to the 1nd value sets
		m_ButtonSprite.setTextureRect( m_SpriteRect[_inSetting?0:1]);
		m_ButtonBorder.setOutlineColor(_inBorderColor);
	}
}

/// <summary>
/// Toggle active state of button. If activated, change border to specified color. Set to black otherwise.
/// Returns active state
/// </summary>
/// <param name="_inBorderColor"></param>
/// <returns>m_bIsActive</returns>
bool CButton::ToggleButton(sf::Color _inBorderColor)
{
	if (!m_bTextOnly)
	{
		m_bIsActive = !m_bIsActive;
		//If active set rect to the 1nd value sets
		m_ButtonSprite.setTextureRect(m_SpriteRect[m_bIsActive ? 0 : 1]);
		m_ButtonBorder.setOutlineColor(m_bIsActive ? _inBorderColor:sf::Color::Black);
	}
	return m_bIsActive;
}

/// <summary>
/// display the button
/// </summary>
/// <param name="_inWindow"></param>
void CButton::Display(sf::RenderWindow& _inWindow)
{
	if (m_bTextOnly)
	{
		_inWindow.draw(m_ButtonText);
	}
	else
	{
		_inWindow.draw(m_ButtonSprite);
	}

	_inWindow.draw(m_ButtonBorder);
}

/// <summary>
/// returns if the positon (assumed mouse position)
/// is within the button
/// </summary>
/// <param name="_inCoordinates"></param>
/// <returns></returns>
bool CButton::CheckIfClicked(sf::Vector2f _inCoordinates)
{
	return m_ButtonBorder.getGlobalBounds().contains(_inCoordinates);
}
