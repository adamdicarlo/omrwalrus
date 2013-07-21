#ifndef __ECAT_INCLUDED__
#define __ECAT_INCLUDED__

#include "commands.hpp"
#include "entity.hpp"
#include "display.hpp"
#include "game-world.hpp"
#include "image.hpp"
#include "input.hpp"
#include "system.hpp"

#define CAT_MASS				12000.0f /* not used */
#define BALL_RELOAD				30
#define MAT_RELOAD				60
#define BASE_FORCE				200.0f
#define MAX_FORCE				1400.0f
#define ARROW_X_OFS				24.0f
#define ARROW_Y_OFS				(-12.0f)
#define MAX_ARROW_FACTOR		2.0f


namespace kt {


class ECat : public Entity, public IInputListener {
public:
	ECat(GameWorld *pw) : Entity(pw) {
		m_type = EntityCat;
		m_pimgIdle = m_pimgAim = m_pimgThrow = NULL;
		System.Input.InsertListener(this);
		System.Input.Bind(DeviceKeyboard, KEY_RIGHT, CMD_AIMRIGHT);
		System.Input.Bind(DeviceKeyboard, KEY_LEFT, CMD_AIMLEFT);
		System.Input.Bind(DeviceKeyboard, KEY_SPACE, CMD_FIRE);
		System.Input.Bind(DeviceKeyboard, KEY_LSHIFT, CMD_NEXTITEM);
	}

	virtual ~ECat() {
		m_pWorld->B2DWorld()->Remove(&m_body);
	}

	virtual void Init() {
		m_pimgIdle  = System.Display.GetImage("cat1.png");
		m_pimgAim   = System.Display.GetImage("cat3.png");
		m_pimgThrow = System.Display.GetImage("cat2.png");
		m_pimgHudBall  = System.Display.GetImage("hudball.png");
		m_pimgHudMat   = System.Display.GetImage("hudmat.png");
		m_pimgSelector = System.Display.GetImage("selector.png");
		m_pimgArrow    = System.Display.GetImage("arrow.png");
		
		m_selopacity = 1.0f;
		m_deltaOpacity = -0.009f;
		m_aimForce     = BASE_FORCE;
		m_aimAngle     = 90.0f;
		m_aim          = AimNone;
		m_state        = StateIdle;
		m_weapon       = WeaponBall;
		m_cBalls       = 100;
		m_cMats        = 10;
		
		// do not add body to the world
		m_body.Set(Vec2((float)m_pimgIdle->Width() - 2.0f, (float)m_pimgIdle->Height() - 2.0f), CAT_MASS);
		//m_pWorld->B2DWorld()->Add(&m_body);
	}

	virtual bool Tick(timestamp) {
		const float forceRate = 35.0f;
		float aimSpeed = 2.0f;
		
		if (m_aim == AimRight)
			m_aimAngle += aimSpeed;
		else if (m_aim == AimLeft)
			m_aimAngle -= aimSpeed;
		
		switch (m_state) {
			case StateIdle:
				break;
				
			case StateWindup:
				m_aimForce += forceRate;
				if (m_aimForce >= MAX_FORCE)
					m_aimForce = MAX_FORCE;
				break;
			case StateThrow:
				m_aimForce -= forceRate;
				m_aimForce = Max(BASE_FORCE, m_aimForce);
				m_reload--;
				if (m_reload <= 0) {
					m_state = StateIdle;
				}
				break;
			default: break;
		}
		
		// SELECTOR
		m_selopacity += m_deltaOpacity;
		if (m_selopacity > 1.0f) {
			m_selopacity = 1.0f;
			m_deltaOpacity = -m_deltaOpacity;
		} else if (m_selopacity < 0.35f) {
			m_selopacity = 0.35f;
			m_deltaOpacity = -m_deltaOpacity;
		}
		return true;
	}

	void Throw() {
		EntityPtr projectile;
		m_state = StateThrow;
		m_pWorld->PlayShootSound();
		
		if (m_weapon == WeaponBall) {	
			m_reload = BALL_RELOAD;
			projectile = m_pWorld->AddBall();
			
		}
		else if (m_weapon == WeaponMat) {
			m_reload = MAT_RELOAD;
			projectile = m_pWorld->AddMat();
			m_aimForce *= 1.2;
		}
		
		float angle = -m_aimAngle;
		projectile->SetPos(Vec2D(m_body.position.x + ARROW_X_OFS, m_body.position.y + ARROW_Y_OFS));
		projectile->SetVel(Vec2D(cos(DegToRad(angle)) * m_aimForce,
						   sin(DegToRad(angle)) * m_aimForce));
	}
		
