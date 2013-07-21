#ifndef __KT_INPUT_EVENT_INCLUDED__
#define __KT_INPUT_EVENT_INCLUDED__

namespace kt {

enum InputType {
    INPUT_ANALOG, INPUT_BUTTONDOWN, INPUT_BUTTONUP
};

struct InputEvent {
	InputType type;
	real xAxis;
	real yAxis;
	int  iButton;
	int  cmd;
};


} // namespace kt

#endif // __KT_INPUT_EVENT_INCLUDED__
