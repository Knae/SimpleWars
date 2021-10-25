#include <time.h>
#include "CGameManager.h"
#include "CSceneManager.h"
#include "CUIManager.h"

int main()
{
	srand( unsigned int(time(0)) );

	sf::Clock m_TimeCountUp;
	sf::Event gameEvents;

	CGameManager& m_GMRef = CGameManager::GetRef();
	CSceneManager& m_SMRef = CSceneManager::GetRef();
	CUIManager& m_UIMRef = CUIManager::GetRef();

	m_GMRef.IntializeGame();
	m_GMRef.SetPointersToOtherSystems(&m_UIMRef, &m_SMRef);
	m_GMRef.LoadScene();


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


		m_GMRef.DisplayGameWorld();
	}

	return 1;
}