	virtual bool HandleInput(const InputEvent* pinev) {
		if (m_pWorld->m_state != GameWorld::StatePlay)
			return false;
		
		if (pinev->type == INPUT_BUTTONUP) {
			//System.Log("Button up: %d", pinev->cmd);
			if (pinev->cmd == CMD_FIRE && m_state == StateWindup) {
				Throw();
				m_aim  = AimNone;
				return true;
			}
			if (pinev->cmd == CMD_AIMRIGHT && m_aim == AimRight) {
				m_aim = AimNone;
				return true;
			}
			if (pinev->cmd == CMD_AIMLEFT && m_aim == AimLeft) {
				m_aim = AimNone;
				return true;
			}
			return false;
			
		} else {
			//System.Log("Button down: %d", pinev->cmd);			
			switch (pinev->cmd) {
				case CMD_AIMLEFT:
					m_aim = AimLeft;
					break;
				case CMD_AIMRIGHT:
					m_aim = AimRight;
					break;
				case CMD_FIRE:
					if (m_state == StateIdle) {
						m_state = StateWindup;
					}
					break;
				case CMD_NEXTITEM:
					if (m_reload > 0)
						break;
					if (m_weapon == WeaponBall) {
						m_weapon = WeaponMat;
					} else if (m_weapon == WeaponMat) {
						m_weapon = WeaponBall;
					}
					break;
				default:
					return false; 
					break;	
			}
		}
		return true;
	}

	virtual void Render() {
		float x = m_body.position.x;
		float y = m_body.position.y;
		gfx::Image* pimg;
		
		switch (m_state) {
			case StateIdle:
				pimg = m_pimgIdle;
				break;
			case StateWindup:
				pimg = m_pimgAim;
				break;
			case StateThrow:
				pimg = m_pimgThrow;
				break;
			default:
				pimg = m_pimgIdle;
				break;
		}
		if (pimg == m_pimgAim) // aim bitmap is a bit wider than others
			pimg->RenderCentered(gfx::LayerClose, x + 5.0f, y);
		else
			pimg->RenderCentered(gfx::LayerClose, x, y);
		
		float hudgap = 120.0f;
		if (m_weapon == WeaponBall) {
			m_pimgHudBall->RenderCentered(gfx::LayerClosest, x + hudgap, y);
			m_pimgHudMat->RenderCentered(gfx::LayerClosest, x + hudgap + m_pimgHudBall->Width() + 8.0f, y); 
		}
		else if (m_weapon == WeaponMat) {
			m_pimgHudMat->RenderCentered(gfx::LayerClosest, x + hudgap, y);
			m_pimgHudBall->RenderCentered(gfx::LayerClosest, x + hudgap + m_pimgHudMat->Width() + 8.0f, y);
		}
		m_pimgSelector->RenderCentered(gfx::LayerClosest, x + hudgap, y, m_selopacity);
		
		/*
		 * Draw aim arrow
		 */
		float cxArrow = m_pimgArrow->Width();
		float cyArrow = m_pimgArrow->Height();
		float forceScale = m_aimForce / (MAX_FORCE - BASE_FORCE);
		float cxZoomed = cxArrow * (1.0f + (forceScale * (MAX_ARROW_FACTOR-1.0f)));
		float cyZoomed = cyArrow;
		
		m_pimgArrow->RenderTransformed(
				gfx::LayerClosest, x + ARROW_X_OFS, y + ARROW_Y_OFS,
				0.0f, cyArrow * 0.5f,     // pivot
				cxZoomed, cyZoomed,
				DegToRad(m_aimAngle), 0.75f);
	}	

	enum CatState { StateIdle, StateWindup, StateThrow };
	enum Weapon   { WeaponBall, WeaponMat };
	enum Aim      { AimNone, AimRight, AimLeft };
	
	Weapon      m_weapon;
	CatState    m_state;
	Aim			m_aim;
	int			m_cBalls;
	int			m_cMats;
	int         m_reload;
	float       m_selopacity;
	float       m_deltaOpacity;
	float       m_aimForce;
	float       m_aimAngle;
	gfx::Image* m_pimgArrow;
	gfx::Image* m_pimgSelector;
	gfx::Image* m_pimgHudBall;
	gfx::Image* m_pimgHudMat;
	gfx::Image* m_pimgIdle;
	gfx::Image* m_pimgAim;
	gfx::Image* m_pimgThrow;
};


} // namespace kt

#endif // __ECAT_INCLUDED__
