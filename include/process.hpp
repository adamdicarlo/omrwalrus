#ifndef __KT_PROCESS_INCLUDED__
#define __KT_PROCESS_INCLUDED__

#include "basictypes.hpp"
#include "process.hpp"

namespace kt {

// forward declaration
class ProcessScheduler;

class IProcess {
public:
	virtual ~IProcess() { }
	virtual void Init(ProcessScheduler* psched, void* pvContext) = 0;
	virtual bool Tick(timestamp ts) = 0;
	virtual void Render() = 0;
};


}

#endif // __KT_PROCESS__
