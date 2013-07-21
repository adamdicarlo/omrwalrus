#include "basictypes.hpp"
#include "display.hpp"
#include "system.hpp"
#include <OpenLayer.hpp>
#include <cstdio>

#define MAX_BLITS_PER_LAYER		320


using namespace kt;


/*
 * Static data
 */
gfx::Layer* gfx::Display::m_prgLayers = 0;
uint        gfx::Display::m_cLayers   = 0;



gfx::Display::Display() {
}

gfx::Display::~Display() {

}

void gfx::Display::Init(uint cx, uint cy) {
	ASSERT(cLayers > 0);

	if (!ol::Setup::SetupProgram(false, false, false))
		System.Error("SetupProgram failed");

	if (!ol::Setup::SetupScreen(cx, cy, WINDOWED))
		System.Error("SetupScreen failed for %dx%d", cx, cy);
	m_bIsFullscreen = false;
	
	m_prgLayers = new gfx::Layer[kNumLayers];
	m_cLayers = kNumLayers;
	for (uint i = 0; i < m_cLayers; ++i)
		m_prgLayers[i].Init(MAX_BLITS_PER_LAYER);
}


void gfx::Display::ToggleFullscreen() {
	uint cx = ol::Setup::GetWindowWidth();
	uint cy = ol::Setup::GetWindowHeight();
	System.Log("Toggling fullscreen (%dx%d)", cx, cy);
	m_bIsFullscreen = !m_bIsFullscreen;
	if (!ol::Setup::SetupScreen(cx, cy, m_bIsFullscreen))
		System.Error("SetupScreen failed for %dx%d", cx, cy);

	// weird openGL concurrency(?) bug -- without a pause here,
	// the openGL driver deadlocks while reloading images
	rest(1000);

	// Must reload all bitmaps now.
	ImageList::iterator i;
	for (i = m_lsImages.begin(); i != m_lsImages.end(); ++i) {
		System.Log("Reloading %s", (*i)->GetName());
		(*i)->Reload();
	}
}


uint gfx::Display::Width() {
	return ol::Setup::GetWindowWidth();
}


uint gfx::Display::Height() {
	return ol::Setup::GetWindowHeight();
}


void gfx::Display::Screenshot() {
	char filename[100];
	for (int i = 0; i < 999; ++i) {
		sprintf(filename, "ss/%03d.png", i);
		FILE* fp;
		if ((fp = fopen(filename, "r")) == NULL)
			break;
		else
			fclose(fp);
	}
	System.Log("Screenshot: %s (size %dx%d)", filename, Width(), Height());
	ol::Bitmap bmp(Width(), Height());
	bmp.CopyFromScreen(0, 0);
	bmp.Save(filename);
}

void gfx::Display::Shutdown() {
	ImageList::iterator i;
	for (i = m_lsImages.begin(); i != m_lsImages.end(); ++i) {
		delete (*i);
	}
	m_lsImages.clear();
	delete [] m_prgLayers;
}


/*
 * Gets an image by name, whether or not already loaded.
 */
gfx::Image* gfx::Display::GetImage(PCSZ pszName) {
	ImageList::iterator i;
	gfx::Image* pimg;

	for (i = m_lsImages.begin(); i != m_lsImages.end(); ++i) {
		pimg = *i;
		if (stricmp(pimg->GetName(), pszName) == 0) {
			return (*i);
		}
	}

	gfx::Image* pimgNew = new gfx::Image();
	pimgNew->Load(pszName);
	m_lsImages.push_back(pimgNew);
	System.Log("Loaded image: %s", pszName);
	return pimgNew;
}


/*
 * Gets an image by name, whether or not already loaded.
 */
gfx::Image* gfx::Display::GetOpaqueImage(PCSZ pszName) {
	ImageList::iterator i;
	gfx::Image* pimg;

	for (i = m_lsImages.begin(); i != m_lsImages.end(); ++i) {
		pimg = *i;
		if (stricmp(pimg->GetName(), pszName) == 0) {
			return (*i);
		}
	}

	gfx::Image* pimgNew = new gfx::Image();
	pimgNew->LoadOpaque(pszName);
	m_lsImages.push_back(pimgNew);
	System.Log("Loaded opaque image: %s", pszName);
	return pimgNew;
}



void gfx::Display::FlushBlits() {
	for (uint i = 0; i < m_cLayers; ++i) {
		m_prgLayers[i].FlushBlits();
	}
}



void gfx::Display::BeginFrame() {

}


void gfx::Display::EndFrame() {
	ol::Canvas::Refresh();
}


