#ifndef __EPUSHER_INCLUDED__
#define __EPUSHER_INCLUDED__

#include "entity.hpp"
#include "game-world.hpp"
#include "image.hpp"
#include "system.hpp"


namespace kt {


class EPusher : public Entity {
public:
	EPusher(GameWorld *pw) : Entity(pw) {
		m_type = EntityPusher;
		m_pImage = NULL;
	}

	virtual ~EPusher() {
	}

	virtual void Init() {
		m_pImage = System.Display.GetImage("pusher.png");
		m_minRadius = 16.0f;
		m_maxRadius = 384.0f;
		m_radius = m_minRadius;
		m_step   = 2.0f;
	}

	virtual bool Tick(timestamp) {
		m_radius += m_step / 16.0f;
		if (m_step < 0)
			m_step = -m_radius;
		else
			m_step = m_radius;
		if (m_radius > m_maxRadius) {
			m_radius = m_maxRadius;
			m_step   = -2.0f;
		}
		if (m_radius < m_minRadius) {
			m_radius = m_minRadius;
			m_step   = 2.0f;
		}
		return true;
	}

	virtual void Render() {
		float x = m_body.position.x;
		float y = m_body.position.y;
		m_pImage->RenderCenteredTransformed(gfx::LayerClose, x, y, m_radius, m_radius, 0.0f, 1.0f);
	}	

	float		m_step;
	float       m_radius;
	float       m_maxRadius, m_minRadius;
	gfx::Image* m_pImage;
};


} // namespace kt

#endif // __EPUSHER_INCLUDED__
