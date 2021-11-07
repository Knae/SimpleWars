#include <time.h>
#include "CGameManager.h"

int main()
{
	srand( unsigned int(time(0)) );

	sf::Clock m_TimeCountUp;
	sf::Event gameEvents;

	CGameManager& m_GMRef = CGameManager::GetRef();
	CSceneManager& m_SMRef = CSceneManager::GetRef();
	CUIManager& m_UIMRef = CUIManager::GetRef();
	CUnitManager& m_UnitMRef = CUnitManager::GetRef();

	m_GMRef.IntializeGame();
	m_GMRef.SetPointersToOtherSystems(&m_UIMRef, &m_SMRef, &m_UnitMRef);
	m_GMRef.LoadScene();

	m_TimeCountUp.restart();

	while (m_GMRef.GetGameWindow() -> isOpen())
	{
		while (m_GMRef.GetGameWindow()->pollEvent(gameEvents))
		{
			if (gameEvents.type == sf::Event::Closed)
			{
				m_GMRef.DestroyGameWorld();
			}
			else
			{

			}

		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			m_GMRef.ProcessMouseClick();
		}
		
		double elapsedTime = (m_TimeCountUp.restart()).asMilliseconds();
		m_GMRef.UpdateManagers(elapsedTime);
		m_GMRef.DisplayGameWorld();
	}

	return 1;
}