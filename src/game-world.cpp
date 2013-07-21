#include "basictypes.hpp"
#include "system.hpp"
#include "entity.hpp"
#include "goal.hpp"
#include "eball.hpp"
#include "ecat.hpp"
#include "ebukkit.hpp"
#include "emat.hpp"
#include "epusher.hpp"
#include "ewalrus.hpp"
#include "game-world.hpp"
#include "walrus-generator.hpp"
using namespace kt;

#define PHYSICS_ITERATIONS		10


GameWorld::GameWorld() :
		m_pCat(NULL), m_b2dWorld(Vec2(0.0f, 0.0f), PHYSICS_ITERATIONS)
{
	m_pimgIce = System.Display.GetOpaqueImage("ice1.bmp");
	if (System.HasSound()) {
		m_psndShoot = LoadSound("data/shoot.wav");
		m_psndGrunt = LoadSound("data/grunt.wav");
		m_psndTasty = LoadSound("data/tasty.wav");
		m_psndScore = LoadSound("data/score.wav");
	}
}

SAMPLE* GameWorld::LoadSound(PCSZ pszName) {
	SAMPLE* psnd = load_sample(pszName);
	if (!psnd)
		System.Error("couldn't load '%s'", pszName);
	return psnd;
}

GameWorld::~GameWorld() {
}



void GameWorld::Init() {
	m_pCat = new ECat(this);
	m_state = StateTitle;
}


void GameWorld::Shutdown() {
	delete m_pCat;
}



/*!
    \fn kt::GameWorld::AddWall(Vec2D start, uint cx, uint cy)
 */
void kt::GameWorld::AddWall(Vec2D start, uint cx, uint cy)
{
	m_lsWalls.push_back(new Wall(this, start, cx, cy));
}

EntityPtr GameWorld::AddBukkit() {
	EntityPtr pb = EntityPtr(new EBukkit(this));
	pb->Init();
	m_lsBukkits.push_back(pb);
	return pb;
}

EntityPtr GameWorld::AddWalrus() {
	EntityPtr pWally = EntityPtr(new EWalrus(this));
	pWally->Init();
	m_lsWalruses.push_back(pWally);
	return pWally;
}

EntityPtr GameWorld::AddBall() {
	EntityPtr pBall = EntityPtr(new EBall(this));
	pBall->Init();
	m_lsBalls.push_back(pBall);
	return pBall;
}


EntityPtr GameWorld::AddMat() {
	EntityPtr pMat = EntityPtr(new EMat(this));
	pMat->Init();
	m_lsMats.push_back(pMat);
	return pMat;
}


EntityPtr GameWorld::AddPusher() {
	EntityPtr p = EntityPtr(new EPusher(this));
	p->Init();
	m_lsPushers.push_back(p);
	return p;
}


void GameWorld::Load(uint iLevel) {
	float cx = (float)System.Display.Width();
	float cy = (float)System.Display.Height();
	float radius = 192;
	for (int angle = 0; angle < 360; angle += 20) {
			EntityPtr pBukkit = AddBukkit();
			pBukkit->SetPos(
				Vec2D(
					  (cx/2.6f) + cos(DegToRad(angle))*radius,
					-((cy/2.4f) + sin(DegToRad(angle))*radius))
			);
	
	}
	float pad = 16.0f;
	float segwidth = 32.0f;
	float segheight = 32.0f;
	float goalradius = 256.0f;
	float halfcatheight = 32.0f;
	int cxSegs = (int) ((cx-segwidth-pad*3) / segwidth + 4); // go off right edge of screen
	int cySegs = (int) ((cy-segheight-pad*3) / segheight);

	EntityPtr pPusher = AddPusher();
	pPusher->SetPos(Vec2D(0, 0));
	pPusher = AddPusher();
	pPusher->SetPos(Vec2D(cx , -cy));
	
	WalrusGeneratorPtr cpGenerator(new WalrusGenerator(16));
	m_scheduler.InsertProcess(cpGenerator, (void*)this);
	AddWall(Vec2D(pad + segwidth, -pad), cxSegs, 1);
	AddWall(Vec2D(pad, -(pad + segheight)), 1, cySegs);
	AddWall(Vec2D(pad + segwidth, -(cy-pad-segheight)), cxSegs, 1);
	
	// offscreen wall for boundary
	AddWall(Vec2D(cx + (segwidth*4), -(pad + segheight)), 1, cySegs);
	
	m_pCat->Init();
	m_pCat->SetPos(Vec2D(cx/2.0f, -(pad + halfcatheight - 3.0f)));
	m_goal.Set(pad + segwidth + goalradius/2.0, -(cy-segheight-pad*2-goalradius/2.0), goalradius);
	
	m_timeLeft = (int) (60 * 120);
	m_score = 0;
	m_state = StatePlay;
}

