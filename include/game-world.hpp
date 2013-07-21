#ifndef __KT_GAME_WORLD_INCLUDED__
#define __KT_GAME_WORLD_INCLUDED__

#include <allegro.h> // SAMPLE
#include "Box2D/World.h"
#include "entity.hpp"
#include "goal.hpp"
#include "scheduler.hpp"
#include "wall.hpp"
#include "system.hpp"

#define DegToRad(x)				((x) * k_pi / 180.0f)

namespace kt {

namespace gfx { class Image; }
	
class EBall;
class EBukkit;
class ECat;
class EPusher;
class EWalrus;
class EMat;
class GameView;


class GameWorld {
public:
	GameWorld();
	~GameWorld();
	void Init();
	void Clear();
	void Shutdown();
	void Render();
	void Tick(timestamp tsTime);
	bool IsPaused() const { return m_bIsPaused; }
	void SetPaused(bool bSet) { m_bIsPaused = bSet; }
	ProcessScheduler* Scheduler() { return &m_scheduler; }
	World* B2DWorld() { return &m_b2dWorld; }
	void Load(uint iLevel);
	void AddWall(Vec2D start, uint cx, uint cy);
	void DrawEntityList(EntityList& ls);
	void PushEntities(EntityList& list, EPusher* pp);
	EntityPtr AddBukkit();
	EntityPtr AddWalrus();
	EntityPtr AddBall();
	EntityPtr AddMat();
	EntityPtr AddPusher();
	
	void PlayShootSound() {	PlaySound(m_psndShoot); }
	void PlayGruntSound() { PlaySound(m_psndGrunt); }
	void PlayScoreSound() { PlaySound(m_psndScore); }
	void PlayTastySound() { PlaySound(m_psndTasty); }
	
	void PlaySound(SAMPLE* psnd) {
		ASSERT(psnd);
		if (System.HasSound())
			play_sample(psnd, 255, 128, 1000, 0);
	}
	
	 int m_score;
	 int m_timeLeft;

	 enum GameState { StateTitle, StatePlay, StateGameOver };
	 GameState   m_state;
private:
	
	
	bool FindEntity(EntityList& list, Body* pb, EntityPtr& ptr);
	SAMPLE* LoadSound(PCSZ pszName);

	SAMPLE* m_psndShoot,*m_psndGrunt,*m_psndScore,*m_psndTasty;
	
	friend class GameView;
	ECat*             m_pCat;
	EntityList        m_lsPushers;
	Goal              m_goal;
	World             m_b2dWorld;
	WallList          m_lsWalls;
	EntityList        m_lsBalls;
	EntityList        m_lsBukkits;
	EntityList        m_lsMats;
	EntityList        m_lsWalruses;
	ProcessScheduler  m_scheduler;
	bool              m_bIsPaused;
	gfx::Image*       m_pimgIce;
};


} // namespace kt

#endif // __KT_GAME_WORLD_INCLUDED__
