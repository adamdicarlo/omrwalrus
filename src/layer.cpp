#include "basictypes.hpp"
#include "display.hpp"
#include "image.hpp"
#include "layer.hpp"
#include "system.hpp"


using namespace kt;
using namespace gfx;



void Layer::Init(uint cMaxBlits) {
	m_faBlits.Init(cMaxBlits);
}

// Screen coords
void Layer::EnqueueBlitSC(ol::Bitmap* pbmp, float x, float y, float opacity) {
	Blit* pblit;
	if (m_faBlits.Allocate(&pblit)) {
		pblit->type    = BLIT_NORMAL;
		pblit->pbmp    = pbmp;
		pblit->x       = x;
		pblit->y       = y;
		pblit->xZoom   = 1.0f;
		pblit->yZoom   = 1.0f;
		pblit->opacity = opacity;
	} else {
		System.Warning("Layer::EnqueueBlit: out of blits");
	}
}


void Layer::EnqueueBlit(ol::Bitmap* pbmp, float x, float y, float opacity) {
    Blit* pblit;
	
	// transform from Box2D coordinates to screen coordinates...

    if (m_faBlits.Allocate(&pblit)) {
		pblit->type    = BLIT_NORMAL;
        pblit->pbmp    = pbmp;
		pblit->x       = x;
		pblit->y       = -y;
		pblit->xZoom   = 1.0f;
		pblit->yZoom   = 1.0f;
		pblit->opacity = opacity;
    } else {
        System.Warning("Layer::EnqueueBlit: out of blits");
    }
}

// upper left corner of screen is 0,0
// bottom left corner of screen is 0, -SCREENH
// bottom right is SCREENW, -SCREENH...

void Layer::EnqueueBlit(ol::Bitmap* pbmp, float x, float y, float w, float h, float angle, float opacity) {
    Blit* pblit;

    if (m_faBlits.Allocate(&pblit)) {
		pblit->type    = BLIT_TRANSFORMED;
        pblit->pbmp    = pbmp;
		pblit->x       = x;
		pblit->y       = -y;
		pblit->xZoom   = w;
		pblit->yZoom   = h;
		pblit->angle   = angle;
		pblit->opacity = opacity;
    } else {
        System.Warning("Layer::EnqueueBlit: out of blits");
    }
}


void Layer::EnqueueBlit(ol::Bitmap* pbmp, float x, float y, float w, float h, float xPivot, float yPivot, float angle, float opacity) {
	Blit* pblit;

	if (m_faBlits.Allocate(&pblit)) {
		pblit->type    = BLIT_TRANSFORMEDPIVOT;
		pblit->pbmp    = pbmp;
		pblit->x       = x;
		pblit->y       = -y;
		pblit->xPivot  = xPivot;
		pblit->yPivot  = yPivot;
		pblit->xZoom   = w;
		pblit->yZoom   = h;
		pblit->angle   = angle;
		pblit->opacity = opacity;
	} else {
		System.Warning("Layer::EnqueueBlit: out of blits");
	}
}


void Layer::FlushBlits() {
	uint cBlits = m_faBlits.GetCount();
	for (uint i = 0; i < cBlits; ++i) {
		Blit* pblit = m_faBlits.GetAt(i);
		
		if (pblit->type == BLIT_TRANSFORMED) {
			pblit->pbmp->BlitTransformed(pblit->x, pblit->y, pblit->xZoom, pblit->yZoom, pblit->angle, pblit->opacity);
			//System.Log("Blit Transformed t: %d pos: %g, %g  zoom: %g, %g  angle: %g   opacity:  %g",
				//pblit->type, pblit->x, pblit->y, pblit->xZoom, pblit->yZoom, pblit->angle, pblit->opacity);
		} else if (pblit->type == BLIT_TRANSFORMEDPIVOT) {
			pblit->pbmp->BlitTransformed(pblit->x, pblit->y, pblit->xPivot, pblit->yPivot, pblit->xZoom, pblit->yZoom, pblit->angle, pblit->opacity);
		} else {
			pblit->pbmp->Blit(pblit->x, pblit->y, pblit->opacity);
		}
	}
	m_faBlits.Clear();
}
