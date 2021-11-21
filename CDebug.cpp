#include "CDebug.h"

CDebug::CDebug()
{
	m_EmptyPortrait = nullptr;
	m_UnitPortrait = nullptr;
	m_InfoBackground = nullptr;
	m_UnitTexture = nullptr;
}

CDebug::~CDebug()
{


}

void CDebug::Initiallize(sf::Font* _inFont)
{

	m_UnitPortrait = new sf::Sprite;
	m_UnitPortrait->setTexture(*m_EmptyPortrait);
	m_UnitPortrait->setTextureRect(m_ButtonUnitRect_Blue);
	m_UnitPortrait->setScale(3.0f, 3.0f);
	m_UnitPortrait->setPosition(sf::Vector2f());

	m_InfoBackground = new sf::Sprite;
	m_InfoBackground->setTexture(*m_EmptyPortrait);
	m_InfoBackground->setTextureRect(m_ButtonUnitRect_Blue);
	m_InfoBackground->setScale(3.0f, 3.0f);
	m_InfoBackground->setPosition(sf::Vector2f());

}