void GameWorld::Clear() {
	m_b2dWorld.Clear();
	m_scheduler.Clear();
	m_lsWalls.clear();
	m_lsWalruses.clear();
	m_lsBalls.clear();
	m_lsBukkits.clear();
	m_lsMats.clear();
	m_lsPushers.clear();
	
}


void GameWorld::DrawEntityList(EntityList& ls) {
	EntityList::iterator i;
	for (i = ls.begin(); i != ls.end(); ++i) {
		EntityPtr& ptr = *i;
		ptr->Render();
	}

}

void GameWorld::Render() {
	if (m_state == StateTitle) {
		static ol::Rgba color = ol::Rgba::WHITE;
		gfx::Image* pimgCat = System.Display.GetImage("titlecat.png");
		ol::Canvas::Fill(color);
		pimgCat->m_pbmp->Blit(440.0f, 440.0f);
		System.PrintBig(210, 80, "O MR WALRUS... r u lookin 4 sumthin???");
		System.Print(350, 130, "Press SPACE to shoot, use arrow keys");
		System.Print(350, 160, "to aim, SHIFT to switch items.  Get as");
		System.Print(350, 190, "many bukkits as you can in the goal zone");
		System.Print(350, 220, "before time's up. Green mats will slow");
		System.Print(350, 250, "down walruses.  Your mission, evil cat:");
		System.Print(350, 290, "DEPRIVE THE helpless walruses OF THEIR BUKKITS!");
		System.Print(400, 330, "Press ENTER to start");
		System.Print(50, System.Display.Height() - 50, "F1: Fullscreen | F2: Screenshot");
		return;
	}
	
	ASSERT(m_pCat);
	
	uint nWidth = m_pimgIce->Width();
	uint nHeight = m_pimgIce->Height();
	uint cxTiles = (System.Display.Width()+nWidth-1) / nWidth;
	uint cyTiles = (System.Display.Height()+nHeight-1) / nHeight;
	for (uint y = 0; y < cyTiles; ++y) {
		for (uint x = 0; x < cxTiles; ++x) {
			m_pimgIce->RenderSC(gfx::LayerFarthest, x * nWidth, y * nHeight);	
		}
	}

	DrawEntityList(m_lsWalruses);
	DrawEntityList(m_lsBukkits);
	DrawEntityList(m_lsBalls);
	DrawEntityList(m_lsMats);
	DrawEntityList(m_lsPushers);
		
	WallList::iterator wit;
	for (wit = m_lsWalls.begin(); wit != m_lsWalls.end(); ++wit) {
		(*wit)->Render();
	}
	
	m_pCat->Render();
	m_goal.Render();
	System.Display.FlushBlits();
	ol::Rect box(8, 8, 384, 68, 4.0f, 8.0f);
	box.Draw(Rgba::WHITE);
	box.DrawOutline(Rgba::BLACK);
	System.Print(32, 32, "Score: %d", m_score);
	System.Print(32, 64, "Time Left: %d", m_timeLeft);

	if (m_state == StateGameOver) {
		System.PrintBig(450, 300, "GAME OVER");
	}
}

