#include "basictypes.hpp"
#include "display.hpp"
#include "image.hpp"
#include "system.hpp"

using namespace kt;
using namespace gfx;


Image::Image() : m_pbmp(new ol::Bitmap())
{
}


Image::~Image() {
	delete m_pbmp;
}


void Image::Load(PCSZ pszName) {
	ASSERT(pszName);
	ASSERT(m_pbmp);

	String strFull = "data/";
	strFull += pszName;

	m_pbmp->Load(strFull.c_str(), ol::CONVERT_MAGIC_PINK);
	if (!m_pbmp->IsValid())
		System.Error("Error loading '%s'", strFull.c_str());
	m_strName = pszName;
	m_bIsOpaque = false;
}

void Image::LoadOpaque(PCSZ pszName) {
	ASSERT(pszName);
	ASSERT(m_pbmp);

	String strFull = "data/";
	strFull += pszName;

	m_pbmp->Load(strFull.c_str());
	if (!m_pbmp->IsValid())
		System.Error("Error loading '%s'", strFull.c_str());
	m_strName = pszName;
	m_bIsOpaque = true;
}

void Image::Reload() {
	ASSERT(m_pbmp);
	
	String str = m_strName;
	m_pbmp->Destroy();
	
	if (m_bIsOpaque)
		this->LoadOpaque(str.c_str());
	else
		this->Load(str.c_str());
}

PCSZ Image::GetName() const {
	return m_strName.c_str();
}


// renders to absolute screen coords
void Image::RenderSC(LayerId layer, float x, float y, float opacity /* = 1.0 */) {
	ASSERT(layer >= 0 && layer < kNumLayers);
	gfx::Display::m_prgLayers[layer].EnqueueBlitSC(m_pbmp, x, y, opacity);
}


// renders centered around coordinates in game space (not screen space)
void Image::RenderCentered(LayerId layer, float x, float y, float opacity /* = 1.0 */) {
	ASSERT(layer >= 0 && layer < kNumLayers);
	x -= (float)  (m_pbmp->Width() >> 1);
	y += (float)  (m_pbmp->Height() >> 1);
	gfx::Display::m_prgLayers[layer].EnqueueBlit(m_pbmp, x, y, opacity);
}

void Image::RenderCenteredTransformed(LayerId layer, float x, float y, float w, float h, float angle, float opacity /* = 1.0*/) {
	gfx::Display::m_prgLayers[layer].EnqueueBlit(m_pbmp, x, y, w, h, angle, opacity);
}


void Image::Render(LayerId layer, float x, float y, float opacity /* = 1.0 */) {
	ASSERT(layer >= 0 && layer < kNumLayers);
	gfx::Display::m_prgLayers[layer].EnqueueBlit(m_pbmp, x, y, opacity);
}

void Image::RenderTransformed(LayerId layer, float x, float y, float w, float h, float angle, float opacity /* = 1.0*/) {

	gfx::Display::m_prgLayers[layer].EnqueueBlit(m_pbmp, x, y, w, h, angle, opacity);
}

void Image::RenderTransformed(LayerId layer, float x, float y, float w, float h, float xPivot, float yPivot, float angle, float opacity /* = 1.0*/) {
	gfx::Display::m_prgLayers[layer].EnqueueBlit(m_pbmp, x, y, w, h, xPivot, yPivot, angle, opacity);
}

