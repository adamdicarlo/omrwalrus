#ifndef __KT_INPUT_INCLUDED__
#define __KT_INPUT_INCLUDED__

#include "basictypes.hpp"
#include "event.hpp"
#include "input-device.hpp"
#include "input-event.hpp"

namespace kt {

enum DeviceId {
	DeviceKeyboard	= 0,
	DeviceMouse		= 1,
	DeviceJoystick1 = 2,
	DeviceJoystick2 = 3
};


class IInputListener {
public:
	virtual ~IInputListener() { }
	virtual bool HandleInput(const InputEvent*) = 0;
};


typedef std::list<IInputListener*>	InputListenerList;

class InputManager {
public:
	InputManager();
	~InputManager();

	void Init();
	void Shutdown();
	void Tick(timestamp tsTime);
	void Bind(DeviceId devid, int key, int cmd);
	void InsertListener(IInputListener* pListener);
	InputDevice* GetDevice(uint i);

private:
	void Dispatch(InputEvent*);

	InputListenerList		m_lsListeners;
	static InputManager*	m_pInstance;
	InputDevice*			m_prgDevices;
	uint					m_cDevices;

};


} // namespace kt


#endif // __KT_INPUT_INCLUDED__
