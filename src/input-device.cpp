#include "basictypes.hpp"
#include "input.hpp"
#include "system.hpp"

using namespace kt;


InputDevice::InputDevice() {

}


InputDevice::~InputDevice() {

}


void InputDevice::Bind(int key, int cmd) {
	m_mapBindings.insert(std::make_pair(key, cmd));
}



void InputDevice::Unbind(int key) {
	m_mapBindings.erase(key);
}


InputEventQueue* InputDevice::LockQueue() {
	m_pmQueue.Lock(PM_THREAD_BETA);
	return &m_qEvents;
}


void InputDevice::UnlockQueue() {
	m_pmQueue.Unlock(PM_THREAD_BETA);
}



/*
 * Allegro callback.
 * Forwards responsibility to the InputDevice object associated with the keyboard
 */
void KeyboardLowLevelCallback(int key) {
	InputDevice* pKB = System.Input.GetDevice(DeviceKeyboard);
	pKB->KeyboardCallback(key);
}



#define KEY_RELEASED_MASK   (1 << 7)
void InputDevice::KeyboardCallback(int key) {
	KeyCmdMap::iterator pos;

	int realkey = key & ~KEY_RELEASED_MASK;
	for (pos = m_mapBindings.lower_bound(realkey);
		 pos != m_mapBindings.upper_bound(realkey); ++pos) {
		//System.Log("Inside loop");
		// InputManager deletes these events after dequeueing them
		InputEvent* pinev = new InputEvent();
		ASSERT(pinev);
		if (!pinev)
			return;

		if (key & KEY_RELEASED_MASK)
			pinev->type = INPUT_BUTTONUP;
		else
			pinev->type = INPUT_BUTTONDOWN;

		pinev->iButton = realkey;
		pinev->cmd = pos->second;
 
		// Lock the queue so that it isn't read from by the main thread
		// at the same time we're writing to it
		m_pmQueue.Lock(PM_THREAD_ALPHA);
		m_qEvents.push(pinev);
		m_pmQueue.Unlock(PM_THREAD_ALPHA);
	}
}
END_OF_FUNCTION(KeyboardLowLevelCallback);


/*
 * Allegro callback
 */
void MouseCallback(int flags) {
	flags++;
}
END_OF_FUNCTION(MouseCallback);
