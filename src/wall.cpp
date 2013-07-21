#include <OpenLayer.hpp>
#include "game-world.hpp"
#include "wall.hpp"
#include "system.hpp"

using namespace kt;


Wall::Wall(GameWorld* pw, Vec2D start, uint cxSegs, uint cySegs) :
		m_pWorld(pw), m_start(start), m_cxSegs(cxSegs), m_cySegs(cySegs)
{
	m_pImage = System.Display.GetImage("wallseg.png");
	float w = cxSegs * m_pImage->Width();
	float h = cySegs * m_pImage->Height();
	m_body.Set(Vec2(w, h), FLT_MAX);
	m_body.position.Set(start.x + w/2.0f, start.y - h/2.0f);
	m_pWorld->B2DWorld()->Add(&m_body);
	System.Log("Wall: %g, %g (size: %g, %g)", m_body.position.x, m_body.position.y,
			  m_body.width.x, m_body.width.y);
}


void Wall::Render() {
	float cxTile = (float)m_pImage->Width();
	float cyTile = (float)m_pImage->Height();
	Vec2D pos(m_body.position.x - (m_body.width.x*0.5f),
			  m_body.position.y + (m_body.width.y*0.5f));
	for (uint y = 0; y < m_cySegs; ++y) {
		for (uint x = 0; x < m_cxSegs; ++x) {
			m_pImage->Render(gfx::LayerMiddle, pos.x, pos.y);
			pos.x += cxTile;
		}
		pos.y -= cyTile;
		pos.x = m_body.position.x - (m_body.width.x*0.5f);
	}
}

	
void Wall::Tick() {
		
	
}
