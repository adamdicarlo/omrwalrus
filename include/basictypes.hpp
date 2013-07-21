#ifndef __KT_BASICTYPES_INCLUDED__
#define __KT_BASICTYPES_INCLUDED__

#include "Box2D/MathUtils.h" // for Random
#include <allegro.h> // so EVERYONE gets ASSERT
#include <OpenLayer.hpp> // for Vec2D ... sigh
#include "counted_ptr.hpp"
#include <list>
#include <map>
#include <string>
#include <vector>
#include <queue>

using namespace ol; // openlayer

typedef float			real;
typedef unsigned int	uint; // 32+ bits
typedef int				sint; // 32+ bits
typedef unsigned long	u64;
typedef long			s64;
typedef int				s32;
typedef unsigned int	u32;
typedef short			s16;
typedef unsigned short	u16;
typedef unsigned char	byte;
typedef char			s8;

namespace kt {
	typedef float			timestamp;
	typedef std::string		String;
	typedef const char*		PCSZ;
	typedef char*			PSZ;
}

#endif // __KT_BASICTYPES_INCLUDED__
