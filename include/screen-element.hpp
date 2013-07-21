#ifndef __KT_SCREEN_ELEMENT_INCLUDED__
#define __KT_SCREEN_ELEMENT_INCLUDED__

#include "input.hpp"

namespace kt {


class IScreenElement {
public:
	virtual void Render() = 0;
	virtual void Tick(real rDeltaFrames, real rDeltaSeconds) = 0;
	virtual bool HandleInput(const InputEvent* pinev);
	virtual bool IsActive() const;
private:

};


} // namespace kt


#endif // __KT_SCREEN_ELEMENT_INCLUDED__
