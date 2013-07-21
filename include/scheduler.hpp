#ifndef __KT_PROCESS_SCHEDULER_INCLUDED__
#define __KT_PROCESS_SCHEDULER_INCLUDED__

#include "process.hpp"

namespace kt {


typedef counted_ptr<IProcess>		IProcessPtr;
typedef std::list<IProcessPtr>		IProcessList;

class ProcessScheduler {
public:
	ProcessScheduler();
	void InsertProcess(IProcessPtr cpNewProcess, void* pvContext);
	void RemoveProcess(IProcessPtr cpProcess);
	void Tick(timestamp ts);
	void Clear() {
		m_lsProcesses.clear();
	}

private:
	IProcessList	m_lsProcesses;
};


}

#endif // __KT_PROCESS_SCHEDULER_INCLUDED__
