#ifndef __EBALL_INCLUDED__
#define __EBALL_INCLUDED__

#include "entity.hpp"
#include "game-world.hpp"
#include "image.hpp"
#include "system.hpp"

#define BALL_MASS				50.0f

namespace kt {


class EBall : public Entity {
public:
	EBall(GameWorld *pw) : Entity(pw) {
		m_type = EntityBall;
		m_pImage = NULL;
	}

	virtual ~EBall() {
		m_pWorld->B2DWorld()->Remove(&m_body);
	}

	virtual void Init() {
		m_pImage = System.Display.GetImage("ball.png");
		m_body.Set(Vec2((float)m_pImage->Width(), (float)m_pImage->Height()), BALL_MASS);
		m_pWorld->B2DWorld()->Add(&m_body);
		m_life = (int) (System.TicksPerSec() * 2);
	}

	virtual bool Tick(timestamp) {
		if (m_body.velocity.x >= -0.01 && m_body.velocity.x <= 0.01 &&
			m_body.velocity.y >= -0.01 && m_body.velocity.y <= 0.01) {
			--m_life;
			if (m_life <= 0)
				return false; // delete me!
		}
		
		return true;
	}

	virtual void Render() {
		float x = m_body.position.x;
		float y = m_body.position.y;
		m_pImage->RenderCentered(gfx::LayerClose, x, y);
	}	

	int         m_life;
	gfx::Image* m_pImage;
};


} // namespace kt

#endif // __EBALL_INCLUDED__
