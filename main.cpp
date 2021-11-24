#include <time.h>
#include "CGameManager.h"

int main()
{
	srand( unsigned int(time(0)) );

	sf::Clock m_TimeCountUp;
	sf::Event gameEvents;

	//All these should be done in GameManager instead
	CGameManager& m_GMRef = CGameManager::GetRef();
	CSceneManager& m_SMRef = CSceneManager::GetRef();
	CUIManager& m_UIMRef = CUIManager::GetRef();
	CUnitManager& m_UnitMRef = CUnitManager::GetRef();
	COverlayManager& m_OverlayRef = COverlayManager::GetRef();
	//=======================================================

	m_GMRef.IntializeGame();
	m_GMRef.SetPointersToOtherSystems(&m_UIMRef, &m_SMRef, &m_UnitMRef, &m_OverlayRef);
	//m_GMRef.LoadScene();

	m_TimeCountUp.restart();

	while (m_GMRef.GetGameWindow() -> isOpen())
	{
		while (m_GMRef.GetGameWindow()->pollEvent(gameEvents))
		{
			if (gameEvents.type == sf::Event::Closed)
			{
				m_GMRef.DestroyGameWorld();
			}
			/*else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))*/
			else if (gameEvents.type == sf::Event::MouseButtonReleased && gameEvents.mouseButton.button == sf::Mouse::Button::Left)
			{
				m_GMRef.ProcessMouseClick();
			}
			else
			{

			}
		}
	
		double elapsedTime = (m_TimeCountUp.restart()).asMilliseconds();
		m_GMRef.UpdateManagers(elapsedTime);
		m_GMRef.DisplayGameWorld();
		m_GMRef.UpdateDebugWorld();
	}

	return 1;
}