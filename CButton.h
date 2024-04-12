#pragma once
#ifndef __CBUTTON_H__
#define __CBUTTON_H__

#include <SFML/Graphics.hpp>

class CButton
{
public:
	CButton();
	~CButton();

	void Initialize_Text(std::string _inName, sf::Font* _inFont, std::string _inText, sf::Vector2f _inPosition, sf::Vector2f _inSize = sf::Vector2f());
	void Initialize_Sprite(std::string _inName, sf::Texture* _inTexture, sf::Vector2f _inPosition, sf::Vector2f _inSize);
	void SetActiveInactiveRect(sf::IntRect _inActiveRect, sf::IntRect _inInactiveRect);
	void SetActive(bool _inSetting, sf::Color _inBorderColor = sf::Color::Black);
	bool ToggleButton(sf::Color _inBorderColor = sf::Color::Black);
	void Display(sf::RenderWindow& _inWindow);
	bool CheckIfClicked(sf::Vector2f _inCoordinates);
	inline std::string& GetButtonName() { return m_strButtonName; }
	inline void SetSpriteScale(float _inScaleX, float _inScaleY) { m_ButtonSprite.setScale(_inScaleX, _inScaleY); };
private:
	sf::Text m_ButtonText;
	sf::Sprite m_ButtonSprite;
	sf::RectangleShape m_ButtonBorder;
	std::string m_strButtonName;

	bool m_bTextOnly = false;
	bool m_bIsActive = false;
	bool m_bEnabled = true;
	sf::IntRect m_SpriteRect [2] {sf::IntRect()};

	const unsigned int m_uiFontSize = 18;
	const float m_fOutlineThickness = 8;
	const sf::Color m_FontColor = sf::Color::White;
	const sf::Vector2f m_ButtonDimensions = sf::Vector2f(128.0f,64.0f);
};

#endif // !__CBUTTON_H__