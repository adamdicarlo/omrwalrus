#ifndef __PETERSON_MUTEX_INCLUDED__
#define __PETERSON_MUTEX_INCLUDED__

#define PM_THREAD_ALPHA		0
#define PM_THREAD_BETA		1

/**
 * Implementation of Peterson's algorithm which allows
 * two threads to synchronize access to a resource.
 * Based on implementation found in Modern Operating Systems,
 * 2nd Ed. (Andrew S. Tanenbaum)
 */
class PetersonMutex {
public:
	PetersonMutex();

	/* Wait to acquire a lock to the resource; you must tell this function
	 * which of the two threads you are
	 */
	void Lock(unsigned int thread);
	void Unlock(unsigned int thread);
protected:
	unsigned int _turn;
	bool _interested[2];
};

#endif
