#ifndef __EMAT_INCLUDED__
#define __EMAT_INCLUDED__

#include "entity.hpp"
#include "game-world.hpp"
#include "image.hpp"
#include "system.hpp"

#define MAT_MASS				25.0f

namespace kt {


class EMat : public Entity {
public:
	EMat(GameWorld *pw) : Entity(pw) {
		m_pImage = NULL;
		m_type = EntityMat;
	}

	virtual ~EMat() {
	}

	virtual void Init() {
		m_pImage = System.Display.GetImage("hudmat.png");
		m_body.Set(Vec2((float)m_pImage->Width() * 4.0f, (float)m_pImage->Height() * 4.0f), MAT_MASS);
		//m_pWorld->B2DWorld()->Add(&m_body);
		m_life = (int) (System.TicksPerSec() * Random(20.0f, 55.0f));
	}

	virtual bool Tick(timestamp) {
		--m_life;
		if (m_life <= 0)
			return false;
		return true;
	}

	virtual void Render() {
		float x = m_body.position.x;
		float y = m_body.position.y;
		m_pImage->RenderCenteredTransformed(gfx::LayerFar, x, y, m_pImage->Width()*2.8f, m_pImage->Height()*2.8f, 0.0f, 0.9f);
	}	

	
	int         m_life;
	gfx::Image* m_pImage;
};


} // namespace kt

#endif // __EMAT_INCLUDED__

