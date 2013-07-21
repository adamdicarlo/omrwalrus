#include "goal.hpp"
#include "display.hpp"
#include "system.hpp"


namespace kt {

// game space coords
Goal::Goal() : m_pImage(NULL)
{

}


Goal::~Goal()
{
}

bool Goal::IsInside(Vec2D test) {
	float hr = m_radius * 0.5f;
	float mx = m_pos.x;
	float my = m_pos.y;
	float x = test.x;
	float y = test.y;
	
	if ((x < mx-hr) || (x > mx+hr) || (y < my-hr) || (y > my+hr)) {
	} else {
		return true;
	}
	
	return false;
}


// creates goal given world space coords
void Goal::Set(float x, float y, float radius) {	
	m_pImage = System.Display.GetImage("goal.png");
	m_pos.x = x;
	m_pos.y = y;
	m_radius = radius;
	m_opacity = 1.0f;
	m_deltaOpacity = -0.009f;
}

/*!
    \fn kt::Goal::Render()
 */
void Goal::Render()
{
	m_pImage->RenderCenteredTransformed(
		gfx::LayerFar, m_pos.x, m_pos.y,
 		m_radius, m_radius, 0.0f, m_opacity
	);
}

void Goal::Tick() {
	const float min = 0.35f;
	m_opacity += m_deltaOpacity;
	if (m_opacity <= min || m_opacity >= 1.0f) {
		m_deltaOpacity = -m_deltaOpacity;
	}
	if (m_opacity < min)
		m_opacity = min;
	if (m_opacity > 1.0f)
		m_opacity = 1.0f;
}


} // namespace kt
