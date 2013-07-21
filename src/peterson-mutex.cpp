
#include "peterson-mutex.hpp"
#include "basictypes.hpp" // ASSERT


/**
 * Implementation of Peterson's algorithm which allows
 * two threads to synchronize access to a resource.
 * Based on implementation found in Modern Operating Systems,
 * 2nd Ed. (Andrew S. Tanenbaum)
 */
PetersonMutex::PetersonMutex() {
	_interested[0] = _interested[1] = false;
}

/* Wait to acquire a lock to the resource; you must tell this function
 * which of the two threads you are
 */
void PetersonMutex::Lock(unsigned int thread) {
	ASSERT(thread < 2);
	unsigned int other;
	other = 1 - thread;
	_interested[thread] = true;
	_turn = thread;
	while (_turn == thread && _interested[other])
		;
}

void PetersonMutex::Unlock(unsigned int thread) {
	ASSERT(thread < 2);
	_interested[thread] = false;
}

