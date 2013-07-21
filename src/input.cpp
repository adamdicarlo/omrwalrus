#include "basictypes.hpp"
#include <allegro.h>
#include "input.hpp"
#include "system.hpp"


using namespace kt;


/*
 * Static class data initialization
 */
InputManager* InputManager::m_pInstance = NULL;


/*
 * Prototypes: functions implemented in input-device.cpp
 */
void KeyboardLowLevelCallback(int key);
void MouseCallback(int flags);


/*
 * Allegro callback
 */
static void CloseCallback() {
	System.Quit();
}


/*
 * Constructor
 */
InputManager::InputManager() : m_prgDevices(0), m_cDevices(0) {
}


InputManager::~InputManager() {
	delete [] m_prgDevices;
	m_prgDevices = 0;
	m_cDevices = 0;
}


void InputManager::Init() {
	ASSERT(!m_prgDevices);
	m_prgDevices = new kt::InputDevice[2];
	m_cDevices = 2;

	install_keyboard();
	//install_mouse();
	set_keyboard_rate(0, 0);
	set_close_button_callback(&CloseCallback);

	LOCK_FUNCTION(KeyboardLowLevelCallback);
	LOCK_FUNCTION(MouseCallback);
	keyboard_lowlevel_callback = KeyboardLowLevelCallback;
	//mouse_callback = MouseCallback;
}


void InputManager::Shutdown() {
	//remove_mouse();
	remove_keyboard();
}


InputDevice* InputManager::GetDevice(uint i) {
	ASSERT(i < m_cDevices);
	ASSERT(m_prgDevices);
	return &m_prgDevices[i];
}


void InputManager::Tick(timestamp) {
	ASSERT(m_prgDevices);

	InputEventQueue* pq;
	InputEvent* pinev;
	for (uint i = 0; i < m_cDevices; ++i) {
		pq = m_prgDevices[i].LockQueue();

		while (!pq->empty()) {
			pinev = pq->front();
			Dispatch(pinev);
			pq->pop();
			delete pinev;
		}

		m_prgDevices[i].UnlockQueue();
	}
}

void InputManager::Dispatch(InputEvent* pinev) {
	InputListenerList::iterator pos;
	IInputListener* pListener;
	for (pos = m_lsListeners.begin(); pos != m_lsListeners.end(); ++pos) {
		pListener = *pos;
		if (pListener->HandleInput(pinev))
			return;
	}
	System.Log("Unhandled input event (type %d)", pinev->type);
}


void InputManager::InsertListener(IInputListener* pListener) {
	ASSERT(pListener);
	m_lsListeners.push_back(pListener);
}


void InputManager::Bind(DeviceId devid, int key, int cmd) {
	ASSERT(devid >= 0 && devid < m_cDevices);
	m_prgDevices[devid].Bind(key, cmd);
}
