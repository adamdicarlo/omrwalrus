#ifndef __KT_IMAGE_INCLUDED__
#define __KT_IMAGE_INCLUDED__

#include <OpenLayer.hpp>
#include "layer.hpp"  // LayerId

namespace kt {
namespace gfx {


class Display;

class Image {
public:
	Image();
	~Image();
	void Load(PCSZ pszName);
	void LoadOpaque(PCSZ pszName);
	void Reload();
	PCSZ GetName() const;
	void RenderCentered(LayerId layer, float x, float y, float opacity = 1.0 );
	void RenderCenteredTransformed(LayerId layer, float x, float y, float w, float h, float angle, float opacity = 1.0);
	void Render(LayerId layer, float x, float y, float opacity = 1.0);
	void RenderSC(LayerId layer, float x, float y, float opacity = 1.0);
	void RenderTransformed(LayerId layer, float x, float y, float w, float h, float angle, float opacity = 1.0);
	void RenderTransformed(LayerId layer, float x, float y, float w, float h, float xPivot, float yPivot, float angle, float opacity /* = 1.0*/);
	inline uint Width() {
			ASSERT(m_pbmp && m_pbmp->IsValid());
			return m_pbmp->Width();
	}
	
	inline uint Height() {
		ASSERT(m_pbmp && m_pbmp->IsValid());
		return m_pbmp->Height();
	}
	
	ol::Bitmap*     m_pbmp;

protected:
	String			m_strName;
	bool            m_bIsOpaque;
};



}
}
#endif // __KT_IMAGE_INCLUDED__
