/*
 *  This file is part of the KT Game Engine, Copyright 2007 Adam DiCarlo.
 *  Please see license.txt.
 *
 *  GameView represents the view of a single player in the game.
 */
#include <allegro.h>  // KEY_...
#include "basictypes.hpp"
#include "scheduler.hpp"
#include "commands.hpp"
#include "game-view.hpp"
#include "game-world.hpp"
#include "system.hpp"

using namespace kt;


GameView::~GameView() {

}


/*
 * Initializes a view of the given world
 */
void GameView::Init(GameWorld* pw) {
	ASSERT(pw);

	m_pScheduler = new ProcessScheduler;
	if (!m_pScheduler)
		System.Error("GameView can't allocate scheduler");

	System.Input.Bind(DeviceKeyboard, KEY_F1, CMD_TOGGLEFULLSCREEN);
	System.Input.Bind(DeviceKeyboard, KEY_F2, CMD_SCREENSHOT);
	System.Input.Bind(DeviceKeyboard, KEY_ESC, CMD_QUIT);
	System.Input.Bind(DeviceKeyboard, KEY_ENTER, CMD_STARTGAME);
	
	System.Input.InsertListener(this);
	m_pWorld = pw;
	m_bIsActive = true;
}


/*
 * Implementation of InputListener virtual callback.
 */
bool GameView::HandleInput(const InputEvent* pinev) {
	ASSERT(pinev);
	if (pinev->type != INPUT_BUTTONDOWN)
		return false;
	switch (pinev->cmd) {
		case CMD_TOGGLEFULLSCREEN:
			System.Display.ToggleFullscreen();
			break;
		case CMD_SCREENSHOT:
			System.Display.Screenshot();
			break;
		case CMD_STARTGAME:
			if (m_pWorld->m_state == GameWorld::StateTitle) {
				m_pWorld->Load(1);
				m_pWorld->m_state = GameWorld::StatePlay;
			} else if (m_pWorld->m_state == GameWorld::StateGameOver) {
				m_pWorld->Clear();
				m_pWorld->m_state = GameWorld::StateTitle;
			}
			break;
		case CMD_QUIT:
			System.Quit();
			break;
		default:
			return false;
			break;
	}

	return true;
}


void GameView::Render() {
	m_pWorld->Render();
	//System.Display.FlushBlits();
	
//	int count = m_pWorld->m_b2dWorld.bodies.size();

//	System.Print(0, 0, "Bodies: %d", count);
}


void GameView::Tick(timestamp ts) {
	ASSERT(m_pScheduler);
	m_pScheduler->Tick(ts);
	m_pWorld->Tick(ts);
}


bool GameView::IsActive() const {
	return m_bIsActive;
}
