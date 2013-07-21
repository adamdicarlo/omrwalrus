#ifndef __KT_GAME_INCLUDED__
#define __KT_GAME_INCLUDED__

#include "scheduler.hpp"
#include "game-world.hpp"

namespace kt {
	
	class IGameView;
	typedef std::list<IGameView*>	IGameViewList;
	
	class Game {
	public:
		~Game();
		void Init();
		void Shutdown();
		void Render();
		void Tick(timestamp tsTime);
		GameWorld* World() { return &m_world; }
	
	private:
		GameWorld			m_world;
		IGameViewList		m_lsViews;
	};

} // namespace kt

#endif // __KT_GAME_INCLUDED__
