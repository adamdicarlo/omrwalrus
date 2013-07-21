#ifndef __EBRICK_INCLUDED__
#define __EBRICK_INCLUDED__

#include "entity.hpp"
#include "game-world.hpp"
#include "image.hpp"
#include "system.hpp"

#define BUKKIT_MASS				25.0f

namespace kt {


class EBukkit : public Entity {
public:
	EBukkit(GameWorld *pw) : Entity(pw) {
		m_type = EntityBukkit;
		m_pImage = NULL;
	}

	virtual ~EBukkit() {
		m_pWorld->B2DWorld()->Remove(&m_body);
	}

	void InGoal() {
		if (m_state == BukkitFull) {
			m_state = BukkitScored;
			m_pWorld->m_score += 10000;
		}
	}
	
	virtual void Init() {
		m_pImage = System.Display.GetImage("bukkitfull.png");
		m_pimgEaten  = System.Display.GetImage("bukkit.png");
		m_pimgScored = System.Display.GetImage("bukkitscored.png");
		m_body.Set(Vec2((float)m_pImage->Width() - 2.0f, (float)m_pImage->Height() - 2.0f), BUKKIT_MASS);
		m_pWorld->B2DWorld()->Add(&m_body);
		m_state = BukkitFull;
		m_life = (int) (System.TicksPerSec() * 10.0f);
		m_rotate = 0.0f;
	}

	virtual bool Tick(timestamp) {
		if (m_state == BukkitEaten) {
			m_rotate -= 5.5f;
			if (m_rotate < -90.0f)
				m_rotate = -90.0f;
			--m_life;
			if (m_life <= 0)
				return false;
		}
		return true;
	}

	virtual void Render() {
		float x = m_body.position.x;
		float y = m_body.position.y;
		if (m_state == BukkitScored) {
			m_pimgScored->RenderCentered(gfx::LayerMiddle, x, y);
		} else if (m_state == BukkitEaten) {
			m_pimgEaten->RenderCenteredTransformed(gfx::LayerMiddle, x, y, m_pimgEaten->Width(), m_pimgEaten->Height(), DegToRad(m_rotate), 1.0f);
		} else {
			m_pImage->RenderCentered(gfx::LayerMiddle, x, y);
		}
	}	

	enum BukkitState { BukkitEaten, BukkitFull, BukkitScored };
	
	int         m_life;
	float       m_rotate;
	BukkitState m_state;
	gfx::Image* m_pImage;
	gfx::Image* m_pimgEaten;
	gfx::Image* m_pimgScored;
};


} // namespace kt

#endif // __EBRICK_INCLUDED__