void GameWorld::PushEntities(EntityList& list, EPusher* pp) {
	EntityList::iterator i;
	float px = pp->m_body.position.x;
	float py = pp->m_body.position.y;
	System.Log("Hey.. radius: %g", pp->m_radius);
	for (i = list.begin(); i != list.end(); ++i) {
		float distance;
		float x, y, dx, dy;
		EntityPtr ent = *i;
		x = ent->m_body.position.x;
		y = ent->m_body.position.y;
		dx = px - x;
		dy = py - y; 
		distance = sqrt(dx*dx + dy*dy);
		
		System.Log("dist: %g", distance);
		if (distance <= pp->m_radius) {
			ent->m_body.velocity.x = (pp->m_maxRadius - distance) * -dx / 100.0f;
			ent->m_body.velocity.y = (pp->m_maxRadius - distance) * -dy / 100.0f;
		}
	}
}


void GameWorld::Tick(timestamp tsTime) {

	if (m_state == StateTitle) {
		return;
	} else if (m_state == StateGameOver) {
		return;
	}
	
	float timeStep = 1.0f / System.TicksPerSec();
	float friction = 0.98f;
	float sticky_friction = 0.90f;
	float mat_friction    = 0.97f;
	
	EntityList::iterator i;
	
	for (i = m_lsBalls.begin(); i != m_lsBalls.end(); ) {
		EntityPtr ball = *i;
		if (!ball->Tick(tsTime)) {
			i = m_lsBalls.erase(i);
			continue;
		} else {
			ball->m_body.velocity.x *= friction;
			ball->m_body.velocity.y *= friction;
			
			++i;
		}
	}
	
	
	for (i = m_lsWalruses.begin(); i != m_lsWalruses.end(); ) {
		EntityPtr wally = *i;
		
		if (!wally->Tick(tsTime)) {
			i = m_lsWalruses.erase(i);
			continue;
		}
		
		wally->m_body.angularVelocity = 0.0f;
		
		// CHECK if on stickymat
		float x = wally->m_body.position.x;
		float y = wally->m_body.position.y;
		EntityList::iterator mit;
		bool bSticky = false;
		for (mit = m_lsMats.begin(); mit != m_lsMats.end(); ++mit) {
			EntityPtr mat = *mit;
			float mx = mat->m_body.position.x;
			float my = mat->m_body.position.y;
			float mw = mat->m_body.width.x * 0.5f;
			float mh = mat->m_body.width.y * 0.5f;
			if ((x < mx-mw) || (x > mx+mw) || (y < my-mh) || (y > my+mh)) {
			} else {
				// on sticky mat
				bSticky = true;
				break;
			}
		}
		
		if (bSticky) {
			wally->m_body.velocity.x *= sticky_friction;
			wally->m_body.velocity.y *= sticky_friction;	
		} else {
			wally->m_body.velocity.x *= friction;
			wally->m_body.velocity.y *= friction;
		}
		++i;
	}
	
	/* Update BUKKITS */
	EntityList::iterator bit;
	for (bit = m_lsBukkits.begin(); bit != m_lsBukkits.end(); ) {
		EntityPtr bukkit = *bit;
		
		bukkit->m_body.velocity.x *= friction;
		bukkit->m_body.velocity.y *= friction;
		bukkit->m_body.angularVelocity = 0.0f;

		if (m_goal.IsInside(Vec2D(bukkit->m_body.position.x,bukkit->m_body.position.y))) {
			EBukkit* pb = (EBukkit*) bukkit.get();
			pb->InGoal();
		}
		
		if (!bukkit->Tick(tsTime)) {
			bit = m_lsBukkits.erase(bit);
		} else {
			++bit;
		}	
	}
	
	/* Update sticky mats */
	for (bit = m_lsMats.begin(); bit != m_lsMats.end(); ) {
		EntityPtr mat = *bit;
		
		mat->m_body.position.x += mat->m_body.velocity.x * timeStep;
		mat->m_body.position.y += mat->m_body.velocity.y * timeStep;
		mat->m_body.velocity.x *= mat_friction;
		mat->m_body.velocity.y *= mat_friction;
	
		/* Collision check this mat against all the walls */
		float x = mat->m_body.position.x;
		float y = mat->m_body.position.y;
		WallList::iterator wit;
		for (wit = m_lsWalls.begin(); wit != m_lsWalls.end(); ++wit) {
			Wall* wall = *wit;
			float wx, wy, ww, wh;
			wx = wall->m_body.position.x;
			wy = wall->m_body.position.y;
			ww = wall->m_body.width.x * 0.5f;
			wh = wall->m_body.width.y * 0.5f;
			if ((x < wx-ww) || (x > wx+ww) || (y < wy-wh) || (y > wy+wh)) {
			} else {
				// hit wall... back 'er up!
				mat->m_body.position.x -= mat->m_body.velocity.x * timeStep;
				mat->m_body.position.y -= mat->m_body.velocity.y * timeStep;
				
				// adjust velocity (bounce)
				if (x >= wx-ww || x <= wx+ww)
					mat->m_body.velocity.y = -mat->m_body.velocity.y;
				if (y >= wy-wh || y <= wy+wh)
					mat->m_body.velocity.x = -mat->m_body.velocity.x;
				break;
			}
		}

		// Make mat think
		if (!mat->Tick(tsTime)) {
			bit = m_lsMats.erase(bit);
		} else {
			++bit;
		}
	}
	
	WallList::iterator wit;
	for (wit = m_lsWalls.begin(); wit != m_lsWalls.end(); ++wit) {
		(*wit)->Tick();
	}

	EntityList::iterator pit ;
	for (pit = m_lsPushers.begin(); pit != m_lsPushers.end(); ++pit) {
		EPusher* pp = (EPusher*) (*pit).get();
		pp->Tick(tsTime);
		PushEntities(m_lsBalls, pp);
		PushEntities(m_lsBukkits, pp);
		PushEntities(m_lsWalruses, pp);
		PushEntities(m_lsMats, pp);
	}
	
	m_pCat->Tick(tsTime);
	m_goal.Tick();
	m_scheduler.Tick(tsTime);	
	m_b2dWorld.Step(timeStep);
	
	/* NOW poke around in Box2D internals! */
	std::map<ArbiterKey, Arbiter>::iterator ait;

	for (ait = m_b2dWorld.arbiters.begin(); ait != m_b2dWorld.arbiters.end(); ++ait) {
		Arbiter& arb = ait->second;
		EntityPtr other;
		EntityPtr walrus;
		if ((FindEntity(m_lsBalls, arb.body1, other) &&
		     FindEntity(m_lsWalruses, arb.body2, walrus)) ||
			(FindEntity(m_lsBalls, arb.body2, other) &&
			 FindEntity(m_lsWalruses, arb.body1, walrus))) {
			walrus->Struck(other);
		}
		if ((FindEntity(m_lsBukkits, arb.body1, other) &&
			 FindEntity(m_lsWalruses, arb.body2, walrus)) ||
			(FindEntity(m_lsBukkits, arb.body2, other) &&
			 FindEntity(m_lsWalruses, arb.body1, walrus))) {
			//System.Log("3, 2, 1 CONTACT");
			walrus->Struck(other);
	   }
	}
	
	--m_timeLeft;
	if (m_timeLeft <= 0) {
		m_state = StateGameOver;
	}
}

bool GameWorld::FindEntity(EntityList& list, Body* pb, EntityPtr& ptr) {
	EntityList::iterator it;
	for (it = list.begin(); it != list.end(); ++it) {
		EntityPtr check = *it;
		if (&check->m_body == pb) {
			ptr = *it;
			return true;
		}
	}
	return false;
}

