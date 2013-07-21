#include "basictypes.hpp"
#include "scheduler.hpp"
#include "system.hpp"


using namespace kt;


ProcessScheduler::ProcessScheduler() {
}


void ProcessScheduler::InsertProcess(IProcessPtr cpNewProcess, void* pvContext) {
	ASSERT(cpNewProcess.get());
	cpNewProcess->Init(this, pvContext);
	m_lsProcesses.push_back(cpNewProcess);
}



void ProcessScheduler::Tick(timestamp ts) {
	IProcessList::iterator i;
	for (i = m_lsProcesses.begin(); i != m_lsProcesses.end(); ++i) {
		if (!((*i)->Tick(ts))) {
			i = m_lsProcesses.erase(i);
		}
	}
}


void ProcessScheduler::RemoveProcess(IProcessPtr cpProcess) {
	IProcessList::iterator i;
	for (i = m_lsProcesses.begin(); i != m_lsProcesses.end(); ++i) {
		if ((*i).get() == cpProcess.get()) {
			i = m_lsProcesses.erase(i);
			return;
		}
	}
	System.Warning("RemoveProcess failed");
}
