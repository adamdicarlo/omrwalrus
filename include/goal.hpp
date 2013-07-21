#ifndef KTGOAL_HPP
#define KTGOAL_HPP

#include "basictypes.hpp"


namespace kt {

namespace gfx { class Image; }
	
/**
	@author Adam DiCarlo <adam.dicarlo@gmail.com>
*/
class Goal{
public:
	Goal();
	void Set(float x, float y, float radius);
	void Render();
	void Tick();
    ~Goal();
	bool IsInside(Vec2D test);

	gfx::Image* m_pImage;
	Vec2D m_pos;
	float m_radius;
	float m_opacity;
	float m_deltaOpacity;
};

}

#endif
