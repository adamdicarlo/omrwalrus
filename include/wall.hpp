#ifndef __KT_WALL_INCLUDED__
#define __KT_WALL_INCLUDED__

#include "Box2D/Body.h"
#include "Box2D/World.h"
#include "basictypes.hpp"

namespace kt {

	class GameWorld;
	namespace gfx { class Image; }
	
	class Wall {
	public:
		Wall(GameWorld* pw, Vec2D start, uint cxSegs, uint cySegs);
		void Render();
		void Tick();
		
		Body        m_body;
	
	protected:
		GameWorld*  m_pWorld;
		Vec2D       m_start;
		uint		m_cxSegs, m_cySegs;
		gfx::Image* m_pImage;
	};
	
	typedef std::list<Wall*>	WallList;
}

#endif // __KT_WALL_INCLUDED__
