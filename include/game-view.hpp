#ifndef __KT_GAME_VIEW_INCLUDED__
#define __KT_GAME_VIEW_INCLUDED__

#include "input.hpp"
#include "scheduler.hpp"

namespace kt {

class GameWorld;
class IScreenElement;
typedef std::list<IScreenElement*> IScreenElementList;


class IGameView : public IInputListener {
public:
	virtual ~IGameView() { }
	virtual void Render() = 0;
	virtual void Tick(timestamp) = 0;
	virtual bool IsActive() const = 0;
};


class GameView : public IGameView {
public:
	virtual ~GameView();
	virtual void Init(GameWorld* pw);
	virtual bool HandleInput(const InputEvent* pinev);
	virtual void Render();
	virtual void Tick(timestamp tsTime);
	virtual bool IsActive() const;

private:
	GameWorld*			m_pWorld;
	IScreenElementList	m_lsScreens;
	ProcessScheduler*	m_pScheduler;
	bool				m_bIsActive;
};


} // namespace kt

#endif // __KT_GAME_VIEW_INCLUDED__
