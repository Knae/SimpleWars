
#include "CButton.h"

CButton::CButton()
{
} 

CButton::~CButton()
{
}

void CButton::Initialize(std::string _inName, sf::Font* _inFont, std::string _inText, sf::Vector2f _inPosition, unsigned int in_TileWidth)
{
	sf::Vector2f position = sf::Vector2f(_inPosition.x , _inPosition.y);

	m_strButtonName = _inName;

	m_ButtonText.setString(_inText);
	m_ButtonText.setFont(*_inFont);
	m_ButtonText.setCharacterSize(m_uiFontSize);
	m_ButtonText.setFillColor(m_FontColor);
	m_ButtonText.setPosition(position.x + 8.0f, position.y + 8.0f);

	m_ButtonBorder.setSize(m_ButtonDimensions);
	m_ButtonBorder.setPosition(position.x, position.y);
	m_ButtonBorder.setOutlineColor(m_FontColor);
	m_ButtonBorder.setOutlineThickness(m_fOutlineThickness);
	m_ButtonBorder.setFillColor(sf::Color::Transparent);
}

void CButton::Display(sf::RenderWindow& _inWindow)
{
	_inWindow.draw(m_ButtonBorder);
	_inWindow.draw(m_ButtonText);
}

bool CButton::CheckIfClicked(sf::Vector2f _inCoordinates)
{
	return m_ButtonBorder.getGlobalBounds().contains(_inCoordinates);
}
