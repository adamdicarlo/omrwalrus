#ifndef __KT_WALRUS_INCLUDED__
#define __KT_WALRUS_INCLUDED__

#include "display.hpp"
#include "ebukkit.hpp"
#include "entity.hpp"
#include "image.hpp"
#include "system.hpp"

#define WALRUS_MASS			60.0f
#define SHOCK_SECS			5.0f

namespace kt {

class EWalrus : public Entity {
public:
	EWalrus(GameWorld *pw) : Entity(pw) {
		m_type = EntityWalrus;
	}
	virtual ~EWalrus() {
		m_pWorld->B2DWorld()->Remove(&m_body);
	}
	
	virtual void Init() {
		m_pimgLay = System.Display.GetImage("walrus1.png");
		m_pimgSlide = System.Display.GetImage("walrus2.png");
		m_pimgShock = System.Display.GetImage("walshock.png");
		m_state = StateLay;
		m_cRemTicks = System.TicksPerSec() * 2;
		m_body.Set(Vec2((float)m_pimgLay->Width() - 3.0f, (float)m_pimgLay->Height() - 2.0f), WALRUS_MASS);
		m_body.friction = 0.2f;
		m_pWorld->B2DWorld()->Add(&m_body);
		m_cDelayTicks = (int)Random(System.TicksPerSec() * 0.5f, System.TicksPerSec() * 3.0f);
	}

	virtual void Render() {
		float x = m_body.position.x;
		float y = m_body.position.y;
		switch (m_state) {
			case StateLay:
				m_pimgLay->RenderCentered(gfx::LayerClose, x, y);
				break;
			case StateSlideStart:
				m_pimgSlide->RenderCentered(gfx::LayerClose, x, y);
				break;
				
			case StateSlideEnd:
				m_pimgSlide->RenderCentered(gfx::LayerClose, x, y);
				break;
				
			case StateShock:
				m_pimgShock->RenderCentered(gfx::LayerClose, x, y);
				break;
			default:
				ASSERT(false);
				break;
		}
	}

	virtual void Struck(counted_ptr<Entity>& struck) {
		if (struck->m_type == EntityBukkit) {
			if (m_state == StateShock) // can't eat while shocked
				return;
			
			EBukkit* bukkit = (EBukkit*) struck.get();
			if (bukkit->m_state == EBukkit::BukkitScored)
				return;
			
			if (bukkit->m_state == EBukkit::BukkitFull) {
				m_pWorld->PlayTastySound();
			}

			bukkit->m_state = EBukkit::BukkitEaten;
		} else if (struck->m_type == EntityBall) {
			if (m_state != StateShock)
				m_pWorld->PlayGruntSound();
			m_pWorld->m_score += 500;
			m_state = StateShock;
			m_cRemTicks = (int) (System.TicksPerSec() * SHOCK_SECS);
		}
	}
	
	virtual bool Tick(timestamp) {
		m_body.angularVelocity = 0.0f;
		m_body.rotation = 0.0f;
		if (--m_cRemTicks <= 0) {
			switch (m_state) {
				case StateLay:
					m_body.velocity.x = Random(-150.0f, -90.0f);
					m_body.velocity.y;
					m_state = StateSlideStart;
					m_cRemTicks = m_cDelayTicks;
					break;
					
				case StateSlideStart:
					m_state = StateSlideEnd;
					m_cRemTicks = System.TicksPerSec() * 1;
					break;
					
				case StateSlideEnd:
					m_state = StateLay;
					m_cRemTicks = System.TicksPerSec() * 1;
					break;
					
				case StateShock:
					m_state = StateLay;
					m_cRemTicks = System.TicksPerSec() * 1;
					break;
				default:
					ASSERT(false); // bad walrus
					break;
			}
		}
		return true;
	}


	enum WalrusState { StateLay, StateSlideStart, StateSlideEnd, StateShock };
	WalrusState m_state;
	int         m_cRemTicks; // ticks until changing states
	int         m_cDelayTicks; // normal delay ticks...how fast (slow) of a walrus is this?
	gfx::Image* m_pimgLay;
	gfx::Image* m_pimgSlide;
	gfx::Image* m_pimgShock;
};

} // namespace kt

#endif // __KT_WALRUS_INCLUDED__
