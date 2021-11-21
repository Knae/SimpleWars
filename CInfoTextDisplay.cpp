#include "CInfoTextDisplay.h"

CInfoTextDisplay::CInfoTextDisplay()
{
}

CInfoTextDisplay::~CInfoTextDisplay()
{
	Clear();
}

void CInfoTextDisplay::Initialize(float _inTopLeftPosX, float _inTopLeftPosY, sf::Font* _inFont, unsigned int _inNumberOfText)
{
	for (unsigned int i = 0; i < _inNumberOfText; i++)
	{
		sf::Text* newLabel = new sf::Text();
		newLabel->setFont(*_inFont);
		newLabel->setCharacterSize(m_fFontSize);
		newLabel->setFillColor(m_fFontColor);
		newLabel->setPosition(_inTopLeftPosX + 8.0f, _inTopLeftPosY + 8.0f +(16.0f * i));
		m_vecLabels.push_back(newLabel);

		sf::Text* newValue = new sf::Text();
		newValue->setFont(*_inFont);
		newValue->setCharacterSize(m_fFontSize);
		newValue->setFillColor(m_fFontColor);
		newValue->setPosition(_inTopLeftPosX + 48.0f, _inTopLeftPosY + 8.0f + (16.0f * i));
		m_vecValues.push_back(newValue);

		newLabel = nullptr;
		newValue = nullptr;
	}
}

bool CInfoTextDisplay::SetTextAtIndex(int _inIndex, std::string _inLabel, std::string _inValue)
{
	size_t givenIndex = _inIndex;
	if (givenIndex <= m_vecLabels.size())
	{
		m_vecLabels[givenIndex]->setString(_inLabel);
		m_vecValues[givenIndex]->setString(_inValue);
		return true;
	}
	else
	{
		//Index given out of range of vector
		return false;
	}
}

bool CInfoTextDisplay::SetValueDisplayAtIndex(int _inIndex, std::string _inValue)
{
	size_t givenIndex = _inIndex;
	if (givenIndex <= m_vecLabels.size())
	{
		m_vecValues[_inIndex]->setString(_inValue);
		return true;
	}
	else
	{
		//Index given out of range of vector
		return false;
	}
}

void CInfoTextDisplay::DisplayInfo(sf::RenderWindow& _inWindow)
{
	for (auto& label : m_vecLabels)
	{
		_inWindow.draw(*label);
		//std::cout << "\nPrinting - " <<  std::string(label->getString()) << std::endl;
	}

	for (auto& value : m_vecValues)
	{
		_inWindow.draw(*value);
		//std::cout << "\nPrinting - " << std::string(value->getString()) << std::endl;
	}
}

void CInfoTextDisplay::Clear()
{
	for (auto& label : m_vecLabels)
	{
		delete label;
		label = nullptr;
	}

	for (auto& value : m_vecValues)
	{
		delete value;
		value = nullptr;
	}

	m_vecLabels.clear();
	m_vecValues.clear();
}
