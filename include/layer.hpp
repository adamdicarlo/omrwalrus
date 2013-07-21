#ifndef __KT_LAYER_INCLUDED__
#define __KT_LAYER_INCLUDED__

#include <OpenLayer.hpp>
#include "fixed-array.hpp"

#define BLIT_NORMAL				1
#define BLIT_TRANSFORMED		2
#define BLIT_TRANSFORMEDPIVOT	3

namespace kt {
namespace gfx {

enum LayerId {

	LayerFarthest = 0,
	LayerFar      = 1,
	LayerMiddle   = 2,
	LayerClose    = 3,
	LayerClosest  = 4,
	kNumLayers    = 5
};


class Image;
class Layer {
public:
	void Init(uint cMaxBlits);
	void EnqueueBlitSC(ol::Bitmap* pbmp, float x, float y, float opacity);
	void EnqueueBlit(ol::Bitmap* pbmp, float x, float y, float opacity);
	void EnqueueBlit(ol::Bitmap* pbmp, float x, float y, float w, float h, float angle, float opacity);
	void EnqueueBlit(ol::Bitmap* pbmp, float x, float y, float w, float h, float xPivot, float yPivot, float angle, float opacity);
	void FlushBlits();


protected:
	struct Blit {
		uint		type;
		ol::Bitmap*	pbmp;
		float x, y, xZoom, yZoom, angle, opacity;
		float xPivot, yPivot;
	};

	FixedArray<Blit>	m_faBlits;
};


} // namespace gfx
} // namespace kt

#endif // __KT_LAYER_INCLUDED__
