#ifndef __KT_INPUT_DEVICE_INCLUDED__
#define __KT_INPUT_DEVICE_INCLUDED__

#include "peterson-mutex.hpp"

namespace kt {

class InputEvent;

typedef std::queue<InputEvent*> InputEventQueue;

class InputDevice {
public:
	InputDevice();
	~InputDevice();
	
	void Init();
	void Bind(int key, int cmd);
	void Unbind(int key);
	void KeyboardCallback(int key);

	// Only one caller may lock our queue at a time
	InputEventQueue* LockQueue();
	void UnlockQueue();

private:
	typedef std::multimap<int, int> KeyCmdMap;

	// Only objects which are bound to some command will
	// be here.
	KeyCmdMap			m_mapBindings;
	PetersonMutex		m_pmQueue;
	InputEventQueue		m_qEvents;
};


} // namespace kt

#endif
