#ifndef __KT_DISPLAY_INCLUDED__
#define __KT_DISPLAY_INCLUDED__

#include "basictypes.hpp"
#include "layer.hpp"
#include "image.hpp"

namespace kt {
namespace gfx {


typedef std::list<kt::gfx::Image*>	ImageList;

class Display {
public:
	Display();
	~Display();
	void Init(uint cx, uint cy);
	void Shutdown();
	gfx::Image* GetImage(PCSZ pszName);
	gfx::Image* GetOpaqueImage(PCSZ pszName);
	void BeginFrame();
	void FlushBlits();
	void EndFrame();
	void ToggleFullscreen();
	void Screenshot();
	uint Width();
	uint Height();
	
private:
	friend class gfx::Image;

	static gfx::Layer*	m_prgLayers;
	static uint			m_cLayers;
	ImageList			m_lsImages;
	bool				m_bIsFullscreen;
};


}
}


#endif // __KT_DISPLAY_INCLUDED__
