#ifndef __KT_SYSTEM_INCLUDED__
#define __KT_SYSTEM_INCLUDED__

#include "basictypes.hpp"
#include "display.hpp"
#include "input.hpp"
#include <OpenLayer.hpp>

namespace kt {

class Game;


/*
 * System: the application
 */
class KTSystem {
public:
	KTSystem();
	~KTSystem();
	void Init();
	void Shutdown();
	void Error(PCSZ pszFormat, ...);
	void Warning(PCSZ pszFormat, ...);
	void Log(PCSZ pszFormat, ...);
	void Run();
	void Quit();
	void NeedRender();
	void Print(int x, int y, PCSZ pszFormat, ...);
	void PrintBig(int x, int y, PCSZ pszFormat, ...);
	bool HasSound() { return m_bSound; }
	
	inline int TicksPerSec() const {
		return (int)m_rDesiredFPS;
	}

	/* Data */

	gfx::Display		Display;
	InputManager		Input;
	ol::TextRenderer*	Font;
	ol::TextRenderer*   BigFont;
	bool                m_bSound;
	
protected:
	void Render();
	void Tick();


	bool		m_bQuit;
	bool		m_bNeedRender;

	// timing
	real		m_rInterval;		// interval (in game ticks) since last cycle
	timestamp	m_tsTime;			// current game time; number of ticks since startup
	real		m_rDesiredFPS;		// FPS we are currently (trying to) tick and render at

	Game*		m_pGame;
};

/* The System */
extern KTSystem System;

}


#endif // __KT_SYSTEM_INCLUDED__
