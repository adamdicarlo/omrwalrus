#ifndef __KT_WALRUS_GENERATOR_INCLUDED__
#define __KT_WALRUS_GENERATOR_INCLUDED__

#include "basictypes.hpp"
#include "game-world.hpp"
#include "process.hpp"

namespace kt {

	// forward declaration
	class ProcessScheduler;
	
	class WalrusGenerator : public IProcess {
		public:
			WalrusGenerator(int cWalruses) : m_cWalruses(cWalruses) {}
			virtual ~WalrusGenerator() { }
			virtual void Init(ProcessScheduler*, void* pWorld) {
				ASSERT(pWorld);
				m_yTop = -Random(80.0f, 160.f);
				m_yBottom = -(float)System.Display.Height() + m_yTop*2.0f;
				m_yGap = 75.0f;
				m_posNext = Vec2D(System.Display.Width() + 31.0f, m_yTop);
				m_pWorld = (GameWorld*) pWorld;
				m_countdown = System.TicksPerSec() * 2;
			}
			virtual bool Tick(timestamp) {
				if (--m_countdown <= 0) {
					m_countdown = (int)(System.TicksPerSec() * Random(9.0f,15.0f));
					
					EntityPtr wally = m_pWorld->AddWalrus();
					wally->SetPos(m_posNext);
					m_posNext.y -= Random(m_yGap, m_yGap + 72.0f);
					
					// cycle through spawn locations vertically
					if (m_posNext.y <= m_yBottom)
						m_posNext.y = m_yTop;
						
					--m_cWalruses;
					if (m_cWalruses <= 0)
						return false;
				}
				return true;
			}
			
			virtual void Render() {}
			
		private:
			float      m_yTop, m_yBottom, m_yGap;
			Vec2D      m_posNext;
			GameWorld* m_pWorld;
			int m_cWalruses;
			int m_countdown;
	};

	typedef counted_ptr<IProcess>	WalrusGeneratorPtr;
}

#endif // __KT_WALRUS_GENERATOR_INCLUDED__
