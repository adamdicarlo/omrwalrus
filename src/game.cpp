#include "basictypes.hpp"
#include "system.hpp"
#include "game.hpp"
#include "game-view.hpp"

using namespace kt;


Game::~Game() {
}



void Game::Init() {
	m_world.Init();

	GameView* pView = new GameView();
	pView->Init(&m_world);

	m_lsViews.push_back(pView);
}


void Game::Shutdown() {
	IGameViewList::iterator i;
	for (i = m_lsViews.begin(); i != m_lsViews.end(); ++i) {
		delete (*i);
	}
	m_lsViews.clear();	
	m_world.Shutdown();
}


void Game::Render() {
	IGameViewList::iterator i;
	for (i = m_lsViews.begin(); i != m_lsViews.end(); ++i) {
		(*i)->Render();
	}

	
}


void Game::Tick(timestamp tsTime) {
	IGameViewList::iterator i;
	for (i = m_lsViews.begin(); i != m_lsViews.end(); ++i) {
		(*i)->Tick(tsTime);
	}
}



