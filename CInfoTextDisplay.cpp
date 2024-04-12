#include "CInfoTextDisplay.h"

CInfoTextDisplay::CInfoTextDisplay()
{
}

CInfoTextDisplay::~CInfoTextDisplay()
{
	Clear();
}

/// <summary>
/// Initializes the InfoDisplay class with info of 
/// where to place and how many items to display
/// </summary>
/// <param name="_inTopLeftPosX"></param>
/// <param name="_inTopLeftPosY"></param>
/// <param name="_inFont"></param>
/// <param name="_inNumberOfText"></param>
void CInfoTextDisplay::Initialize(float _inTopLeftPosX, float _inTopLeftPosY, sf::Font* _inFont, unsigned int _inNumberOfText)
{
	
	m_TopLeftPosition = sf::Vector2f(_inTopLeftPosX, _inTopLeftPosY);

	for (unsigned int i = 0; i < _inNumberOfText; i++)
	{
		sf::Text* newLabel = new sf::Text();
		newLabel->setFont(*_inFont);
		newLabel->setCharacterSize(m_uiFontSize);
		newLabel->setFillColor(m_FontColor);
		newLabel->setPosition(m_TopLeftPosition.x + 8.0f, m_TopLeftPosition.y + 8.0f +(16.0f * i));
		m_vecLabels.push_back(newLabel);

		sf::Text* newValue = new sf::Text();
		newValue->setFont(*_inFont);
		newValue->setCharacterSize(m_uiFontSize);
		newValue->setFillColor(m_FontColor);
		newValue->setPosition(m_TopLeftPosition.x + 48.0f, m_TopLeftPosition.y + 8.0f + (16.0f * i));
		m_vecValues.push_back(newValue);

		newLabel = nullptr;
		newValue = nullptr;
	}
}

/// <summary>
/// Set the label and value of a specific display item
/// </summary>
/// <param name="_inIndex"></param>
/// <param name="_inLabel"></param>
/// <param name="_inValue"></param>
/// <returns></returns>
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

/// <summary>
///	Set the value of a specific item to display
/// </summary>
/// <param name="_inIndex"></param>
/// <param name="_inValue"></param>
/// <returns></returns>
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

/// <summary>
/// Adjust the x and y position of this info display relative to original position
/// </summary>
/// <param name="_inDisplacement"></param>
void CInfoTextDisplay::AdjustPosition(sf::Vector2f _inChange)
{
	for (unsigned int i = 0; i < m_vecLabels.size(); i++)
	{
		m_vecLabels[i]->setPosition(m_TopLeftPosition.x + 8.0f + _inChange.x, m_TopLeftPosition.y + 8.0f + (16.0f * i) + _inChange.y);
		m_vecValues[i]->setPosition(m_TopLeftPosition.x + 48.0f + _inChange.x, m_TopLeftPosition.y + 8.0f + (16.0f * i) + _inChange.y);
	}
}

//Display all the items
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

/// <summary>
/// Destroy everything
/// </summary>
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
