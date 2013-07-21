#ifndef __KT_FIXED_ARRAY_INCLUDED__
#define __KT_FIXED_ARRAY_INCLUDED__



template<typename T>
class FixedArray {
public:
	FixedArray() : m_prgElems(0), m_cUsed(0), m_cMax(0) {}

	~FixedArray() {
		delete [] m_prgElems;
	}

	void Init(uint size) {
		ASSERT(!m_prgElems);
		m_prgElems = new T[size];
		m_cMax = size;
		m_cUsed = 0;
	}

	bool Allocate(T** ppdata) {
		ASSERT(ppdata);
		ASSERT(m_prgElems);
		ASSERT(m_cMax > 0);

		if (m_cUsed >= m_cMax)
			return false;
		*ppdata = &m_prgElems[m_cUsed];
		++m_cUsed;
		return true;
	}

	void Clear() {
		m_cUsed = 0;
	}

	T* GetAt(uint i) {
		ASSERT(m_prgElems && i < m_cUsed);
		return &m_prgElems[i];
	}

	uint GetCount() const {
		return m_cUsed;
	}

protected:
	T*    m_prgElems;
	uint  m_cUsed;
	uint  m_cMax;
};


#endif // __KT_FIXED_ARRAY_INCLUDED__

